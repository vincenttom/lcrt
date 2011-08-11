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
#include "cqconnect.h"
#include "user.h"
#include "debug.h"

static void lcrt_rlogin_contents_changed(struct lcrt_terminal *lterminal)
{
}

static int lcrt_rlogin_connect_remote(struct lcrt_terminal *lterminal)
{
}

static void lcrt_rlogin_create_subbox(struct lcrt_qconnect *lqconnect)
{
    GtkWidget *vbox;
    GtkWidget *vbox_spec;
    GtkWidget *hbox2;
    GtkWidget *label_hostname;
    GtkWidget *entry_hostname;
    GtkWidget *hbox4;
    GtkWidget *label_username;
    GtkWidget *entry_username;
    GtkWidget *hbox3;
    struct lcrt_window *parent;

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

    label_hostname = gtk_label_new (lqconnect->config.value[LCRT_Q_LB_HOSTNAME]);
    gtk_widget_show (label_hostname);
    gtk_box_pack_start (GTK_BOX (hbox2), label_hostname, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_hostname, 90, 30);
    gtk_misc_set_alignment (GTK_MISC (label_hostname), 0, 0.5);

    entry_hostname = gtk_entry_new_with_max_length(HOSTNAME_LEN);
    lqconnect->prlogin.hostname = entry_hostname;
    gtk_widget_show (entry_hostname);
    gtk_box_pack_start (GTK_BOX (hbox2), entry_hostname, FALSE, TRUE, 0);
    gtk_widget_set_size_request (entry_hostname, 220, 30);
    gtk_entry_set_invisible_char (GTK_ENTRY (entry_hostname), 9679);
    g_signal_connect ((gpointer) entry_hostname, "changed",
                    G_CALLBACK (lcrt_qconnect_on_entry_hostname_changed),
                    lqconnect);

    hbox4 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox4);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox4, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox4, -1, 30);

    label_username = gtk_label_new (lqconnect->config.value[LCRT_Q_LB_USERNAME]);
    gtk_widget_show (label_username);
    gtk_box_pack_start (GTK_BOX (hbox4), label_username, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_username, 90, 30);
    gtk_misc_set_alignment (GTK_MISC (label_username), 0, 0.5);

    entry_username = gtk_entry_new_with_max_length(USERNAME_LEN);
    lqconnect->prlogin.username = entry_username;
    gtk_widget_show (entry_username);
    gtk_box_pack_start (GTK_BOX (hbox4), entry_username, FALSE, TRUE, 0);
    gtk_widget_set_size_request (entry_username, 220, 30);
    gtk_entry_set_invisible_char (GTK_ENTRY (entry_username), 9679);

    hbox3 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox3);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox3, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox3, -1, 135);

    if (lqconnect->flag == LCRT_QCONNECT_SESSION_OPTION) {
        struct lcrtc_user *user;
        if ((user = lcrt_user_find_by_name(&parent->u_config, lqconnect->uname)) != NULL) {
            gtk_window_set_focus(GTK_WINDOW(lqconnect->q_connect), lqconnect->prlogin.hostname);
            gtk_entry_set_text(GTK_ENTRY(lqconnect->prlogin.hostname), user->hostname);
            gtk_entry_set_text(GTK_ENTRY(lqconnect->prlogin.username), user->username);
            gtk_entry_set_text(GTK_ENTRY(lqconnect->q_et_default_command), user->command);
        }
    } else {
        gtk_window_set_focus(GTK_WINDOW(lqconnect->q_connect), lqconnect->prlogin.hostname);
    	gtk_widget_set_sensitive(lqconnect->q_bt_connect, FALSE);
    }
}

static struct lcrtc_user *lcrt_rlogin_create_session(struct lcrt_qconnect *lqconnect)
{
}

struct lcrt_protocol_callback lcrt_protocol_rlogin_callbacks = {
    .protocol         = LCRT_PROTOCOL_RLOGIN,
    .contents_changed = lcrt_rlogin_contents_changed,
    .connect_remote   = lcrt_rlogin_connect_remote,
    .create_subbox    = lcrt_rlogin_create_subbox,
    .create_session   = lcrt_rlogin_create_session,
};

