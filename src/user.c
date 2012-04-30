/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sat 22 Jan 2011 12:00:18 AM CST
 *
 *
 * Description:  
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "foperate.h"
#include "iwindow.h"
#include "user.h"
#include "debug.h"

int lcrt_user_create_config(struct lcrt_user *luser);

static const char *lcrt_user_get_db_name(struct lcrt_user *luser)
{
    return luser->db.db_name;
}
static const char *lcrt_user_get_db_table(struct lcrt_user *luser)
{
    return luser->db.db_table;
}

int lcrt_user_init_config(struct lcrt_user *luser)
{
    char db_name[256];

    if (luser == NULL)
        return EINVAL;

    memset(luser, 0, sizeof(struct lcrt_user));
    snprintf(db_name, sizeof(db_name), "%s/%s/", lcrt_config_get_local_dir(), LCRT_USER_CONFIG_DIR);
    lcrt_mkdir(db_name);
    strcat(db_name, LCRT_USER_DB);
    lcrt_config_init(&luser->db, db_name, LCRT_USER_TABLE);
    INIT_LIST_HEAD(&luser->child);

    luser->get_db = lcrt_user_get_db_name;
    luser->get_tb = lcrt_user_get_db_table;

    return 0;
}
int lcrt_user_destroy_config(struct lcrt_user *luser)
{
    struct lcrtc_user *user;
    struct list_head *pos, *tmp;

    if (luser == NULL)
        return EINVAL;

    list_for_each_safe(pos, tmp, &luser->child) {
        user = list_entry(pos, struct lcrtc_user, brother);
        lcrtc_user_destroy(user);
    }
    return LCRTE_OK;
}
int lcrt_user_load_config(struct lcrt_user *luser)
{
    int rv;
    struct lcrtc_user *user;
    const char *default_command;
    const char *default_folder;

    const char *proto[LCRT_PROTOCOL_NUMBER] = {LCRT_PROTOCOL_NAME};

    if (luser == NULL)
        return EINVAL;

    debug_print("SQL:SELECT * FROM %s\n", luser->get_tb(luser));
    rv = luser->db.select(&luser->db, "SELECT * FROM %s", luser->get_tb(luser));
    if (rv == LCRTE_NO_TABLE) {
        lcrt_user_create_config(luser);
        return LCRTE_NO_CONFIG;
    }

    while (rv == LCRTE_OK) {
        user = lcrtc_user_create();
        if (user == NULL)
            return ENOMEM;

        strncpy(user->name, 
                luser->db.get_text_col(&luser->db, LCRT_USER_NAME), 
                HOSTNAME_LEN);

        strncpy(user->hostname, 
                luser->db.get_text_col(&luser->db, LCRT_USER_HOSTNAME), 
                HOSTNAME_LEN);

        user->protocol = luser->db.get_int_col(&luser->db, LCRT_USER_PROTOCOL);
        strncpy(user->username,
                luser->db.get_text_col(&luser->db, LCRT_USER_USERNAME),
                USERNAME_LEN);

        strncpy(user->password,
                luser->db.get_text_col(&luser->db, LCRT_USER_PASSWORD),
                PASSWORD_LEN);

        user->port = luser->db.get_int_col(&luser->db, LCRT_USER_PORT);
        default_command = luser->db.get_text_col(&luser->db, LCRT_USER_DEFAULT_COMMAND);
        if (default_command != NULL) {
            strncpy(user->command, default_command,DEFAULT_COMMAND_LEN);
        } else {
            strcpy(user->command, "");
        }
        default_folder = luser->db.get_text_col(&luser->db, LCRT_USER_FOLDER);
        if (default_folder != NULL) {
            strncpy(user->folder , default_folder, DIRNAME_LEN);
        } else {
            strcpy(user->folder, "");
        }
        user->is_folder = luser->db.get_int_col(&luser->db, LCRT_USER_IS_FOLDER);

        lcrtc_user_refresh(user);
        lcrt_user_add(luser, user);

        rv = luser->db.get_row(&luser->db);

        debug_print("[%-10s|%-10s]:%s %s %s@%s -p %d && %s [%s]\n",
                luser->get_db(luser),
                luser->get_tb(luser),
                user->name,
                proto[user->protocol],
                user->username,
                user->hostname,
                user->port,
                user->command,
                user->folder);
    }

    return rv;
}
int lcrt_user_rename(struct lcrt_user *luser, struct lcrtc_user *user, char *new_name, int update_to_db)
{
    int rv = LCRTE_OK;
    struct lcrtc_user *child;
    char name[DIRNAME_LEN + 1];

    /* if we are not a folder */
    if (!user->is_folder) {
        /* only update structure in memory */
        if (!update_to_db) {
            strcpy(user->name, new_name);
            return LCRTE_OK;
        }
        /* update new name to database */
        rv = luser->db.exec(&luser->db, 
            "UPDATE %s SET name='%s' WHERE name='%s'",
            luser->get_tb(luser),
            new_name,
            user->name);
        if (luser->db.changes(&luser->db) == 1) {
            strcpy(user->name, new_name);
            return LCRTE_OK;
        }
    } else {
        strcpy(name, user->folder);
        /* If we want update the name of a folder, we should update all it's
         * children. so search in user list, if user->folder match with new_name,
         * update user too.
         */
        list_for_each_entry(child, &luser->child, brother) {
            if (strcmp(name, child->folder) != 0)
                continue;
            if (!update_to_db) {
                if (child->is_folder) {
                    strcpy(child->name, new_name);
                }
                strcpy(child->folder, new_name);
                continue;
            }
            if (!child->is_folder) {
                rv = luser->db.exec(&luser->db, 
                        "UPDATE %s SET folder='%s' WHERE name='%s'",
                        luser->get_tb(luser),
                        new_name,
                        child->name);
            } else {
                rv = luser->db.exec(&luser->db, 
                        "UPDATE %s SET name='%s', folder='%s' WHERE name='%s'",
                        luser->get_tb(luser),
                        new_name,
                        new_name,
                        child->name);
            }
            if (luser->db.changes(&luser->db) == 1) {
                if (child->is_folder) {
                    strcpy(child->name, new_name);
                }
                strcpy(child->folder, new_name);
            }
        }
    }
    luser->db.close(&luser->db);
    return rv;

}
int lcrt_user_save_one(struct lcrt_user *luser, struct lcrtc_user *user)
{
    luser->db.exec(&luser->db, 
        "ALTER TABLE %s ADD COLUMN command VARCHAR(512) DEFAULT NULL",
        luser->get_tb(luser));
    luser->db.exec(&luser->db, 
        "ALTER TABLE %s ADD COLUMN folder VARCHAR(64) DEFAULT NULL",
        luser->get_tb(luser));
    luser->db.exec(&luser->db, 
        "ALTER TABLE %s ADD COLUMN is_folder INTERGER DEFAULT 0",
        luser->get_tb(luser));
    luser->db.exec(&luser->db, 
        "UPDATE %s SET protocol=%d, hostname='%s', username='%s', password='%s', port=%d, command='%s', folder='%s', is_folder='%d' WHERE name='%s'",
        luser->get_tb(luser),
        user->protocol,
        user->hostname,
        user->username,
        user->password,
        user->port,
        user->command,
        user->folder,
        user->is_folder,
        user->name);

    if (luser->db.changes(&luser->db) == 0) {
        luser->db.exec(&luser->db,
                       "INSERT INTO %s VALUES('%s', '%s', %d, '%s', '%s', %d, '%s', '%s', %d)",
                       luser->get_tb(luser),
                       user->name,
                       user->hostname,
                       user->protocol,
                       user->username,
                       user->password,
                       user->port,
                       user->command,
                       user->folder,
                       user->is_folder);
    }
    return LCRTE_OK;
}
int lcrt_user_del_one(struct lcrt_user *luser, struct lcrtc_user *user, int del_from_db)
{
    if (del_from_db) {
        luser->db.exec(&luser->db, 
                   "DELETE FROM %s WHERE name='%s'",
                   luser->get_tb(luser),
                   user->name);
    }
    lcrtc_user_destroy(user);
    luser->db.close(&luser->db);
    return LCRTE_OK;
}
int lcrt_user_del_folder(struct lcrt_user *luser, struct lcrtc_user *folder, int del_from_db)
{
    struct lcrtc_user *user;
    struct list_head *pos, *tmp;
    char name[DIRNAME_LEN + 1];

    strcpy(name, folder->folder);

    list_for_each_safe(pos, tmp, &luser->child) {
        user = list_entry(pos, struct lcrtc_user, brother);
        if (strcmp(user->folder, name) == 0) {
            if (del_from_db) {
                luser->db.exec(&luser->db, 
                            "DELETE FROM %s WHERE folder='%s'",
                            luser->get_tb(luser),
                            name);
            }
            lcrtc_user_destroy(user);
        }
    }
    luser->db.close(&luser->db);
    return LCRTE_OK;
}

