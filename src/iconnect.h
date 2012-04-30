/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sat 22 Jan 2011 04:18:02 PM CST
 *
 *
 * Description:  
 */

#ifndef __LCRT_ICONNECT_H__
#define __LCRT_ICONNECT_H__

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "iwindow.h"

#define LCRT_ICONNECT_TABLE "iconnect"

enum {
    LCRT_C_CONNECT_IN_TAB,
    LCRT_C_CONNECT,
    LCRT_C_QUICK_CONNECT,
    LCRT_C_RENAME,
    LCRT_C_DELETE,
    LCRT_C_FIND,
    LCRT_C_SESSION_OPTION,
    LCRT_C_MKDIR,
    LCRT_C_SESSION,
    LCRT_C_SHOW_DIALOG_ON_STARTUP,
    LCRT_C_CONNECT_BUTTON,
    LCRT_C_CLOSE_BUTTON,
    LCRT_C_WARN_MSG,
    LCRT_C_ERR_MSG,
    LCRT_C_NUMBER
};
#define LCRT_C_NAME \
    "c_connect_in_tab", \
    "c_connect", \
    "c_quick_connect", \
    "c_rename", \
    "c_delete", \
    "c_find", \
    "c_session_option", \
    "c_mkdir", \
    "c_session", \
    "c_show_dialog_on_startup", \
    "c_connect_button", \
    "c_close_button", \
    "c_warn_msg", \
    "c_err_msg"

#define LCRT_C_VALUE \
    "Connect in Tab", \
    "Connect", \
    "Quick Connect", \
    "Rename a session", \
    "Delete a session", \
    "Find a session", \
    "Session option", \
    "Create new folder", \
    "Sessions", \
    "Show dialog on startup", \
    "Connect", \
    "Close", \
    "Are you sure you want to delete the selected item ?", \
    "There is no memory to create window"
#define LCRT_C_ICONS \
    "", \
    "lcrt-connect.png", \
    "lcrt-quick-connect.png", \
    "lcrt-rename.png", \
    "lcrt-delete.png", \
    "lcrt-find.png", \
    "lcrt-settings.png", \
    "lcrt-mkdir.png",

#define LCRT_C_CALLBACK \
    NULL, \
    NULL, \
    lcrt_connect_on_quick_connect_activate, \
    lcrt_connect_on_rename_activate, \
    lcrt_connect_on_delete_activate, \
    lcrt_connect_on_find_activate, \
    lcrt_connect_on_session_option_activate, \
    lcrt_connect_on_mkdir_activate, \

#define LCRT_C_SHORTCUT \
    {0, 0}

struct lcrtc_connect {
    struct lcrt_config db;
    const char *name[LCRT_C_NUMBER];
    char value[LCRT_C_NUMBER][LCRT_CONFIG_VALUE_LEN];
    int   shortcut[LCRT_C_NUMBER][2];
};
struct lcrt_connect {
    struct lcrt_window *parent;

    GtkWidget *c_connect;
    GtkWidget *c_toolbar;
    GtkWidget *c_checkbutton;
    GtkWidget *c_connectbutton;
    GtkWidget *toolitem[LCRT_C_NUMBER];
    gboolean connect_button_clicked;
    GtkTreeIter c_treeiter;
    GtkWidget *c_treeview;
    gboolean tab;

    struct lcrtc_connect config;
    const char *(*get_db)(struct lcrt_connect *lconnect);
    const char *(*get_tb)(struct lcrt_connect *lconnect);
};

int lcrt_create_connect(struct lcrt_window *parent, gboolean tab);
void lcrt_destroy_connect(struct lcrt_connect *lconnect);
int lcrt_connect_del_user(struct lcrt_connect *lconnect, const char *user_name);
int lcrt_connect_find_folder(struct lcrt_connect *lconnect, 
        GtkTreeIter *folder, const char *folder_name);

#endif
