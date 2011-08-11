/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sat 22 Jan 2011 06:33:56 PM CST
 *
 *
 * Description:  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iwindow.h"
#include "cwindow.h"
#include "iqconnect.h"
#include "debug.h"
#include "user.h"
#include "message.h"
#include "cqconnect.h"
#include "iterminal.h"
#include "ssh.h"
#include "telnet.h"
#include "rlogin.h"
#include "serial.h"
#include "shell.h"

void lcrt_qconnect_on_button_connect_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_qconnect *lqconnect = (struct lcrt_qconnect *)user_data;
    struct lcrt_window *lwindow = lqconnect->parent;
    struct lcrtc_user *user;

    debug_where();
    if (lqconnect->on_button_connect_clicked) {
        debug_where();
        user = lqconnect->on_button_connect_clicked(lqconnect);
    }

    lcrt_qconnect_on_button_cancel_clicked(NULL, lqconnect);
    if (lcrt_window_get_auto_save(lwindow)) {
        lcrt_user_save_one(&lwindow->u_config, user);
    }
    debug_print("qconnecting...\n");
    return;
}
void lcrt_qconnect_on_entry_hostname_changed(GtkWidget *widget, gpointer user_data)
{
    struct lcrt_qconnect *lqconnect = (struct lcrt_qconnect *)user_data;
    if (gtk_entry_get_text_length(GTK_ENTRY(widget)) > 0)
        gtk_widget_set_sensitive(lqconnect->q_bt_connect, TRUE);
    else
        gtk_widget_set_sensitive(lqconnect->q_bt_connect, FALSE);
}
void lcrt_qconnect_on_button_cancel_clicked (GtkButton *button, gpointer user_data)
{
    struct lcrt_qconnect *lqconnect = (struct lcrt_qconnect *)user_data;
    debug_where();
    gtk_widget_destroy(lqconnect->q_connect);
    lcrt_destroy_qconnect(lqconnect);
}

gboolean lcrt_qconnect_on_window_delete(GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
    lcrt_qconnect_on_button_cancel_clicked(NULL, user_data);
    return FALSE;
}
void lcrt_qconnect_on_protocol_changed(GtkComboBox *widget, gpointer user_data)
{
    struct lcrt_qconnect *lqconnect = (struct lcrt_qconnect *)user_data;
    const char *str_port[LCRT_PROTOCOL_NUMBER] = {LCRT_PROTOCOL_PORT};
    const char *proto = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
    typedef struct lcrtc_user *(*on_button_connect_clicked_callback_t)(struct lcrt_qconnect *lqconnect);
    typedef void (*create_subbox_callback_t)(struct lcrt_qconnect *lqconnect);

    on_button_connect_clicked_callback_t on_button_connect_clicked_callbacks[LCRT_PROTOCOL_NUMBER] = {
        LCRT_PROTOCOL_ON_BUTTON_CONNECT_CLICKED_CALLBACKS
    };

    create_subbox_callback_t create_subbox_callbacks[LCRT_PROTOCOL_NUMBER] = {
        LCRT_PROTOCOL_CREATE_SUBBOX_CALLBACKS
    };

    lqconnect->nproto = lcrt_user_get_protocol(proto);

    debug_print("protocol: %s\n", lqconnect->nproto);
    if (lqconnect->q_vbox_spec) {
        gtk_widget_destroy(lqconnect->q_vbox_spec);
        lqconnect->q_vbox_spec = NULL;
    }
    lqconnect->on_button_connect_clicked = on_button_connect_clicked_callbacks[lqconnect->nproto];
    lqconnect->create_subbox = create_subbox_callbacks[lqconnect->nproto];
    
    if (lqconnect->create_subbox)
        lqconnect->create_subbox(lqconnect);

#if 0
    gtk_widget_set_sensitive(lqconnect->q_et_hostname, TRUE);
    gtk_widget_set_sensitive(lqconnect->q_et_username, TRUE);
    gtk_widget_set_sensitive(lqconnect->q_cb_firewall, TRUE);
    gtk_widget_set_sensitive(lqconnect->q_et_port, TRUE);
    lcrt_qconnect_on_entry_hostname_changed(lqconnect->q_et_hostname, lqconnect);

    if (protocol == LCRT_PROTOCOL_SSH1) {
        gtk_widget_set_sensitive(lqconnect->q_ssh_frame, TRUE);
        lcrt_qconnect_destroy_ssh1(lqconnect);
        lcrt_qconnect_destroy_ssh2(lqconnect);
        lcrt_qconnect_create_ssh1(lqconnect);
    } else if (protocol == LCRT_PROTOCOL_SSH2) {
        gtk_widget_set_sensitive(lqconnect->q_ssh_frame, TRUE);
        lcrt_qconnect_destroy_ssh1(lqconnect);
        lcrt_qconnect_destroy_ssh2(lqconnect);
        lcrt_qconnect_create_ssh2(lqconnect);
    } else if (protocol == LCRT_PROTOCOL_SHELL) {
        gtk_widget_set_sensitive(lqconnect->q_et_hostname, FALSE);
        gtk_widget_set_sensitive(lqconnect->q_et_username, FALSE);
        gtk_widget_set_sensitive(lqconnect->q_cb_firewall, FALSE);
        gtk_widget_set_sensitive(lqconnect->q_et_port, FALSE);
        gtk_widget_set_sensitive(lqconnect->q_bt_connect, TRUE);
        gtk_widget_set_sensitive(lqconnect->q_ssh_frame, FALSE);
    } else {
        gtk_widget_set_sensitive(lqconnect->q_ssh_frame, FALSE);
    }
    gtk_entry_set_text(GTK_ENTRY(lqconnect->q_et_port), str_port[protocol]);
    if (protocol == LCRT_PROTOCOL_TELNET || protocol == LCRT_PROTOCOL_TELNET_SSL)
        gtk_entry_set_editable(GTK_ENTRY(lqconnect->q_et_port), FALSE);
#endif
}
