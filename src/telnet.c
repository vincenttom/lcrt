/*
 * Copyright (c) 2011 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Sun 07 Aug 2011 05:08:19 PM CST
 * File Name   : telnet.c
 *
 * Description:  
 */
//#define __LCRT_DEBUG__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "iterminal.h"
#include "iwindow.h"
#include "user.h"
#include "iqconnect.h"
#include "cqconnect.h"
#include "iterminal.h"
#include "ilogin.h"
#include "debug.h"
#include "telnet.h"

struct lcrt_telnet_if {
    GtkWidget *hostname;
    GtkWidget *username;
    GtkWidget *firewall;
    GtkWidget *port;
};

struct lcrt_telnet_tm {
    struct lcrt_login *login;
};

static void lcrt_telnet_receive(struct lcrt_terminal *lterminal)
{
    struct lcrt_window *lwindow = lterminal->parent->parent;
    VteTerminal *vteterminal = lterminal->terminal;
    struct lcrt_telnet_tm *ttelnet = (struct lcrt_telnet_tm *)lterminal->private_data;
    struct lcrtc_user *user = lterminal->user;
    char *text;

    text = vte_terminal_get_text(vteterminal, NULL, NULL, NULL);
    if (lcrt_check_space_string(text)) {
        debug_print("text null\n");
        return;
    }
#if 1
    debug_print("++++++++++++++++++++++++CONTENTS++++++++++++++++++++++++\n");
    debug_print("%s", text);
    debug_print("++++++++++++++++++++++++CONTENTS++++++++++++++++++++++++\n");
#endif
    if (strstr(text, "Connection refused") != NULL) {
        lcrt_message_error(lwindow->window, 
                           lcrt_terminal_get_config(lterminal, LCRT_TM_CONNECTION_REFUSED));
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
        return;
    }
    if (strstr(text, "Connection closed") != NULL) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
        return;
    }
    if (strstr(text, "Login incorrect") != NULL) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
        ttelnet->login = lcrt_create_login(lterminal, TRUE);
    }
    if (strstr(text, "login:") != NULL && 
        (lterminal->connected & LCRT_TERMINAL_SEND_USERNAME) == 0) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
        if (strlen(user->username) == 0) {
            ttelnet->login = lcrt_create_login(lterminal, FALSE);
            debug_where();
        }
        debug_where();
        lterminal->connected |= LCRT_TERMINAL_SEND_USERNAME;
        vte_terminal_feed_child(vteterminal, user->username, strlen(user->username));
        vte_terminal_feed_child(vteterminal, LCRT_TERMINAL_SEND_CMD, 
                                strlen(LCRT_TERMINAL_SEND_CMD));
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
        return;
    }
    if (strstr(text, "Password:") != NULL) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
        if (strlen(user->password) == 0) {
            struct lcrt_login *login = lcrt_create_login(lterminal, FALSE);
            ttelnet->login = login;
            debug_where();
        }
        debug_where();
        lterminal->connected |= LCRT_TERMINAL_SEND_PASSWORD;
        vte_terminal_feed_child(vteterminal, user->password, strlen(user->password));
        vte_terminal_feed_child(vteterminal, LCRT_TERMINAL_SEND_CMD, 
                                strlen(LCRT_TERMINAL_SEND_CMD));
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
        return;
    }
    if (lterminal->connected == (LCRT_TERMINAL_SEND_USERNAME | LCRT_TERMINAL_SEND_PASSWORD))
        lcrt_terminal_set_connected_status(lterminal);
}

static int lcrt_telnet_connect(struct lcrt_terminal *lterminal)
{
    lcrt_protocol_t protocol;
    struct lcrtc_user *user;
    char *argv[5], *work_dir;
    char hostname[256], port[32];
    char *dep_prog[] = {LCRT_DEP_PROG};
    int dep = -1;
    char tmp[32];
    struct lcrt_telnet_tm *ttelnet;

    if (lterminal == NULL)
       return EINVAL;

    ttelnet = (struct lcrt_telnet_tm *)calloc(1, sizeof(struct lcrt_telnet_tm));
    if (ttelnet == NULL)
        return -ENOMEM;
    lterminal->private_data = ttelnet;
    user = lterminal->user;
    argv[0] = dep_prog[LCRT_DEP_TELNET];
    strcpy(hostname, user->hostname);
    argv[1] = hostname;

    sprintf(port, "%d", user->port);
    argv[2] = port;
    argv[3] = NULL;
    work_dir = ".";

    if (lcrt_exec_check(user->protocol) != 0) {
        lcrt_message_info(lterminal->parent->parent->window, 
                          lterminal->parent->config.value[LCRT_TM_CONNECTION_PROG_NOT_FOUND],
                          argv[0]);
        return LCRTE_NOT_FOUND;
    }

    lterminal->child_pid  = vte_terminal_fork_command(VTE_TERMINAL(lterminal->terminal), 
                argv[0], argv, NULL , work_dir, FALSE, FALSE, FALSE);
    debug_print("child_pid = %d\n", lterminal->child_pid);
    lcrt_statusbar_set_user(lterminal->parent->parent->w_statusbar, lterminal->user);
    debug_where();
    return LCRTE_OK;

}
void lcrt_telnet_disconnect(struct lcrt_terminal *lterminal)
{
    if (lterminal->connected) {
        vte_terminal_feed_child(lterminal->terminal, 
                            LCRT_TERMINAL_EXIT_CMD, 
                            strlen(LCRT_TERMINAL_EXIT_CMD));
        lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_DISCONNECT);
    }
    if (lterminal->private_data) {
        free(lterminal->private_data);
        lterminal->private_data = NULL;
    }
}

