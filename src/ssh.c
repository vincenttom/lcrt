/*
 * Copyright (c) 2011 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Sun 07 Aug 2011 05:01:18 PM CST
 * File Name   : ssh.c
 *
 * Description:  
 */
#define __LCRT_DEBUG__
#include <stdio.h>
#include <string.h>
#include "iterminal.h"
#include "iqconnect.h"
#include "cqconnect.h"
#include "ilogin.h"
#include "debug.h"

static void __lcrt_toggled_authentication(
    GtkTreeViewColumn *tree_column, 
    GtkCellRenderer *cell,
    GtkTreeModel *model, 
    GtkTreeIter *iter, 
    gpointer data);

static void __lcrt_set_authentication_func(
    GtkCellRendererToggle *cell, 
    const gchar *path_str,
    GtkTreeModel *model);

static void __lcrt_qconnect_create_ssh2_authentication(struct lcrt_qconnect *lqconnect);
static void __lcrt_qconnect_create_ssh1_authentication(struct lcrt_qconnect *lqconnect);

void lcrt_qconnect_create_ssh_subbox(struct lcrt_qconnect *lqconnect)
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
    GtkWidget *frame;
    GtkWidget *alignment;
    GtkWidget *hbox_frame;
    GtkWidget *vbox_frame;
    GtkWidget *vbox3;
    GtkWidget *checkbutton_passwd;
    GtkWidget *vscrollbar;
    GtkWidget *vbox2;
    GtkWidget *button_properties;
    GtkWidget *fixed3;
    GtkWidget *label_frame;
    GtkWidget *tree_view;
    GtkWidget *scrolledwindow;
    GtkListStore *list_store;
    GtkCellRenderer *cell;
    char *firewall[LCRT_FIREWALL_NUMBER] = {LCRT_FIREWALL_NAME};
    int i;
    struct lcrt_window *parent;
    const char *str_port[LCRT_PROTOCOL_NUMBER] = {LCRT_PROTOCOL_PORT};

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
    lqconnect->pssh.hostname = entry_hostname;
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
    lqconnect->pssh.username = entry_username;
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
    lqconnect->pssh.firewall = combobox_firewall;
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
    lqconnect->pssh.port = entry_port;
    gtk_widget_show (entry_port);
    gtk_box_pack_start (GTK_BOX (hbox3), entry_port, FALSE, TRUE, 0);
    gtk_widget_set_size_request (entry_port, 60, 30);
    gtk_entry_set_invisible_char (GTK_ENTRY (entry_port), 9679);
    
    vbox3 = gtk_vbox_new (FALSE, 0); 
    gtk_widget_show (vbox3);
    gtk_box_pack_start (GTK_BOX (vbox_spec), vbox3, TRUE, TRUE, 0); 
    gtk_widget_set_size_request (vbox3, -1, 105);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_widget_set_size_request (frame, -1, 105);
    gtk_box_pack_start (GTK_BOX (vbox3), frame, TRUE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_OUT);

    hbox_frame = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox_frame);
    gtk_container_add (GTK_CONTAINER (frame), hbox_frame);

    vbox_frame = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox_frame);
    gtk_box_pack_start (GTK_BOX (hbox_frame), vbox_frame, FALSE, TRUE, 0);
    gtk_widget_set_size_request (vbox_frame, 200, -1);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_size_request (scrolledwindow, 200, -1);
    gtk_widget_show (scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox_frame), scrolledwindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), 
              GTK_POLICY_NEVER, 
              GTK_POLICY_ALWAYS);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    list_store = gtk_list_store_new (2, GTK_TYPE_POINTER, G_TYPE_STRING);
    lqconnect->pssh.authentication = list_store;

    tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list_store));
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (tree_view), FALSE);
    gtk_widget_show (tree_view);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), tree_view);

    cell = gtk_cell_renderer_toggle_new ();
    g_signal_connect (cell, "toggled", 
            G_CALLBACK (__lcrt_set_authentication_func), list_store);
    gtk_tree_view_insert_column_with_data_func (GTK_TREE_VIEW (tree_view),
                            -1, "record", cell,
                            __lcrt_toggled_authentication, NULL, NULL);

    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (tree_view),
                             -1, "text", gtk_cell_renderer_text_new (),
                             "text", 1, NULL);

    g_object_unref (list_store);

    if (lqconnect->nproto == LCRT_PROTOCOL_SSH2) {
        /* ssh2 authentication */
        __lcrt_qconnect_create_ssh2_authentication(lqconnect);
    } else {
        /* ssh1 authentication */
        __lcrt_qconnect_create_ssh1_authentication(lqconnect);
    }
    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);
    gtk_box_pack_start (GTK_BOX (hbox_frame), vbox2, TRUE, FALSE, 0);

    button_properties = gtk_button_new_with_mnemonic (lqconnect->config.value[LCRT_Q_BT_PROPERTIES]);
    lqconnect->q_bt_properties= button_properties;
    gtk_widget_show (button_properties);
    gtk_box_pack_start (GTK_BOX (vbox2), button_properties, FALSE, FALSE, 0);
    gtk_widget_set_size_request (button_properties, -1, 30);

    fixed3 = gtk_fixed_new ();
    gtk_widget_show (fixed3);
    gtk_box_pack_start (GTK_BOX (hbox_frame), fixed3, TRUE, TRUE, 0);
    gtk_widget_set_size_request (fixed3, 10, -1);

    label_frame = gtk_label_new (lqconnect->config.value[LCRT_Q_LB_FRAME]);
    gtk_widget_show (label_frame);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label_frame);
    gtk_label_set_use_markup (GTK_LABEL (label_frame), TRUE);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_firewall), 0);

    if (lqconnect->flag == LCRT_QCONNECT_SESSION_OPTION) {
        struct lcrtc_user *user;
        char tmp[32];
        if ((user = lcrt_user_find_by_name(&parent->u_config, lqconnect->uname)) != NULL) {
            gtk_window_set_focus(GTK_WINDOW(lqconnect->q_connect), lqconnect->pssh.hostname);
            gtk_entry_set_text(GTK_ENTRY(lqconnect->pssh.hostname), user->hostname);
            gtk_entry_set_text(GTK_ENTRY(lqconnect->pssh.username), user->username);
            sprintf(tmp, "%d", user->port);
            gtk_entry_set_text(GTK_ENTRY(lqconnect->pssh.port), tmp);
            gtk_entry_set_text(GTK_ENTRY(lqconnect->q_et_default_command), user->command);
        }
    } else {
        gtk_window_set_focus(GTK_WINDOW(lqconnect->q_connect), lqconnect->pssh.hostname);
    }
    gtk_entry_set_text(GTK_ENTRY(lqconnect->pssh.port), str_port[lqconnect->nproto]);

}
static void __lcrt_toggled_authentication(
    GtkTreeViewColumn *tree_column, 
    GtkCellRenderer *cell,
    GtkTreeModel *model, 
    GtkTreeIter *iter, 
    gpointer data)
{
    gboolean *selected;

    gtk_tree_model_get (model, iter, 0, &selected, -1);
    g_object_set (cell, "active", *selected, NULL);
}