int lcrt_user_save_config(struct lcrt_user *luser)
{
    struct lcrtc_user *user;

    list_for_each_entry(user, &luser->child, brother) {
        lcrt_user_save_one(luser, user);
    }
    luser->db.close(&luser->db);
    return 0;
}
int lcrt_user_create_config(struct lcrt_user *luser)
{
    struct lcrtc_user *user;

    assert(luser);

    luser->db.exec(&luser->db, 
                         "CREATE TABLE %s( \
                          name V    ARCHAR(128) PRIMARY KEY, \
                          hostname  VARCHAR(128) NOT NULL, \
                          protocol  INTEGER DEFAULT 0, \
                          username  VARCHAR(64) DEFAULT NULL, \
                          password  VARCHAR(64) DEFAULT NULL, \
                          port      INTERGER DEFAULT 0, \
                          command   VARCHAR(512) DEFAULT NULL, \
                          folder    VARCHAR(64) DEFAULT NULL, \
                          is_folder INTERGER DEFAULT 0)",
                         luser->get_tb(luser));
    debug_where();
    list_for_each_entry(user, &luser->child, brother) {
        luser->db.exec(&luser->db,
                             "INSERT INTO %s VALUES('%s', '%s', %d, '%s', '%s', %d, '%s', '%s', '%d')",
                             luser->get_tb(luser),
                             user->name,
                             user->hostname,
                             user->protocol,
                             user->username,
                             user->password,
                             user->port,
                             user->command,
                             user->folder,
                             user->is_folder);

    }
    debug_where();
    luser->db.close(&luser->db);
    return 0;
}
int lcrt_user_add(struct lcrt_user *luser, struct lcrtc_user *user)
{
    if (luser && user) {
        list_add(&user->brother, &luser->child);
        return LCRTE_OK;
    } else {
        return EINVAL;
    }
}
void lcrt_user_del(struct lcrtc_user *user)
{
    if (user)
        list_del(&user->brother);
}
struct lcrtc_user *lcrtc_user_create(void)
{
    struct lcrtc_user *user;

