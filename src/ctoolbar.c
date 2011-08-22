/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Mon 17 Jan 2011 10:17:47 PM CST
 * File Name:    ctoolbar.c
 *
 * Description:  
 */

#include <stdio.h>
#include "itoolbar.h"
#include "iwindow.h"
#include "cwindow.h"
#include "inotebook.h"
#include "iterminal.h"
#include "iqconnect.h"
#include "iconnect.h"
#include "cfile.h"
#include "user.h"
#include "itoolbar.h"
#include "cedit.h"

void lcrt_toolbar_on_connect_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_toolbar *ltoolbar = (struct lcrt_toolbar *)user_data;

    lcrt_file_on_connect_activate(NULL, ltoolbar->parent->w_menubar->m_file);
}


void lcrt_toolbar_on_quick_connect_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_toolbar *ltoolbar = (struct lcrt_toolbar *)user_data;

    lcrt_create_qconnect(ltoolbar->parent, NULL, LCRT_QCONNECT_IN_TAB, NULL);
}


void lcrt_toolbar_on_connect_in_tab_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_toolbar *ltoolbar = (struct lcrt_toolbar *)user_data;

    lcrt_create_connect(ltoolbar->parent, TRUE);
}
void lcrt_toolbar_on_reconnect_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_toolbar *ltoolbar = (struct lcrt_toolbar *)user_data;
    lcrt_file_on_reconnect_activate(NULL, ltoolbar->parent->w_menubar->m_file);
}
void lcrt_toolbar_on_disconnect_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_toolbar *ltoolbar = (struct lcrt_toolbar *)user_data;
    lcrt_file_on_disconnect_activate(NULL, ltoolbar->parent->w_menubar->m_file);
}

void lcrt_toolbar_on_copy_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_toolbar *ltoolbar = (struct lcrt_toolbar *)user_data;
    lcrt_edit_on_copy_activate(NULL, ltoolbar->parent);
}

void lcrt_toolbar_on_paste_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_toolbar *ltoolbar = (struct lcrt_toolbar *)user_data;
    lcrt_edit_on_paste_activate(NULL, ltoolbar->parent);
}

void lcrt_toolbar_on_terminal_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_toolbar *ltoolbar = (struct lcrt_toolbar *)user_data;
    struct lcrt_window *lwindow = ltoolbar->parent;
    struct lcrtc_user *user;
    const char *name = "shell";
    
    if ((user = lcrt_user_find_by_name(&lwindow->u_config, "shell")) == NULL) {
        user = lcrtc_user_create();
        if (user == NULL)
            return;
        lcrtc_user_set_data(
               user,
               name,
               name,
               LCRT_PROTOCOL_SHELL,
               NULL,
               "/bin/bash",
               NULL,
               0,
               TRUE);
        lcrtc_user_ref(user);
        lcrt_user_add(&lwindow->u_config, user);
    }

    lcrt_window_set_current_user(lwindow, user);
    lcrt_create_terminal(lwindow->w_notebook);
    if (lcrt_window_get_auto_save(lwindow)) {
        lcrt_user_save_one(&lwindow->u_config, user);
    }
}