static void lcrt_telnet_show(struct lcrt_qconnect *lqconnect)
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
    GtkWidget *label_firewall;
    GtkWidget *combobox_firewall;
    GtkWidget *label_port;
    GtkWidget *entry_port;
    GtkWidget *vbox3;
    char *firewall[LCRT_FIREWALL_NUMBER] = {LCRT_FIREWALL_NAME};
    int i;
    struct lcrt_window *parent;
    const char *str_port[LCRT_PROTOCOL_NUMBER] = {LCRT_PROTOCOL_PORT};
    static struct lcrt_telnet_if sltelnet, *ltelnet = &sltelnet;

    memset(ltelnet, 0, sizeof(struct lcrt_telnet_if));
    lqconnect->private_data = ltelnet;


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
    ltelnet->hostname = entry_hostname;
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
    ltelnet->username = entry_username;
    gtk_widget_show (entry_username);
    gtk_box_pack_start (GTK_BOX (hbox4), entry_username, FALSE, TRUE, 0);
    gtk_widget_set_size_request (entry_username, 220, 30);
    gtk_entry_set_invisible_char (GTK_ENTRY (entry_username), 9679);

    hbox3 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox3);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox3, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox3, -1, 25);

    label_firewall = gtk_label_new (lqconnect->config.value[LCRT_Q_LB_FIREWALL]);
    gtk_widget_show (label_firewall);
    gtk_box_pack_start (GTK_BOX (hbox3), label_firewall, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_firewall, 90, 25);
    gtk_misc_set_alignment (GTK_MISC (label_firewall), 0, 0.5);

    combobox_firewall = gtk_combo_box_entry_new_text ();
    ltelnet->firewall = combobox_firewall;
    gtk_widget_show (combobox_firewall);
    gtk_box_pack_start (GTK_BOX (hbox3), combobox_firewall, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_firewall, 110, 25);
    debug_where();
    for (i = 0; i < LCRT_FIREWALL_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_firewall), firewall[i]);
    }
    gtk_entry_set_editable(GTK_ENTRY(GTK_BIN(combobox_firewall)->child), FALSE);
    debug_where();

    label_port = gtk_label_new (lqconnect->config.value[LCRT_Q_LB_PORT]);
    gtk_widget_show (label_port);
    gtk_box_pack_start (GTK_BOX (hbox3), label_port, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_port, 50, 30);
    gtk_misc_set_alignment (GTK_MISC (label_port), 0, 0.49);

    entry_port = gtk_entry_new ();
    ltelnet->port = entry_port;
    gtk_widget_show (entry_port);
    gtk_box_pack_start (GTK_BOX (hbox3), entry_port, FALSE, TRUE, 0);
    gtk_widget_set_size_request (entry_port, 60, 30);
    gtk_entry_set_invisible_char (GTK_ENTRY (entry_port), 9679);
    
    vbox3 = gtk_vbox_new (FALSE, 0); 
    gtk_widget_show (vbox3);
    gtk_box_pack_start (GTK_BOX (vbox_spec), vbox3, TRUE, TRUE, 0); 
    gtk_widget_set_size_request (vbox3, 200, 105);

    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_firewall), 0);

    if (lqconnect->flag == LCRT_QCONNECT_SESSION_OPTION) {
        struct lcrtc_user *user;
        char tmp[32];
        if ((user = lcrt_user_find_by_name(&parent->u_config, lqconnect->uname)) != NULL) {
            gtk_window_set_focus(GTK_WINDOW(lqconnect->q_connect), ltelnet->hostname);
            gtk_entry_set_text(GTK_ENTRY(ltelnet->hostname), user->hostname);
            gtk_entry_set_text(GTK_ENTRY(ltelnet->username), user->username);
            sprintf(tmp, "%d", user->port);
            gtk_entry_set_text(GTK_ENTRY(ltelnet->port), tmp);
            gtk_entry_set_text(GTK_ENTRY(lqconnect->q_et_default_command), user->command);
        }
    } else {
        gtk_window_set_focus(GTK_WINDOW(lqconnect->q_connect), ltelnet->hostname);
    	gtk_widget_set_sensitive(lqconnect->q_bt_connect, FALSE);
    }
    gtk_entry_set_text(GTK_ENTRY(ltelnet->port), str_port[lqconnect->nproto]);
    gtk_entry_set_editable(GTK_ENTRY(ltelnet->port), FALSE);
}

