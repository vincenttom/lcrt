/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Mon 17 Jan 2011 10:18:51 PM CST
 * File Name:    ctoolbar.h
 *
 * Description:  
 */

#ifndef __LCRT_CTOOLBAR_H__
#define __LCRT_CTOOLBAR_H__
#include <gtk/gtk.h>
void lcrt_toolbar_on_connect_activate(GtkWidget *toolitem, gpointer user_data);
void lcrt_toolbar_on_quick_connect_activate(GtkWidget *toolitem, gpointer user_data);
void lcrt_toolbar_on_connect_in_tab_activate(GtkWidget *toolitem, gpointer user_data);
void lcrt_toolbar_on_reconnect_activate(GtkWidget *toolitem, gpointer user_data);
void lcrt_toolbar_on_disconnect_activate(GtkWidget *toolitem, gpointer user_data);
void lcrt_toolbar_on_copy_activate(GtkWidget *toolitem, gpointer user_data);
void lcrt_toolbar_on_paste_activate(GtkWidget *toolitem, gpointer user_data);
void lcrt_toolbar_on_terminal_activate(GtkWidget *toolitem, gpointer user_data);
#endif