static void __lcrt_set_authentication_func(
    GtkCellRendererToggle *cell, 
    const gchar *path_str,
    GtkTreeModel *model)
{
    GtkTreePath *path;
    GtkTreeIter iter;
    gboolean *brecord;
  
    path = gtk_tree_path_new_from_string (path_str);
    gtk_tree_model_get_iter (model, &iter, path);
  
    gtk_tree_model_get (model, &iter, 0, &brecord, -1);
    //change record state
    *brecord = !*brecord;
  
    gtk_tree_model_row_changed (model, path, &iter);
    gtk_tree_path_free (path);
}
static void __lcrt_qconnect_create_ssh_authentication(
    GtkListStore *store, 
    const gchar *text, 
    gboolean *brecord)
{
  GtkTreeIter iter;

  gtk_list_store_append (store, &iter);
  *brecord = !*brecord;
  gtk_list_store_set (store, &iter, 0, brecord, 1, text, -1);
}
static void __lcrt_qconnect_create_ssh2_authentication(struct lcrt_qconnect *lqconnect)
{
    GtkListStore *store = lqconnect->pssh.authentication;
    
    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_PASSWD], 
            &lqconnect->pssh.auth[0]);
    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_PUBLICKEY],
            &lqconnect->pssh.auth[1]);
    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_KEYBOARD],
            &lqconnect->pssh.auth[2]);
    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_GSSAPI],
            &lqconnect->pssh.auth[3]);
}
static void __lcrt_qconnect_create_ssh1_authentication(struct lcrt_qconnect *lqconnect)
{
    GtkListStore *store = lqconnect->pssh.authentication;

    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_PASSWD],
            &lqconnect->pssh.auth[0]);
    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_RSA],
            &lqconnect->pssh.auth[1]);
    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_TIS],
            &lqconnect->pssh.auth[2]);
}