    user = (struct lcrtc_user *)calloc(1, sizeof(struct lcrtc_user));
    if (user == NULL)
        return NULL;

    INIT_LIST_HEAD(&user->brother);
    return user;
}
int lcrtc_user_ref(struct lcrtc_user *user)
{
    if (user == NULL)
        return EINVAL;
    user->used++;

    return LCRTE_OK;
}
int lcrtc_user_unref(struct lcrtc_user *user)
{
    if (user == NULL)
        return EINVAL;
    if (user->used > 0)
        user->used--;

    return LCRTE_OK;
}
int lcrtc_user_is_dirty(struct lcrtc_user *user)
{
    if (user == NULL || user->dirty == FALSE)
        return 0;
    return 1;
}
int lcrtc_user_refresh(struct lcrtc_user *user)
{
    if (user)
        user->dirty = FALSE;

    return LCRTE_OK;
}
void lcrtc_user_destroy(struct lcrtc_user *user)
{
    debug_print("destroy_user = %p\n", user);
    if (user == NULL)
        return;
    lcrtc_user_unref(user);
    if (user->used == 0) {
        list_del(&user->brother);
        free(user);
    }
}
struct lcrtc_user *lcrt_user_find_by_name(struct lcrt_user *luser, const char *name)
{
    struct lcrtc_user *user;
    list_for_each_entry(user, &luser->child, brother) {
        if (strcmp(user->name, name) == 0)
            return user;
    }
    return NULL;
}
int lcrtc_user_set_data(
    struct lcrtc_user *user,
    const char *name, 
    const char *hostname,
    lcrt_protocol_t protocol,
    const char *username,
    const char *password,
    const char *command,
    int port,
    int dirty,
    const char *folder,
    int is_folder)
{
    if (user == NULL)
        return EINVAL;

    if (name == NULL)
        name = hostname;

    if (name != NULL)
        strncpy(user->name, name, HOSTNAME_LEN);

    if (hostname != NULL)
        strncpy(user->hostname, hostname, HOSTNAME_LEN);
    if (protocol != LCRT_PROTOCOL_UNKNOWN)
        user->protocol = protocol;

    if (username != NULL)
        strncpy(user->username, username, USERNAME_LEN);

    if (password != NULL)
        strncpy(user->password, password, USERNAME_LEN);

    if (port != -1)
        user->port = port;

    if (command != NULL) {
        strncpy(user->command, command, DEFAULT_COMMAND_LEN);
    }

    if (is_folder != -1)
        user->is_folder = is_folder;

    if (folder != NULL)
        strncpy(user->folder, folder, DIRNAME_LEN);

    user->dirty = dirty;
    return LCRTE_OK;
}
lcrt_protocol_t lcrt_user_get_protocol(const char *proto)
{
    char *str_proto[LCRT_PROTOCOL_NUMBER] = {LCRT_PROTOCOL_NAME};
    int i;
    for (i = 0; i < LCRT_PROTOCOL_NUMBER; i++) {
        if (strcmp(str_proto[i], proto) == 0)
            return i;
    }
    return LCRT_PROTOCOL_UNKNOWN;
}
void lcrtc_user_dump(struct lcrtc_user *user, const char *func_name)
{
#ifdef __LCRT_DEBUG__
    const char *proto[LCRT_PROTOCOL_NUMBER] = {LCRT_PROTOCOL_NAME};
    if (user == NULL)
        return;

    debug_print("==========%s=================\n", func_name);
    debug_print("protocol: %s\n", proto[user->protocol]);
    debug_print("hostname: %s\n", user->hostname);
    debug_print("username: %s\n", user->username);
    debug_print("password: %s\n", user->password);
    debug_print("===========================");
    int i;
    for (i = 0; i < strlen(func_name); i++)
        debug_print("=");
    debug_print("\n");
#endif
}
int lcrt_exec_check(lcrt_protocol_t prot)
{
    int rv, i;
    static int exist[LCRT_DEP_NUMBER] = {-1, -1, -1};
    char *dep_prog[] = {LCRT_DEP_PROG};
    if (exist[LCRT_DEP_SSH] == -1) {
        for (i = 0; i < LCRT_DEP_NUMBER; i++) {
            exist[i] = lcrt_echeck(dep_prog[i]);
        }
    }
    switch (prot) {
    case LCRT_PROTOCOL_SSH2:
    case LCRT_PROTOCOL_SSH1:
        rv = exist[LCRT_DEP_SSH];
        break;
    case LCRT_PROTOCOL_TELNET:
    case LCRT_PROTOCOL_TELNET_SSL:
        rv = exist[LCRT_DEP_TELNET];
        break;
    case LCRT_PROTOCOL_RLOGIN:
        rv = exist[LCRT_DEP_RLOGIN];
        break;
    case LCRT_PROTOCOL_SHELL:
        rv = exist[LCRT_DEP_SHELL];
        break;
    case LCRT_PROTOCOL_SERIAL:
    default:
        rv = -1;
        break;
    }
    return rv;
}
/**
 * @brief find a new label like keyword.
 * @param lwindow main window structure
 * @param [in] keyword the keyword for search
 * @param [out] label return the new unused label we found
 * @return 0 ok, otherwise is error
 */
int lcrt_user_find_unused_label(struct lcrt_window *lwindow, 
    const char *keyword /* in */, char *label /* out */)
{
    int i;

    if (!lwindow || !keyword || !label)
        return LCRTE_INVAL;

    strcpy(label, keyword);

    if (lcrt_user_find_by_name(&lwindow->u_config, keyword) != NULL) {
        for (i = 1; i <= LCRT_MAX_LABEL; i++) {
            sprintf(label, "%s (%d)", keyword, i);
            if (lcrt_user_find_by_name(&lwindow->u_config, label) == NULL)
                return LCRTE_OK;
        };
    }
    return LCRTE_OK;
}
