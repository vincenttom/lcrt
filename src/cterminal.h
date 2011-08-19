/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sat 15 Jan 2011 05:26:59 PM CST
 * File Name:    cterminal.h
 *
 * Description:  
 */

#ifndef _LCRT_CTERMINAL_H
#define _LCRT_CTERMINAL_H

#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <vte/vte.h>
void lcrt_terminal_on_commit(VteTerminal *widget, gchar *text, guint length, gpointer user_data);
void lcrt_terminal_on_child_exited(VteTerminal *vteterminal, gpointer user_data);
void lcrt_terminal_on_contents_changed(VteTerminal *vteterminal, gpointer user_data);
void lcrt_terminal_on_selection_changed(VteTerminal *vteterminal, gpointer user_data);
gboolean lcrt_terminal_on_label_title_button_press_event(GtkWidget*widget,
            GdkEventButton  *event, gpointer user_data);
gboolean lcrt_terminal_on_button_press_event(GtkWidget*widget,
            GdkEventButton  *event, gpointer user_data);
void lcrt_terminal_on_button_close_clicked(GtkButton *button, gpointer user_data);
gboolean lcrt_terminal_has_selection(struct lcrt_window *lwindow);
void lcrt_terminal_set_connected_status(struct lcrt_terminal *lterminal);
#endif
