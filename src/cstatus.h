/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Mon 24 Jan 2011 09:57:22 PM CST
 *
 *
 * Description:  
 */

#ifndef __LCRT_CSTATUS_H__
#define __LCRT_CSTATUS_H__
#include <gtk/gtk.h>

void lcrt_status_on_show_window_active(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_status_on_activate(GtkStatusIcon *status_icon, gpointer data);
gboolean lcrt_status_on_button_press_event(GtkStatusIcon  *status_icon,
            GdkEventButton *event,
            gpointer user_data);
void lcrt_status_on_quit_activate(GtkMenuItem *menuitem, gpointer user_data);

#endif
