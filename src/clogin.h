/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sun 23 Jan 2011 02:36:41 AM CST
 *
 *
 * Description:  
 */

#ifndef __LCRT_CLOGIN_H__
#define __LCRT_CLOGIN_H__

#include <gtk/gtk.h>

void lcrt_login_on_okbutton_clicked(GtkButton *button, gpointer user_data);
void lcrt_login_on_username_changed(GtkEditable *editable, gpointer user_data);
void lcrt_login_on_password_changed(GtkEditable *editable, gpointer user_data);
void lcrt_login_on_cancelbutton_clicked (GtkButton *button, gpointer user_data);
gboolean lcrt_login_on_window_delete(GtkWidget *widget, GdkEvent  *event, gpointer user_data);
#endif
