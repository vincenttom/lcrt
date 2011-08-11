/*
 * Copyright(c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sat 15 Jan 2011 04:01:41 PM CST
 * File Name:    iCreateWindow.c
 *
 * Description:  
 */
//#define __LCRT_DEBUG__
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <vte/vte.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <assert.h>
#include <errno.h>
#include "debug.h"
#include "support.h"
#include "user.h"
#include "message.h"
#include "iwindow.h"
#include "iterminal.h"
#include "cterminal.h"
#include "inotebook.h"
#include "isettings.h"
#include "itoolbar.h"
#include "cstatusbar.h"
#include "ssh.h"
#include "telnet.h"
#include "rlogin.h"
#include "serial.h"
#include "shell.h"
#include "protocol.h"

int lcrt_create_terminal(struct lcrt_notebook *parent)
{
    GtkWidget *scrolledwindow;
    GtkWidget *terminal;
    GtkWidget *label_title;
    GtkWidget *hbox_title;
    GtkWidget *eventbox;
    GtkWidget *button_close;
    GtkWidget *button_image;

    //GtkAccelGroup *accel_group = parent->parent->w_accel;
    struct lcrt_settings *lsettings = parent->parent->w_settings;
    struct lcrt_protocol_callback *callback;

    struct lcrt_terminal *lterminal;
    char *err;

    lterminal = (struct lcrt_terminal *)calloc(1, sizeof(struct lcrt_terminal));
    if (lterminal == NULL) {
        err = "Create terminal error";
        goto out;
    
    }
    debug_print("terminal_addr = %p\n", lterminal);
    lterminal->parent = parent;
    lterminal->connected = FALSE;
    lterminal->signal_connected = FALSE;
    lterminal->user = parent->parent->current_user;
    list_add(&lterminal->brother, &parent->child);

    terminal = vte_terminal_new();
    lterminal->terminal = VTE_TERMINAL(terminal);
    debug_print("create VTETerminal = %p\n", lterminal->terminal);
    callback = lcrt_protocol_get_callback(lterminal->user->protocol);
    lterminal->contents_changed = callback->contents_changed;
    lterminal->connect_remote = callback->connect_remote;

    scrolledwindow = gtk_scrolled_window_new(NULL, vte_terminal_get_adjustment(VTE_TERMINAL(terminal)));
    lterminal->scrolledwindow = scrolledwindow;
    gtk_widget_show(scrolledwindow);
    gtk_container_add(GTK_CONTAINER(lterminal->parent->notebook), scrolledwindow);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), 
                                   GTK_POLICY_NEVER, 
                                   GTK_POLICY_ALWAYS);
    debug_print("create scrolledwindow = %p\n", lterminal->scrolledwindow);

    gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK (parent->notebook), scrolledwindow, TRUE);
    gtk_notebook_set_tab_detachable(GTK_NOTEBOOK (parent->notebook), scrolledwindow, TRUE);

    hbox_title = gtk_hbox_new (FALSE, 2);
    GTK_WIDGET_UNSET_FLAGS (hbox_title, GTK_CAN_FOCUS);
    gtk_widget_show (hbox_title);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (parent->notebook), scrolledwindow, hbox_title);

    eventbox = gtk_event_box_new (); 
    gtk_widget_show (eventbox);
    GTK_WIDGET_UNSET_FLAGS (eventbox, GTK_CAN_FOCUS);
    gtk_box_pack_start (GTK_BOX (hbox_title), eventbox, FALSE, FALSE, 0);
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(eventbox), FALSE);

    label_title = gtk_label_new (NULL);
    lterminal->label = label_title;
    gtk_widget_set_size_request (label_title, -1, 15);

    lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_DISCONNECT);
    gtk_widget_show (label_title);
    GTK_WIDGET_UNSET_FLAGS (label_title, GTK_CAN_FOCUS);
    gtk_container_add(GTK_CONTAINER(eventbox), label_title);
    g_signal_connect ((gpointer) eventbox, "button_press_event",
                      G_CALLBACK (lcrt_terminal_on_label_title_button_press_event),
                      lterminal);

    button_close = gtk_toggle_button_new ();
    gtk_widget_show (button_close);
    gtk_box_pack_start (GTK_BOX (hbox_title), button_close, FALSE, FALSE, 0);
    gtk_widget_set_size_request (button_close, 20, 20);
    GTK_WIDGET_UNSET_FLAGS (button_close, GTK_CAN_FOCUS);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button_close), TRUE);
    gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (button_close), TRUE);
    gtk_button_set_relief( GTK_BUTTON(button_close), GTK_RELIEF_NONE ) ;
    button_image = create_pixmap(NULL, "lcrt-delete.png");
    gtk_widget_set_size_request (button_image, 20, 20);
    gtk_widget_show (button_image);
    gtk_container_add (GTK_CONTAINER (button_close), button_image);

    g_signal_connect ((gpointer) button_close, "clicked",
                       G_CALLBACK (lcrt_terminal_on_button_close_clicked),
                       lterminal);  

    //vte_terminal_set_backspace_binding(VTE_TERMINAL(terminal), VTE_ERASE_ASCII_BACKSPACE);

    gtk_widget_show(terminal);
    GTK_WIDGET_SET_FLAGS(terminal, GTK_CAN_FOCUS);
    gtk_container_add(GTK_CONTAINER(scrolledwindow), terminal);
    //lcrt_terminal_fork(lterminal);
    if (lterminal->connect_remote)
        lterminal->connect_remote(lterminal);
    debug_print("terminal = %p, font = %s\n", lterminal->terminal, lsettings->lt_t_font);
    vte_terminal_set_font_from_string(lterminal->terminal, lsettings->lt_t_font);
    debug_where();
    vte_terminal_set_color_foreground(VTE_TERMINAL(lterminal->terminal), &lsettings->lt_t_fcolor);
    vte_terminal_set_color_background(VTE_TERMINAL(lterminal->terminal), &lsettings->lt_t_bcolor);
    if (lsettings->lt_t_transparent != 0)
        vte_terminal_set_background_transparent(VTE_TERMINAL(lterminal->terminal), TRUE);
    vte_terminal_set_background_saturation(VTE_TERMINAL(lterminal->terminal), (double)lsettings->lt_t_transparent / 100.0);
    vte_terminal_set_scrollback_lines(VTE_TERMINAL(lterminal->terminal), lsettings->lt_t_scrolllines);
    vte_terminal_set_background_image_file(VTE_TERMINAL(lterminal->terminal), lsettings->lt_t_backimage);
    vte_terminal_set_audible_bell(VTE_TERMINAL(lterminal->terminal), lsettings->lt_t_bell);


    g_signal_connect_after(GTK_OBJECT(terminal), 
                     "commit", 
                     G_CALLBACK(lcrt_terminal_on_commit), 
                     lterminal);
    g_signal_connect(GTK_OBJECT(terminal), 
                     "child-exited", 
                     G_CALLBACK(lcrt_terminal_on_child_exited), 
                     lterminal);
    g_signal_connect(GTK_OBJECT(terminal), 
                     "contents-changed", 
                     G_CALLBACK(lcrt_terminal_on_contents_changed), 
                     lterminal);
    g_signal_connect(GTK_OBJECT(terminal), 
                     "selection-changed", 
                     G_CALLBACK(lcrt_terminal_on_selection_changed), 
                     lterminal);

    g_signal_connect ((gpointer) terminal, "button_press_event",
                      G_CALLBACK (lcrt_terminal_on_button_press_event),
                      lterminal);

    gtk_notebook_set_current_page(GTK_NOTEBOOK(parent->notebook),
            gtk_notebook_page_num(GTK_NOTEBOOK(parent->notebook), scrolledwindow));
    return 0;