static struct lcrtc_user *lcrt_telnet_create(struct lcrt_qconnect *lqconnect)
{
    lcrt_protocol_t protocol = lqconnect->nproto;
    struct lcrt_window *lwindow = lqconnect->parent;
    struct lcrt_telnet_if *ltelnet = (struct lcrt_telnet_if *)lqconnect->private_data;
    const char *hostname;
    struct lcrtc_user *user;
    int i = 1;
    char name[HOSTNAME_LEN + 256];
    debug_where();
    hostname = gtk_entry_get_text(GTK_ENTRY(ltelnet->hostname));
    
    strcpy(name, hostname);

    if (lqconnect->flag != LCRT_QCONNECT_SESSION_OPTION) {

        if ((user = lcrtc_user_create()) == NULL) {
            /* 
             * FIXME: There is no more memory, how can 
             * we handle this exception ?
             */
            return;
        }

        lcrt_user_find_unused_label(lwindow, hostname, name);

        lcrtc_user_set_data(
           user,
           name,
           hostname,
           protocol,
           gtk_entry_get_text(GTK_ENTRY(ltelnet->username)),
           NULL,
           gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
           atoi(gtk_entry_get_text(GTK_ENTRY(ltelnet->port))),
           TRUE
        );
        lcrtc_user_ref(user);
        lcrt_user_add(&lwindow->u_config, user);
        lcrt_window_set_current_user(lwindow, user);
        if (lqconnect->flag == LCRT_QCONNECT_IN_TAB) {
            lcrt_create_terminal(lwindow->w_notebook);
        }
    } else {
        if ((user = lcrt_user_find_by_name(&lwindow->u_config, lqconnect->uname)) != NULL) {
            lcrtc_user_set_data(
               user,
               lqconnect->uname,
               hostname,
               protocol,
               gtk_entry_get_text(GTK_ENTRY(ltelnet->username)),
               NULL,
               gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
               atoi(gtk_entry_get_text(GTK_ENTRY(ltelnet->port))),
               TRUE
            );
        }
    }
    lcrtc_user_dump(user, __func__);
    return user;

}

struct lcrt_protocol_callback lcrt_protocol_telnet_callbacks = {
    .protocol   = LCRT_PROTOCOL_TELNET,
    .receive    = lcrt_telnet_receive,
    .connect    = lcrt_telnet_connect,
    .disconnect = lcrt_telnet_disconnect,
    .show       = lcrt_telnet_show,
    .create     = lcrt_telnet_create,
    .changed    = NULL
};

struct lcrt_protocol_callback lcrt_protocol_telnet_ssl_callbacks = {
    .protocol   = LCRT_PROTOCOL_TELNET_SSL,
    .receive    = lcrt_telnet_receive,
    .connect    = lcrt_telnet_connect,
    .disconnect = lcrt_telnet_disconnect,
    .show       = lcrt_telnet_show,
    .create     = lcrt_telnet_create,
    .changed    = NULL
};

#if 0
        hostname = "shell";
    else
        hostname = gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_hostname));
    
    strcpy(name, hostname);
    if (lqconnect->flag != LCRT_QCONNECT_SESSION_OPTION) {
        if (protocol != LCRT_PROTOCOL_SHELL ||
            (protocol == LCRT_PROTOCOL_SHELL && 
            (user = lcrt_user_find_by_name(&lwindow->u_config, name)) == NULL)) {
    
            user = lcrtc_user_create();
            if (user == NULL)
                return;
            if (lcrt_user_find_by_name(&lwindow->u_config, name) != NULL) {
                do {
                    sprintf(name, "%s (%d)", hostname, i++);
                    if (lcrt_user_find_by_name(&lwindow->u_config, name) == NULL)
                        break;
                } while (i < 100);
            }
            lcrtc_user_set_data(
               user,
               name,
               hostname,
               protocol,
               gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_username)),
               NULL,
               gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
               atoi(gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_port))),
               TRUE
            );
            lcrtc_user_ref(user);
            lcrt_user_add(&lwindow->u_config, user);
        }
        lcrt_window_set_current_user(lwindow, user);
        if (lqconnect->flag == LCRT_QCONNECT_IN_TAB) {
            lcrt_create_terminal(lwindow->w_notebook);
        }
    } else {
        if ((user = lcrt_user_find_by_name(&lwindow->u_config, lqconnect->uname)) != NULL) {
            lcrtc_user_set_data(
               user,
               lqconnect->uname,
               hostname,
               protocol,
               gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_username)),
               NULL,
               gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
               atoi(gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_port))),
               TRUE
            );
        }
    }

}
#endif
