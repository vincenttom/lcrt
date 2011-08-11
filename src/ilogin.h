/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sun 23 Jan 2011 02:36:35 AM CST
 *
 *
 * Description:  
 */

#ifndef __LCRT_ILOGIN_H__
#define __LCRT_ILOGIN_H__

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"

#define LCRT_ILOGIN_TABLE "ilogin"

enum {
    LCRT_L_LOGIN,
    LCRT_L_INFO,
    LCRT_L_AGAIN,
    LCRT_L_USERNAME,
    LCRT_L_PASSWORD,
    LCRT_L_REMEMBER,
    LCRT_L_OKBUTTON,
    LCRT_L_CANCELBUTTON,
    LCRT_L_ERR_MSG,
    LCRT_L_NUMBER
};
#define LCRT_L_NAME \
    "l_login", \
    "l_info", \
    "l_again", \
    "l_username", \
    "l_password", \
    "l_remember", \
    "l_okbutton", \
    "l_cancelbutton", \
    "l_err_msg"

#define LCRT_L_VALUE \
    "Login", \
    "Input username & password:",\
    "Username or password error:", \
    "Username:", \
    "Password:", \
    "Remember ?", \
    "OK", \
    "Cancel", \
    "There is no memory to create dialog"

#define LCRT_L_SHORTCUT \
    {0, 0}

struct lcrtc_login {
    struct lcrt_config db;
    const char *name[LCRT_L_NUMBER];
    char value[LCRT_L_NUMBER][LCRT_CONFIG_VALUE_LEN];
    int   shortcut[LCRT_L_NUMBER][2];
};

struct lcrt_login {
    struct lcrt_terminal *parent;
    GtkWidget *login;
    GtkWidget *l_username;
    GtkWidget *l_password;
    GtkWidget *l_remember;

    struct lcrtc_login config;
    const char *(*get_db)(struct lcrt_login *llogin);
    const char *(*get_tb)(struct lcrt_login *llogin);
};

struct lcrt_login *lcrt_create_login(struct lcrt_terminal *parent, int again);
void lcrt_destroy_login(struct lcrt_login *llogin);

#endif