out:
    lcrt_message_warning(parent->parent->window, err);
    return -errno;
}
int lcrt_terminal_set_status(struct lcrt_terminal *lterminal, char *label_name, lcrt_terminal_status_t status)
{
    struct lcrt_toolbar *ltoolbar = lterminal->parent->parent->w_toolbar;
    char buf[256];
    gboolean sensitive;
    char *title = (label_name == NULL) ? lterminal->user->name : label_name;
    if (lterminal == NULL)
        return EINVAL;
    switch (status) {
    case LCRT_TERMINAL_DISCONNECT:
    case LCRT_TERMINAL_CHILD_EXIT:
        sprintf(buf, "<span foreground=\"#CD0000\">%s</span>", title);
        sensitive = TRUE;
        break;
    case LCRT_TERMINAL_CONNECTED:
        sprintf(buf, "<span foreground=\"#009900\">%s</span>", title);
        sensitive = FALSE;
        break;
    default:
        sprintf(buf, "%s", lterminal->user->name);
        sensitive = TRUE;
        break;
    }
    gtk_label_set_markup(GTK_LABEL(lterminal->label), buf);
    lterminal->connected = status;
    if (lterminal != lterminal->parent->current_terminal)
        goto out;
    gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_RECONNECT], sensitive);
    gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_DISCONNECT], !sensitive);
    gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_COPY], FALSE);
    gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_PASTE], !sensitive);
