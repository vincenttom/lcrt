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

typedef enum {
    LCRT_PROTOCOL_SSH2,
    LCRT_PROTOCOL_SSH1,
    LCRT_PROTOCOL_TELNET,
    LCRT_PROTOCOL_TELNET_SSL,
    LCRT_PROTOCOL_RLOGIN,
    LCRT_PROTOCOL_SERIAL,
    LCRT_PROTOCOL_SHELL,
    LCRT_PROTOCOL_NUMBER,
    LCRT_PROTOCOL_UNKNOWN
}lcrt_protocol_t;

/** default port of protocol */
#define LCRT_PROTOCOL_PORT \
    "22", \
    "22", \
    "23", /**<can't change*/ \
    "992", /**<can't change*/ \
    "0", \
    "0", \
    "0"

#define LCRT_PROTOCOL_NAME \
    "SSH2", \
    "SSH1", \
    "Telnet", \
    "Telnet/SSL", \
    "Rlogin", \
    "Serial", \
    "Shell"

/* 
 * When a terminal receive data from remote, it will call this functions
 * to handle.
 * NOTE: The order of this functions must be same wtih lcrt_protocol_t 
 */
#define LCRT_PROTOCOL_CONTENTS_CHANGED_CALLBACKS \
    lcrt_terminal_ssh_contents_changed, \
    lcrt_terminal_ssh_contents_changed, \
    lcrt_terminal_telnet_contents_changed, \
    lcrt_terminal_telnet_contents_changed, \
    lcrt_terminal_rlogin_contents_changed, \
    lcrt_terminal_serial_contents_changed, \
    lcrt_terminal_shell_contents_changed
/*
 * When we open the Quick connect dialog, we should show different
 * options for each protocol, so we use these callbacks to create 
 * sub box in quick connect dialog.
 */
#define LCRT_PROTOCOL_CREATE_SUBBOX_CALLBACKS \
    lcrt_qconnect_create_ssh_subbox, \
    lcrt_qconnect_create_ssh_subbox, \
    lcrt_qconnect_create_telnet_subbox, \
    lcrt_qconnect_create_telnet_subbox, \
    lcrt_qconnect_create_rlogin_subbox, \
    lcrt_qconnect_create_serial_subbox, \
    lcrt_qconnect_create_shell_subbox
/*
 * When you click the connect button in quick connect dialog, we
 * should create a terminal to connect with remote,each protocol
 * has different method.
 */
#define LCRT_PROTOCOL_ON_BUTTON_CONNECT_CLICKED_CALLBACKS \
    lcrt_qconnect_ssh_on_button_connect_clicked, \
    lcrt_qconnect_ssh_on_button_connect_clicked, \
    lcrt_qconnect_telnet_on_button_connect_clicked, \
    lcrt_qconnect_telnet_on_button_connect_clicked, \
    lcrt_qconnect_rlogin_on_button_connect_clicked, \
    lcrt_qconnect_serial_on_button_connect_clicked, \
    lcrt_qconnect_shell_on_button_connect_clicked

enum {
    LCRT_FIREWALL_NONE,
    LCRT_FIREWALL_NUMBER
};
#define LCRT_FIREWALL_NAME \
    "none"
enum {
    LCRT_DEP_SSH,
    LCRT_DEP_TELNET,
    LCRT_DEP_RLOGIN,
    LCRT_DEP_SHELL,
    LCRT_DEP_NUMBER
};
#define LCRT_DEP_PROG \
    "ssh", \
    "telnet", \
    "rlogin", \
    "bash"
#define HOSTNAME_LEN 128
#define USERNAME_LEN 64
#define PASSWORD_LEN 64
#define DEFAULT_COMMAND_LEN 512
enum {
    LCRT_USER_NAME,
    LCRT_USER_HOSTNAME,
    LCRT_USER_PROTOCOL,
    LCRT_USER_USERNAME,
    LCRT_USER_PASSWORD,
    LCRT_USER_PORT,
    LCRT_USER_DEFAULT_COMMAND,
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
    int dirty);
void lcrtc_user_dump(struct lcrtc_user *user, const char *func_name);
int lcrt_user_save_one(struct lcrt_user *luser, struct lcrtc_user *user);
int lcrt_user_destroy_config(struct lcrt_user *luser);
struct lcrtc_user *lcrt_user_find_by_name(struct lcrt_user *luser, const char *name);
int lcrt_user_del_one(struct lcrt_user *luser, struct lcrtc_user *user);
int lcrt_user_rename(struct lcrt_user *luser, struct lcrtc_user *user, char *new_name);
int lcrt_exec_check(lcrt_protocol_t prot);
#endif
