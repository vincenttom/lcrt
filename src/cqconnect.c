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
#define __LCRT_DEBUG__
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
    struct lcrtc_user *user = NULL;

    debug_where();
    if (lqconnect->ops && lqconnect->ops->create) {
        debug_where();
        user = lqconnect->ops->create(lqconnect);
    }
    if (user) {
        lcrt_qconnect_on_button_cancel_clicked(NULL, lqconnect);
        if (lcrt_window_get_auto_save(lwindow)) {
            lcrt_user_save_one(&lwindow->u_config, user);
        }
        debug_print("qconnecting...\n");
        lqconnect->f_status = GTK_RESPONSE_OK;
    } else {
        fprintf(stderr, "connect faild...\n");
        lqconnect->f_status = GTK_RESPONSE_CANCEL;
    }
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
	const char *str_proto[LCRT_PROTOCOL_NUMBER] = {LCRT_PROTOCOL_NAME};

    lqconnect->nproto = lcrt_user_get_protocol(proto);

	debug_where();
    debug_print("protocol: %s\n", str_proto[lqconnect->nproto]);
    if (lqconnect->q_vbox_spec) {
        gtk_widget_destroy(lqconnect->q_vbox_spec);
        lqconnect->q_vbox_spec = NULL;
    }

    lqconnect->ops = lcrt_protocol_get_callback(lqconnect->nproto);
    
    if (lqconnect->ops && lqconnect->ops->show)
        lqconnect->ops->show(lqconnect);
}
