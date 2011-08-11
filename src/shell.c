/*
 * Copyright (c) 2011 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Sun 07 Aug 2011 05:54:28 PM CST
 * File Name   : rlogin.c
 *
 * Description:  
 */

#include <stdio.h>
#include "iterminal.h"
#include "iqconnect.h"
#include "debug.h"
#include "user.h"
#include "shell.h"
#include "serial.h"

static void lcrt_shell_contents_changed(struct lcrt_terminal *lterminal)
{
    lcrt_terminal_set_connected_status(lterminal);
    debug_print("reset = %f\n",
        gtk_adjustment_get_upper(
           gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(lterminal->scrolledwindow))));
}

static int lcrt_shell_connect_remote(struct lcrt_terminal *lterminal)
{
}

static void lcrt_shell_create_subbox(struct lcrt_qconnect *lqconnect)
{
    GtkWidget *vbox;
    GtkWidget *vbox_spec;
    GtkWidget *hbox2;
    GtkWidget *label_shell;
    GtkWidget *combobox_shell;
    GtkWidget *hbox3;
    struct lcrt_window *parent;
	int i;
    static const char *shells[LCRT_SHELL_SUPPORT_NUMBER] = {LCRT_SHELL_SUPPORT};

    parent = lqconnect->parent;

    vbox = GTK_DIALOG (lqconnect->q_connect)->vbox;

    vbox_spec = gtk_vbox_new (FALSE, 0);
    lqconnect->q_vbox_spec = vbox_spec;
    gtk_widget_show (vbox_spec);
    gtk_box_pack_start (GTK_BOX (vbox), vbox_spec, TRUE, TRUE, 0);
    gtk_box_reorder_child (GTK_BOX (vbox), vbox_spec, 1);
    gtk_widget_set_size_request (vbox_spec, -1, 210);

    hbox2 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox2);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox2, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox2, -1, 25);

    label_shell = gtk_label_new (lqconnect->config.value[LCRT_Q_SHELL]);
    gtk_widget_show (label_shell);
    gtk_box_pack_start (GTK_BOX (hbox2), label_shell, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_shell, 90, 25);
    gtk_misc_set_alignment (GTK_MISC (label_shell), 0, 0.5);

    combobox_shell = gtk_combo_box_entry_new_text ();
    lqconnect->pshell.shell = combobox_shell;
    gtk_widget_show (combobox_shell);
    gtk_box_pack_start (GTK_BOX (hbox2), combobox_shell, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_shell, SERIAL_COMBOBOX_WIDTH, 25);
    
    for (i = 0; i < LCRT_SHELL_SUPPORT_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_shell), shells[i]);
    }
    debug_where();
    //gtk_entry_set_editable(GTK_ENTRY(GTK_BIN(combobox_shell)->child), FALSE);


    hbox3 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox3);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox3, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox3, -1, 185);

    if (lqconnect->flag == LCRT_QCONNECT_SESSION_OPTION) {
        struct lcrtc_user *user;
        if ((user = lcrt_user_find_by_name(&parent->u_config, lqconnect->uname)) != NULL) {
            gtk_window_set_focus(GTK_WINDOW(lqconnect->q_connect), lqconnect->pshell.shell);
			/* NOTE: there is very ugly, we use passwd field to store shell index */
            gtk_combo_box_set_active(GTK_COMBO_BOX(lqconnect->pshell.shell), atoi(user->password));
        }
    } else {
        gtk_window_set_focus(GTK_WINDOW(lqconnect->q_connect), lqconnect->pshell.shell);
    	gtk_widget_set_sensitive(lqconnect->q_bt_connect, TRUE);
    }
}

static struct lcrtc_user *lcrt_shell_create_session(struct lcrt_qconnect *lqconnect)
{
}

struct lcrt_protocol_callback lcrt_protocol_shell_callbacks = {
    .protocol         = LCRT_PROTOCOL_SHELL,
    .contents_changed = lcrt_shell_contents_changed,
    .connect_remote   = lcrt_shell_connect_remote,
    .create_subbox    = lcrt_shell_create_subbox,
    .create_session   = lcrt_shell_create_session,
};