void *lcrt_terminal_ssh_contents_changed(struct lcrt_terminal *lterminal)
{
    VteTerminal *vteterminal = lterminal->terminal;
    struct lcrt_window *lwindow = lterminal->parent->parent;
    struct lcrtc_user *user = lterminal->user;
    char *text;

    debug_where();
    debug_print("VTETerminal %p contents changed\n", vteterminal);
    text = vte_terminal_get_text(vteterminal, NULL, NULL, NULL);
    if (lcrt_check_space_string(text)) {
        debug_print("text null\n");
        return NULL;
    }
    debug_print("++++++++++++++++++++++++CONTENTS++++++++++++++++++++++++\n");
    debug_print("%s", text);
    debug_print("++++++++++++++++++++++++CONTENTS++++++++++++++++++++++++\n");
    if (strstr(text, "Connection timed out") != NULL) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE); 
        lcrt_message_error(lwindow->window,
                           lcrt_terminal_get_config(lterminal, LCRT_TM_CONNECTION_TIMEOUT)); 
        return NULL;
    }
    if (strstr(text, "Connection refused") != NULL) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE); 
        lcrt_message_error(lwindow->window, 
                           lcrt_terminal_get_config(lterminal, LCRT_TM_CONNECTION_REFUSED));
        return NULL;
    }
    if (strstr(text, "Permission denied") != NULL) {
        lterminal->again++;
        debug_print("Password or username error, try again %d\n", lterminal->again);

        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE); 

        if (lterminal->again == 3) {
            //vte_terminal_feed(lterminal->terminal, "connect field\n", strlen("connect field\n"));
            lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_DISCONNECT);
            lcrt_message_error(lterminal->parent->parent->window, 
                               lcrt_terminal_get_config(lterminal, LCRT_TM_CONNECTION_FAILED));
            lterminal->again = 0;
            return NULL;
        }
        struct lcrt_login *login = lcrt_create_login(lterminal, TRUE);
        lterminal->login = login;
        if (lterminal->username_changed == TRUE) {
            kill(lterminal->child_pid, SIGKILL);
            lterminal->child_pid = 0;
            lterminal->username_changed = FALSE;
            lcrt_terminal_fork(lterminal);
            return;
        }
        //lcrt_terminal_fork(lterminal);
        debug_where();
        //return;
    }
    char password_alert[256];
    sprintf(password_alert, "%s@%s's password:", user->username, user->hostname);
    if (strstr(text, password_alert) != NULL) {

        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);

        /* 
         * there is no password or username, we should create 
         * a dialog to get username and password 
         */
        if (strlen(user->password) == 0 ||
            strlen(user->username) == 0) { /* ||
            (lterminal->connected == LCRT_TERMINAL_CONNECTING)) {*/
            debug_print("Enter username or password\n");
            struct lcrt_login *login = lcrt_create_login(lterminal, FALSE);
            lterminal->login = login;
            debug_where();
        }
        debug_print("Auto send username and password\n");
        if (lterminal->username_changed == TRUE) {
            kill(lterminal->child_pid, SIGKILL);
            lterminal->child_pid = 0;
            lterminal->username_changed = FALSE;
            lcrt_terminal_fork(lterminal);
            return NULL;
        }
        vte_terminal_feed_child(vteterminal, user->password, strlen(user->password));
        vte_terminal_feed_child(vteterminal, LCRT_TERMINAL_SEND_CMD, 
                strlen(LCRT_TERMINAL_SEND_CMD));
        lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_CONNECTING);
        debug_where();
        return NULL;
    }

    if (strstr(text, "Are you sure you want to continue connecting") != NULL) {
        vte_terminal_feed_child(vteterminal, LCRT_TERMINAL_YES_CMD, strlen(LCRT_TERMINAL_YES_CMD));

        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE); 
        lterminal->connected = LCRT_TERMINAL_WAIT_RETURN;

        debug_where();
        return NULL;
    }
    if (lterminal->connected == LCRT_TERMINAL_WAIT_RETURN) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
        lterminal->connected = LCRT_TERMINAL_DISCONNECT;
        return NULL;
    }
#if 0
    if (lterminal->connected == LCRT_TERMINAL_CONNECTING || lterminal->connected != LCRT_TERMINAL_WAIT_RETURN)
#endif
        lcrt_terminal_set_connected_status(lterminal);
    return NULL;
}

struct lcrtc_user *lcrt_qconnect_ssh_on_button_connect_clicked(struct lcrt_qconnect *lqconnect)
{
    lcrt_protocol_t protocol = lqconnect->nproto;
    struct lcrt_window *lwindow = lqconnect->parent;
    const char *hostname;
    struct lcrtc_user *user;
    int i = 1;
    char name[HOSTNAME_LEN + 256];
    debug_where();
    hostname = gtk_entry_get_text(GTK_ENTRY(lqconnect->pssh.hostname));
    
    strcpy(name, hostname);
    if (lqconnect->flag != LCRT_QCONNECT_SESSION_OPTION) {
        if ((user = lcrt_user_find_by_name(&lwindow->u_config, name)) == NULL) {
    
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
               gtk_entry_get_text(GTK_ENTRY(lqconnect->pssh.username)),
               NULL,
               gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
               atoi(gtk_entry_get_text(GTK_ENTRY(lqconnect->pssh.port))),
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
               gtk_entry_get_text(GTK_ENTRY(lqconnect->pssh.username)),
               NULL,
               gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
               atoi(gtk_entry_get_text(GTK_ENTRY(lqconnect->pssh.port))),
               TRUE
            );
        }
    }
    lcrtc_user_dump(user, __func__);
    return user;
}

