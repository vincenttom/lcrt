/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sat 15 Jan 2011 05:06:01 PM CST
 * File Name:    cterminal.c
 *
 * Description:  
 */
//#define __LCRT_DEBUG__
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <vte/vte.h>
#include "user.h"
#include "iterminal.h"
#include "debug.h"
#include "ilogin.h"
#include "message.h"
#include "iwindow.h"
#include "inotebook.h"
#include "imenubar.h"
#include "ifile.h"
#include "iedit.h"
#include "cedit.h"
#include "ipopup.h"
#include "istatus.h"
#include "itoolbar.h"

#define RESET
void lcrt_terminal_on_commit(VteTerminal *widget, gchar *text, guint length, gpointer user_data)
{
    //struct lcrt_terminal *lterminal = (struct lcrt_terminal *)user_data;
}
    
gboolean lcrt_terminal_on_key_press_event(GtkWidget*widget,
            GdkEventKey *event, gpointer user_data)
{
    struct lcrt_terminal *lterminal = (struct lcrt_terminal *)user_data;
    debug_where();
    if (event->type == GDK_KEY_PRESS && event->keyval == GDK_Return) {
        debug_print("reconnect...\n");
        lcrt_terminal_fork(lterminal);
    }
    return FALSE;
}
void lcrt_terminal_on_child_exited(VteTerminal *vteterminal, gpointer user_data)
{
    debug_where();
    struct lcrt_terminal *lterminal = (struct lcrt_terminal *)user_data;
    debug_where();

    if (lterminal->connected != LCRT_TERMINAL_CONNECTED)
        return;
    lterminal->connected = FALSE;
    lterminal->child_pid = 0;
    debug_where();
#ifdef RESET
    vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
#endif
    debug_where();
    lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_CHILD_EXIT);
    g_signal_connect ((gpointer) lterminal->terminal, "key-press-event",
                       G_CALLBACK (lcrt_terminal_on_key_press_event),
                       lterminal);
    debug_where();
    gtk_window_set_focus(GTK_WINDOW(lterminal->parent->parent->window), 
                     GTK_WIDGET(lterminal->terminal));
    debug_where();
    lterminal->signal_connected = TRUE;
    debug_print("child exit\n");
}
int lcrt_check_space_string(const char *text)
{
    if (text == NULL)
        return TRUE;
    const char *p = text;
    while ((*p == ' ' || *p == '\n') && *p != '\0')
        p++;

    if (*p != '\0')
        return FALSE;
    else
        return TRUE;
}

void lcrt_terminal_on_emulation_changed(VteTerminal *vteterminal, gpointer user_data)
{
    debug_print("emulation_changed\n");
    debug_where();
}
void lcrt_terminal_set_connected_status(struct lcrt_terminal *lterminal)
{
    struct lcrt_window *lwindow = lterminal->parent->parent;
    debug_print("Login success\n");
    lterminal->connected =TRUE;
    debug_where();
    lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_CONNECTED);
    if (lterminal->signal_connected) {
        gtk_signal_disconnect_by_func(GTK_OBJECT(lterminal->terminal),
                                      G_CALLBACK(lcrt_terminal_on_key_press_event),
                                      lterminal);
        lterminal->signal_connected = FALSE;
    }
    gtk_window_set_focus(GTK_WINDOW(lwindow->window),
             GTK_WIDGET(lterminal->terminal));
    if (lterminal->save_passwd) {
        lcrt_user_save_one(&lwindow->u_config, lwindow->current_user);
    }
    if (strlen(lterminal->user->command) != 0) {
        vte_terminal_feed_child(lterminal->terminal, lterminal->user->command, strlen(lterminal->user->command));
        vte_terminal_feed_child(lterminal->terminal, "\n", 1);
    }
}
void lcrt_terminal_on_contents_changed(VteTerminal *vteterminal, gpointer user_data)
{
    debug_where();
    struct lcrt_terminal *lterminal = (struct lcrt_terminal *)user_data;
    struct lcrtc_user *user = lterminal->user;
    lcrt_protocol_t protocol = user->protocol;

    if (lterminal->connected == TRUE)
        return;
    lcrtc_user_dump(user, __func__);
    if (lterminal->contents_changed) 
        lterminal->contents_changed(lterminal);

}
gboolean lcrt_terminal_on_label_title_button_press_event(GtkWidget *widget,
            GdkEventButton  *event, gpointer user_data)
{
    struct lcrt_terminal *lterminal = (struct lcrt_terminal *)user_data;
    debug_where();
    if (event->type == GDK_BUTTON_PRESS && event->button == BUTTON_RIGHT) {
        struct lcrt_popup *lpopup = lcrt_create_popup_menu(lterminal);
		gtk_menu_popup (GTK_MENU(lpopup->popup_menu),
			     NULL, NULL, NULL, NULL,
			     event->button, event->time);
        debug_print("Create popup menu\n");
    }
    return FALSE;
}
gboolean lcrt_terminal_on_button_press_event(GtkWidget *widget,
            GdkEventButton  *event, gpointer user_data)
{
    struct lcrt_terminal *lterminal = (struct lcrt_terminal *)user_data;
    debug_where();
    if (event->type == GDK_BUTTON_PRESS && event->button == BUTTON_RIGHT && 
        lterminal->connected == LCRT_TERMINAL_CONNECTED) {
        struct lcrt_edit *ledit = lcrt_edit_create_menuitem(lterminal->parent->parent->w_menubar, TRUE);
        g_signal_connect ((gpointer) ledit->e_menuitem, "deactivate",
                    G_CALLBACK(lcrt_edit_on_popup_menu_deactivate),
                    ledit);
		gtk_menu_popup (GTK_MENU(ledit->e_menuitem),
			     NULL, NULL, NULL, NULL,
			     event->button, event->time);

        debug_print("Create popup menu for terminal = %p\n", ledit);

    }
    return FALSE;
}

void lcrt_terminal_on_selection_changed(VteTerminal *vteterminal, gpointer user_data)
{
    struct lcrt_terminal *lterminal = (struct lcrt_terminal *)user_data;
    struct lcrt_toolbar *ltoolbar = lterminal->parent->parent->w_toolbar;
    gboolean selected = vte_terminal_get_has_selection(lterminal->terminal);
    
    gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_COPY], selected);
    gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_PASTE], lterminal->connected == LCRT_TERMINAL_CONNECTED);
}
void lcrt_terminal_on_button_close_clicked(GtkButton *button, gpointer user_data)
{
    debug_where();
    struct lcrt_terminal *lterminal = (struct lcrt_terminal *)user_data;
    int rv = TRUE;

    if (lterminal->user->protocol != LCRT_PROTOCOL_SHELL && lterminal->connected == LCRT_TERMINAL_CONNECTED) {
        rv = lcrt_message_choose(lterminal->parent->parent->window,
                             GTK_MESSAGE_WARNING,
                             lterminal->parent->parent->w_status->config.value[LCRT_S_SESSION_DISCONNECT],
                             lterminal->user->hostname);
    }
    if (rv == TRUE)
        lcrt_destroy_terminal(lterminal);
}

gboolean lcrt_terminal_has_selection(struct lcrt_window *lwindow)
{
    if (lwindow == NULL || lwindow->w_notebook == NULL ||
        lwindow->w_notebook->current_terminal == NULL)
        return FALSE;
    return vte_terminal_get_has_selection(lwindow->w_notebook->current_terminal->terminal);
}
