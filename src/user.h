/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sat 15 Jan 2011 04:33:36 PM CST
 *
 * Description:  
 */

#ifndef __LCRT_USER_H__
#define __LCRT_USER_H__
#include <gtk/gtk.h>
#include "mkconfig.h"
#include "list.h"
#include "protocol.h"

#define LCRT_MAX_LABEL 100

#ifndef __GTK_H__
    #define FALSE (0)
    #define TRUE !(FALSE)
    typedef int boolean;
#else
    typedef gboolean boolean;
#endif

#define LCRT_USER_CONFIG_DIR "etc/user"
#define LCRT_USER_DB "user.db"
#define LCRT_USER_TABLE "user"

#define HOSTNAME_LEN 128
#define USERNAME_LEN 64
#define PASSWORD_LEN 64
#define DEFAULT_COMMAND_LEN 512
#define DIRNAME_LEN 64
enum {
    LCRT_USER_NAME,
    LCRT_USER_HOSTNAME,
    LCRT_USER_PROTOCOL,
    LCRT_USER_USERNAME,
    LCRT_USER_PASSWORD,
    LCRT_USER_PORT,
    LCRT_USER_DEFAULT_COMMAND,
    LCRT_USER_FOLDER,
    LCRT_USER_IS_FOLDER,
    LCRT_USER_CONFIG_NUMBER
};
struct lcrtc_user {
    char name [HOSTNAME_LEN + 1];
    char hostname[HOSTNAME_LEN + 1];
    lcrt_protocol_t protocol;
    char username[USERNAME_LEN + 1];
    char password[PASSWORD_LEN + 1];
    unsigned int port;
    char command[DEFAULT_COMMAND_LEN + 1];
    char folder[DIRNAME_LEN + 1];
    int is_folder;
    int used;
    int dirty;
    struct list_head brother;
};
struct lcrt_user {
    struct lcrt_config db;
    struct list_head child;
    const char *(*get_db)(struct lcrt_user *luser);
    const char *(*get_tb)(struct lcrt_user *luser);
};

int lcrt_user_init_config(struct lcrt_user *luser);
int lcrt_user_load_config(struct lcrt_user *luser);
int lcrt_user_save_config(struct lcrt_user *luser);
int lcrt_user_add(struct lcrt_user *luser, struct lcrtc_user *user);

void lcrt_user_del(struct lcrtc_user *user);
int lcrtc_user_ref(struct lcrtc_user *user);
int lcrtc_user_unref(struct lcrtc_user *user);
int lcrtc_user_is_dirty(struct lcrtc_user *user);
int lcrtc_user_refresh(struct lcrtc_user *user);
lcrt_protocol_t lcrt_user_get_protocol(const char *proto);
struct lcrtc_user *lcrtc_user_create(void);
void lcrtc_user_destroy(struct lcrtc_user *user);

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
    int is_folder);
void lcrtc_user_dump(struct lcrtc_user *user, const char *func_name);
int lcrt_user_save_one(struct lcrt_user *luser, struct lcrtc_user *user);
int lcrt_user_destroy_config(struct lcrt_user *luser);
struct lcrtc_user *lcrt_user_find_by_name(struct lcrt_user *luser, const char *name);
int lcrt_user_del_one(struct lcrt_user *luser, struct lcrtc_user *user, int del_from_db);
int lcrt_user_del_folder(struct lcrt_user *luser, struct lcrtc_user *user, int del_from_db);
int lcrt_user_rename(struct lcrt_user *luser, struct lcrtc_user *user, char *new_name, int update_to_db);
int lcrt_exec_check(lcrt_protocol_t prot);

struct lcrt_window;
int lcrt_user_find_unused_label(struct lcrt_window *lwindow, 
    const char *keyword /* in */, char *label /* out */);
#endif