out:
    return LCRTE_OK;
}
int lcrt_terminal_fork(struct lcrt_terminal *lterminal)
{
    lcrt_protocol_t protocol;
    struct lcrtc_user *user;
    char *argv[5], *work_dir;
    char hostname[256], port[32];
    char *dep_prog[] = {LCRT_DEP_PROG};
    int dep = -1;
    char tmp[32];

    if (lterminal == NULL)
       return EINVAL;

    user = lterminal->user;
    protocol = user->protocol;
    switch (protocol) {
    case LCRT_PROTOCOL_SSH1:
    case LCRT_PROTOCOL_SSH2:
        argv[0] = dep_prog[LCRT_DEP_SSH];
        argv[1] = protocol == LCRT_PROTOCOL_SSH2 ? "-2" : "-1";
        if (strlen(user->username) == 0) {
            strcpy(hostname, user->hostname);
        } else {
            sprintf(hostname, "%s@%s", user->username, user->hostname);
        }
        sprintf(port, "-p %d", user->port);
        argv[1] = hostname;
        argv[2] = port;
        argv[3] = NULL;
        work_dir = ".";
        dep = LCRT_DEP_SSH;
        break;
    case LCRT_PROTOCOL_TELNET_SSL:
    case LCRT_PROTOCOL_TELNET:
    case LCRT_PROTOCOL_RLOGIN:
        argv[0] = dep_prog[LCRT_DEP_TELNET];
        strcpy(hostname, user->hostname);
        argv[1] = hostname;

        sprintf(port, "%d", user->port);
        argv[2] = port;
        argv[3] = NULL;
        work_dir = ".";
        dep = LCRT_DEP_TELNET;
        break;
    case LCRT_PROTOCOL_SHELL:
        argv[0] = dep_prog[LCRT_DEP_SHELL];
        sprintf(tmp, "/bin/%s", argv[0]);
        if (access(tmp, F_OK | X_OK) != 0)
            argv[0] = "sh";
        argv[1] = NULL;
        work_dir = getenv("HOME");
        dep = LCRT_DEP_SHELL;
        break;
    default:
        lcrt_message_info(lterminal->parent->parent->window, 
                           lterminal->parent->config.value[LCRT_TM_CONNECTION_NOT_SUPPORT]);
        return LCRTE_NOT_SUPPORT;
    }
    if (lcrt_exec_check(protocol) != 0) {
        lcrt_message_info(lterminal->parent->parent->window, 
                          lterminal->parent->config.value[LCRT_TM_CONNECTION_PROG_NOT_FOUND],
                          dep_prog[dep]);
        return LCRTE_NOT_FOUND;
    }

    lterminal->child_pid  = vte_terminal_fork_command(VTE_TERMINAL(lterminal->terminal), 
                argv[0], argv, NULL , work_dir, FALSE, FALSE, FALSE);
    debug_print("child_pid = %d\n", lterminal->child_pid);
    lcrt_statusbar_set_user(lterminal->parent->parent->w_statusbar, lterminal->user);
    debug_where();
    return LCRTE_OK;
}
void lcrt_destroy_terminal(struct lcrt_terminal *lterminal)
{
    if (lterminal == NULL)
        return;
    debug_where();
    if (lterminal->connected) {
        vte_terminal_feed_child(lterminal->terminal, 
                LCRT_TERMINAL_EXIT_CMD, strlen(LCRT_TERMINAL_EXIT_CMD));
    }

    debug_where();
    debug_print("scrolledwindow = %p\n", lterminal->scrolledwindow);
    gtk_widget_destroy(lterminal->scrolledwindow);
    debug_where();

    if (lterminal->child_pid > 0) {
        kill(lterminal->child_pid, SIGKILL);
        lterminal->child_pid = 0;
    }
    debug_print("terminal_del = %p\n", lterminal);
    list_del(&lterminal->brother);
    debug_where();
    free(lterminal);
}

