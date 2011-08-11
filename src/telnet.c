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

#include <stdio.h>
#include <string.h>
#include "iterminal.h"
#include "iwindow.h"
#include "user.h"
#include "iqconnect.h"
#include "iterminal.h"
#include "ilogin.h"
#include "debug.h"

void lcrt_qconnect_create_telnet_subbox(struct lcrt_qconnect *lqconnect)
{
#if 0
    gtk_entry_set_text(GTK_ENTRY(lqconnect->q_et_port), str_port[protocol]);
        if (protocol == LCRT_PROTOCOL_TELNET || protocol == LCRT_PROTOCOL_TELNET_SSL)
                    gtk_entry_set_editable(GTK_ENTRY(lqconnect->q_et_port), FALSE)
#endif
}

void *lcrt_terminal_telnet_contents_changed(struct lcrt_terminal *lterminal)
{
    struct lcrt_window *lwindow = lterminal->parent->parent;
    VteTerminal *vteterminal = lterminal->terminal;
    struct lcrtc_user *user = lterminal->user;
    char *text;

    text = vte_terminal_get_text(vteterminal, NULL, NULL, NULL);
    if (lcrt_check_space_string(text)) {
        debug_print("text null\n");
        return;
    }

    if (strstr(text, "Connection refused") != NULL ||
        strstr(text, "Connection closed") != NULL) {
        lcrt_message_error(lwindow->window, 
                           lcrt_terminal_get_config(lterminal, LCRT_TM_CONNECTION_REFUSED));
        return;
    }
    if (strstr(text, "Login incorrect") != NULL) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
        struct lcrt_login *login = lcrt_create_login(lterminal, TRUE);
        lterminal->login = login;
    }
    if (strstr(text, "login:") != NULL && 
        (lterminal->connected & LCRT_TERMINAL_SEND_USERNAME) == 0) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
        if (strlen(user->username) == 0) {
            struct lcrt_login *login = lcrt_create_login(lterminal, FALSE);
            lterminal->login = login;
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
            lterminal->login = login;
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
struct lcrtc_user *lcrt_qconnect_telnet_on_button_connect_clicked(struct lcrt_qconnect *lqconnect)
{
}
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
