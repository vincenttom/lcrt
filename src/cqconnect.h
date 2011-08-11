/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sat 22 Jan 2011 06:34:06 PM CST
 *
 *
 * Description:  
 */

#ifndef __LCRT_CQCONNECT_H__
#define __LCRT_CQCONNECT_H__
#include <gtk/gtk.h>

void lcrt_qconnect_on_button_connect_clicked(GtkButton *button, gpointer user_data);
void lcrt_qconnect_on_button_cancel_clicked (GtkButton *button, gpointer user_data);
void lcrt_qconnect_on_window_destroy(GtkObject *object, gpointer user_data);
void lcrt_qconnect_on_entry_hostname_changed(GtkWidget *widget, gpointer user_data);
void lcrt_qconnect_on_protocol_changed(GtkComboBox *widget, gpointer user_data);
gboolean lcrt_qconnect_on_window_delete(GtkWidget *widget, GdkEvent  *event, gpointer user_data);

#endif
