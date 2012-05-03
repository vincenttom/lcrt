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
//#define __LCRT_DEBUG__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "iterminal.h"
#include "iqconnect.h"
#include "cqconnect.h"
#include "ilogin.h"
#include "debug.h"
#include "ssh.h"
/* authentication for ssh */
enum {
    LCRT_AUTH_PASSWD = 0,
    LCRT_AUTH_PUBLICKEY,
    LCRT_AUTH_KEYBOARD,
    LCRT_AUTH_GSSAPI,
    LCRT_AUTH_RSA,
    LCRT_AUTH_TIS,
    LCRT_AUTH_NUMBER
};
struct lcrt_ssh_if {
    GtkWidget *hostname;
    GtkWidget *username;
    GtkWidget *firewall;
    GtkWidget *port;
    GtkWidget *passwd;
    GtkWidget *publickey;
    GtkWidget *keyboard;
    GtkWidget *gssapi;
    GtkWidget *rsa;
    GtkWidget *tis;
    GtkListStore *authliststore;
    GtkWidget *authtreeview;
    GtkWidget *properties;
    GtkWidget *ext[5];
};

struct lcrt_ssh_tm {
    int again; /**<  the counter of try again to connect with remote */
    int username_changed; /**< the flag to mark user change the username */
    struct lcrt_login *login; /**<  when create a new session, user should
                                use login dialog to enter username and password */
};
static void lcrt_authentication_passwd_callback(struct lcrt_qconnect *lqconnect);
static void lcrt_authentication_publickey_callback(struct lcrt_qconnect *lqconnect);
static void lcrt_authentication_ketboard_callback(struct lcrt_qconnect *lqconnect);
static void lcrt_authentication_gssapi_callback(struct lcrt_qconnect *lqconnect);
static void lcrt_authentication_rsa_callback(struct lcrt_qconnect *lqconnect);
static void lcrt_authentication_tis_callback(struct lcrt_qconnect *lqconnect);
/*
 * because the third filed will be changed every time, so
 * this structure can not be used by more objects at same time
 */
struct __lcrt_ssh_auth {
    int auth;
    void (*callback)(struct lcrt_qconnect*lqconnect);
    struct lcrt_qconnect *user_data;
};

static struct __lcrt_ssh_auth g_lcrt_ssh_auth[LCRT_AUTH_NUMBER] = {
    {TRUE, lcrt_authentication_passwd_callback, NULL},
    {TRUE, lcrt_authentication_publickey_callback, NULL},
    {TRUE, lcrt_authentication_ketboard_callback, NULL},
    {TRUE, lcrt_authentication_gssapi_callback, NULL},
    {TRUE, lcrt_authentication_rsa_callback, NULL},
    {TRUE, lcrt_authentication_tis_callback, NULL},
};

static void lcrt_ssh_receive(struct lcrt_terminal *lterminal)
{
    VteTerminal *vteterminal = lterminal->terminal;
    struct lcrt_window *lwindow = lterminal->parent->parent;
    struct lcrt_ssh_tm *tssh = (struct lcrt_ssh_tm *)lterminal->private_data;
    struct lcrtc_user *user = lterminal->user;
    char *text;

    debug_where();
    debug_print("VTETerminal %p contents changed\n", vteterminal);
    text = vte_terminal_get_text(vteterminal, NULL, NULL, NULL);
    if (lcrt_check_space_string(text)) {
        debug_print("text null\n");
        return;
    }
#if 0
    debug_print("++++++++++++++++++++++++CONTENTS++++++++++++++++++++++++\n");
    debug_print("%s", text);
    debug_print("++++++++++++++++++++++++CONTENTS++++++++++++++++++++++++\n");
#endif
    if (strstr(text, "Connection timed out") != NULL) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE); 
        lcrt_message_error(lwindow->window,
                           lcrt_terminal_get_config(lterminal, LCRT_TM_CONNECTION_TIMEOUT)); 
        return;
    }
    if (strstr(text, "Connection refused") != NULL) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE); 
        lcrt_message_error(lwindow->window, 
                           lcrt_terminal_get_config(lterminal, LCRT_TM_CONNECTION_REFUSED));
        return;
    }
    if (strstr(text, "Permission denied") != NULL) {
        tssh->again++;
        debug_print("Password or username error, try again %d\n", tssh->again);

        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE); 

        if (tssh->again >= 3) {
            //vte_terminal_feed(lterminal->terminal, "connect field\n", strlen("connect field\n"));
            lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_DISCONNECT);
            lcrt_message_error(lterminal->parent->parent->window, 
                               lcrt_terminal_get_config(lterminal, LCRT_TM_CONNECTION_FAILED));
            tssh->again = 0;
            return;
        }
        struct lcrt_login *login = lcrt_create_login(lterminal, TRUE);
        tssh->login = login;
        if (tssh->username_changed == TRUE) {
            kill(lterminal->child_pid, SIGKILL);
            lterminal->child_pid = 0;
            tssh->username_changed = FALSE;
            if (lterminal->ops && lterminal->ops->connect)
                lterminal->ops->connect(lterminal);
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
            tssh->login = lcrt_create_login(lterminal, FALSE);
            debug_where();
        }
        debug_print("Auto send username and password\n");
        if (tssh->username_changed == TRUE) {
            kill(lterminal->child_pid, SIGKILL);
            lterminal->child_pid = 0;
            tssh->username_changed = FALSE;
            if (lterminal->ops && lterminal->ops->connect)
                lterminal->ops->connect(lterminal);
            return;
        }
        vte_terminal_feed_child(vteterminal, user->password, strlen(user->password));
        vte_terminal_feed_child(vteterminal, LCRT_TERMINAL_SEND_CMD, 
                strlen(LCRT_TERMINAL_SEND_CMD));
        lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_CONNECTING);
        debug_where();
        return;
    }

    if (strstr(text, "Are you sure you want to continue connecting") != NULL) {
        vte_terminal_feed_child(vteterminal, LCRT_TERMINAL_YES_CMD, strlen(LCRT_TERMINAL_YES_CMD));

        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE); 
        lterminal->connected = LCRT_TERMINAL_WAIT_RETURN;

        debug_where();
        return;
    }
    if (lterminal->connected == LCRT_TERMINAL_WAIT_RETURN) {
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
        lterminal->connected = LCRT_TERMINAL_DISCONNECT;
        return;
    }
#if 0
    if (lterminal->connected == LCRT_TERMINAL_CONNECTING || lterminal->connected != LCRT_TERMINAL_WAIT_RETURN)
#endif
        lcrt_terminal_set_connected_status(lterminal);
    return;
}

static int lcrt_ssh_connect(struct lcrt_terminal *lterminal)
{
    lcrt_protocol_t protocol;
    struct lcrtc_user *user;
    char *argv[5], *work_dir;
    char hostname[256], port[32];
    char *dep_prog[] = {LCRT_DEP_PROG};
    int dep = -1;
    char tmp[32];
    struct lcrt_ssh_tm *tssh;
#if VTE_CHECK_VERSION(0, 26, 0)
    char path[512];
#endif

    if (lterminal == NULL)
       return -EINVAL;

    tssh = (struct lcrt_ssh_tm *)calloc(1, sizeof(struct lcrt_ssh_tm));
    if (tssh == NULL)
        return -ENOMEM;

    lterminal->private_data = tssh;

    vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);

    user = lterminal->user;
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

    if (lcrt_exec_check(protocol) != 0) {
        lcrt_message_info(lterminal->parent->parent->window, 
                          lterminal->parent->config.value[LCRT_TM_CONNECTION_PROG_NOT_FOUND],
                          argv[0]);
        return -LCRTE_NOT_FOUND;
    }
#if VTE_CHECK_VERSION(0, 26, 0)
    lcrt_echeck(argv[0], path);
    argv[0] = path;
    vte_terminal_fork_command_full(
                            VTE_TERMINAL(lterminal->terminal), 
                            VTE_PTY_DEFAULT,
                            work_dir,
                            argv, 
                            NULL, 
                            0, 
                            NULL, 
                            NULL,
                            &lterminal->child_pid, 
                            NULL);
#else
    lterminal->child_pid  = vte_terminal_fork_command(VTE_TERMINAL(lterminal->terminal), 
                argv[0], argv, NULL , work_dir, FALSE, FALSE, FALSE);
#endif
    debug_print("child_pid = %d\n", lterminal->child_pid);
    lcrt_statusbar_set_user(lterminal->parent->parent->w_statusbar, lterminal->user);
    debug_where();
    return LCRTE_OK;

}

void lcrt_ssh_disconnect(struct lcrt_terminal *lterminal)
{
    if (lterminal->connected) {
        vte_terminal_feed_child(lterminal->terminal, 
                            LCRT_TERMINAL_EXIT_CMD, 
                            strlen(LCRT_TERMINAL_EXIT_CMD));
        lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_DISCONNECT);
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
    }
    if (lterminal->private_data) {
        free(lterminal->private_data);
        lterminal->private_data = NULL;
    }
}
static void __lcrt_set_authentication_func(
    GtkCellRendererToggle *cell, 
    const gchar *path_str,
    gpointer user_data)
{
    struct lcrt_qconnect *lqconnect = (struct lcrt_qconnect *)user_data;
    struct lcrt_ssh_if *lssh = (struct lcrt_ssh_if *)lqconnect->private_data;
    GtkTreePath *path;
    GtkTreeIter iter;
    struct __lcrt_ssh_auth *lcrt_ssh_auth;
    GtkTreeModel *model = GTK_TREE_MODEL(lssh->authliststore);
  
    path = gtk_tree_path_new_from_string (path_str);
    gtk_tree_model_get_iter (model, &iter, path);
  
    gtk_tree_model_get (model, &iter, 0, &lcrt_ssh_auth, -1);
    //change record state
    lcrt_ssh_auth->auth = !lcrt_ssh_auth->auth;
    debug_where();
    gtk_tree_model_row_changed (model, path, &iter);
    gtk_tree_path_free (path);
    gtk_widget_set_sensitive(lssh->properties, lcrt_ssh_auth->auth);
}
static void __lcrt_qconnect_create_ssh_authentication(
    GtkListStore *store, 
    const gchar *text, 
    struct __lcrt_ssh_auth *auth)
{
  GtkTreeIter iter;

  gtk_list_store_append (store, &iter);
  //*brecord = !*brecord;
  gtk_list_store_set (store, &iter, 0, auth, 1, text, -1);
}
static void __lcrt_qconnect_create_ssh2_authentication(struct lcrt_qconnect *lqconnect)
{
    struct lcrt_ssh_if *lssh = (struct lcrt_ssh_if *)lqconnect->private_data;
    GtkListStore *store = lssh->authliststore;
    int i;

    for (i = 0; i < LCRT_AUTH_NUMBER; i++) {
        g_lcrt_ssh_auth[i].auth = TRUE;
        g_lcrt_ssh_auth[i].user_data = lqconnect;
    }

    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_PASSWD], 
            &g_lcrt_ssh_auth[LCRT_AUTH_PASSWD]);
    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_PUBLICKEY],
            &g_lcrt_ssh_auth[LCRT_AUTH_PUBLICKEY]);
    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_KEYBOARD],
            &g_lcrt_ssh_auth[LCRT_AUTH_KEYBOARD]);
    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_GSSAPI],
            &g_lcrt_ssh_auth[LCRT_AUTH_GSSAPI]);
}
static void __lcrt_qconnect_create_ssh1_authentication(struct lcrt_qconnect *lqconnect)
{
    struct lcrt_ssh_if *lssh = (struct lcrt_ssh_if *)lqconnect->private_data;
    GtkListStore *store = lssh->authliststore;
    int i;

    for (i = 0; i < LCRT_AUTH_NUMBER; i++) {
        g_lcrt_ssh_auth[i].auth = TRUE;
        g_lcrt_ssh_auth[i].user_data = lqconnect;
    }

    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_PASSWD],
            &g_lcrt_ssh_auth[LCRT_AUTH_PASSWD]);
    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_RSA],
            &g_lcrt_ssh_auth[LCRT_AUTH_RSA]);
    __lcrt_qconnect_create_ssh_authentication(store, 
            lqconnect->config.value[LCRT_Q_CB_TIS],
            &g_lcrt_ssh_auth[LCRT_AUTH_TIS]);
}

static void __lcrt_toggled_authentication(
    GtkTreeViewColumn *tree_column, 
    GtkCellRenderer *cell,
    GtkTreeModel *model, 
    GtkTreeIter *iter, 
    gpointer user_data)
{
    gboolean *selected;

    gtk_tree_model_get (model, iter, 0, &selected, -1);
    g_object_set (cell, "active", *selected, NULL);
}
/* start functions for authentication password */
void lcrt_ssh_password_on_cancelbutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_qconnect *lqconnect = (struct lcrt_qconnect *)user_data;
    struct lcrt_ssh_if *lssh = (struct lcrt_ssh_if *)lqconnect->private_data;
    debug_where();
    gtk_widget_destroy(lssh->ext[0]);
}
static gboolean lcrt_ssh_password_on_window_delete(GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
    lcrt_ssh_password_on_cancelbutton_clicked(NULL, user_data);
    return FALSE;
}

void lcrt_ssh_password_on_okbutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_qconnect *lqconnect = (struct lcrt_qconnect *)user_data;
    struct lcrt_ssh_if *lssh = (struct lcrt_ssh_if *)lqconnect->private_data;
    struct lcrtc_user *user;
    
    if (strcmp(gtk_entry_get_text(GTK_ENTRY(lssh->ext[1])),
        gtk_entry_get_text(GTK_ENTRY(lssh->ext[2]))) != 0) {
        lcrt_message_error(lssh->ext[0], lqconnect->config.value[LCRT_Q_PPW_UNMATCH]);
        return;
    }
    if ((user = lcrt_user_find_by_name(&lqconnect->parent->u_config, lqconnect->uname)) == NULL) {
        lcrt_message_error(lssh->ext[0], "Can not find user %s.\n", lqconnect->uname);
        return;
    }
    lcrtc_user_set_data(
       user,
       NULL,
       NULL,
       LCRT_PROTOCOL_UNKNOWN,
       NULL,
       gtk_entry_get_text(GTK_ENTRY(lssh->ext[1])),
       NULL,
       -1,
       1,
       NULL,
       -1
       );
    lcrt_ssh_password_on_cancelbutton_clicked(NULL, lqconnect);
    if (lcrt_window_get_auto_save(lqconnect->parent)) {
        lcrt_user_save_one(&lqconnect->parent->u_config, user);
    }
    debug_where();
    return;

}

void lcrt_ssh_auth_password_dialog(struct lcrt_qconnect *lqconnect)
{
    GtkWidget *dialog_password;
    GtkWidget *dialog_vbox1;
    GtkWidget *label_info;
    GtkWidget *vbox1;
    GtkWidget *table1;
    GtkWidget *label_spassword;
    GtkWidget *label_cpassword;
    GtkWidget *entry_spassword;
    GtkWidget *entry_cpassword;
    GtkWidget *checkbutton_remember;
    GtkWidget *dialog_action_area1;
    GtkWidget *cancelbutton;
    GtkWidget *okbutton;
    GtkAccelGroup *accel_group;

    struct lcrt_ssh_if *lssh = (struct lcrt_ssh_if *)lqconnect->private_data;
    accel_group = gtk_accel_group_new ();

    debug_where();
    
    dialog_password = gtk_dialog_new ();
    lssh->ext[0] = dialog_password;
    gtk_window_set_transient_for(GTK_WINDOW(dialog_password), 
            GTK_WINDOW(lqconnect->parent->window));

    gtk_window_set_title (GTK_WINDOW (dialog_password), lqconnect->config.value[LCRT_Q_PTITLE]);
    gtk_window_set_modal(GTK_WINDOW(dialog_password), TRUE);
    gtk_window_set_position (GTK_WINDOW (dialog_password), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_resizable (GTK_WINDOW (dialog_password), FALSE);
    //gtk_window_set_type_hint (GTK_WINDOW (dialog_password), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_dialog_set_has_separator (GTK_DIALOG (dialog_password), FALSE);

    g_signal_connect ((gpointer) dialog_password, "delete_event",
                    G_CALLBACK (lcrt_ssh_password_on_window_delete),
                    lqconnect);

    dialog_vbox1 = GTK_DIALOG (dialog_password)->vbox;
    gtk_widget_show (dialog_vbox1);
    gtk_widget_set_size_request (dialog_vbox1, 320, 139);

    label_info = gtk_label_new (lqconnect->config.value[LCRT_Q_PPROMPT]);
    gtk_widget_show (label_info);
    gtk_box_pack_start (GTK_BOX (dialog_vbox1), label_info, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_info, 40, 30);
    gtk_misc_set_alignment (GTK_MISC (label_info), 0.5, 0.5);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

    table1 = gtk_table_new (2, 2, FALSE);
    gtk_widget_show (table1);
    gtk_box_pack_start (GTK_BOX (vbox1), table1, FALSE, FALSE, 0);

    label_spassword = gtk_label_new (lqconnect->config.value[LCRT_Q_PSTORE_PASSWORD]);
    gtk_widget_show (label_spassword);
    gtk_table_attach (GTK_TABLE (table1), label_spassword, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_widget_set_size_request (label_spassword, 140, 30);
    gtk_misc_set_alignment (GTK_MISC (label_spassword), 0, 0.5);

    entry_spassword = gtk_entry_new ();
    lssh->ext[1]= entry_spassword;
    gtk_widget_show (entry_spassword);
    gtk_table_attach (GTK_TABLE (table1), entry_spassword, 1, 2, 0, 1,
                      (GtkAttachOptions) (0),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_widget_set_size_request (entry_spassword, 160, 30);
    gtk_entry_set_visibility(GTK_ENTRY(entry_spassword),FALSE);
    gtk_entry_set_invisible_char (GTK_ENTRY (entry_spassword), 9679);

    label_cpassword = gtk_label_new (lqconnect->config.value[LCRT_Q_PCONFIRM_PASSWORD]);
    gtk_widget_show (label_cpassword);
    gtk_table_attach (GTK_TABLE (table1), label_cpassword, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_widget_set_size_request (label_cpassword, 140, 30);
    gtk_misc_set_alignment (GTK_MISC (label_cpassword), 0, 0.5);

    entry_cpassword = gtk_entry_new ();
    lssh->ext[2]= entry_cpassword;
    gtk_widget_show (entry_cpassword);
    gtk_entry_set_visibility(GTK_ENTRY(entry_cpassword),FALSE);
    gtk_table_attach (GTK_TABLE (table1), entry_cpassword, 1, 2, 1, 2,
                      (GtkAttachOptions) (0),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_widget_set_size_request (entry_cpassword, 160, 30);
    gtk_entry_set_invisible_char (GTK_ENTRY (entry_cpassword), 9679);

    dialog_action_area1 = GTK_DIALOG (dialog_password)->action_area;
    gtk_widget_show (dialog_action_area1);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

    cancelbutton = gtk_button_new_with_mnemonic (lqconnect->config.value[LCRT_Q_PCANCEL]);
    gtk_widget_show (cancelbutton);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog_password), cancelbutton, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);

    g_signal_connect ((gpointer) cancelbutton, "clicked",
                      G_CALLBACK (lcrt_ssh_password_on_cancelbutton_clicked),
                      lqconnect);

    okbutton = gtk_button_new_with_mnemonic(lqconnect->config.value[LCRT_Q_POK]);
    gtk_widget_show (okbutton);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog_password), okbutton, GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
    gtk_widget_add_accelerator(okbutton, "clicked", accel_group,
                               GDK_Return, (GdkModifierType) 0,
                               GTK_ACCEL_VISIBLE);

    g_signal_connect ((gpointer) okbutton, "clicked",
                      G_CALLBACK (lcrt_ssh_password_on_okbutton_clicked),
                      lqconnect);

    gtk_window_add_accel_group (GTK_WINDOW (dialog_password), accel_group);
    gtk_widget_show(dialog_password);
    gtk_dialog_run(GTK_DIALOG(dialog_password));

    return;
err:
    lcrt_message_error(lqconnect->q_connect, lqconnect->config.value[LCRT_Q_ERR_MSG]);
}

static void lcrt_authentication_passwd_callback(struct lcrt_qconnect *lqconnect)
{
    debug_where();
    lcrt_ssh_auth_password_dialog(lqconnect);
}
/* end functions for authentication password */

static void lcrt_authentication_publickey_callback(struct lcrt_qconnect *lqconnect)
{
    debug_where();
}

static void lcrt_authentication_ketboard_callback(struct lcrt_qconnect *lqconnect)
{
    debug_where();
}

static void lcrt_authentication_gssapi_callback(struct lcrt_qconnect *lqconnect)
{
    debug_where();
}

static void lcrt_authentication_rsa_callback(struct lcrt_qconnect *lqconnect)
{
    debug_where();
}

static void lcrt_authentication_tis_callback(struct lcrt_qconnect *lqconnect)
{
    debug_where();
}

static void lcrt_ssh_on_button_properties_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_qconnect *lqconnect = (struct lcrt_qconnect *)user_data;
    struct lcrt_ssh_if *lssh = (struct lcrt_ssh_if *)lqconnect->private_data;
    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkTreeView *treeview = GTK_TREE_VIEW (lssh->authtreeview);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);;
    char *value;
    struct __lcrt_ssh_auth *lcrt_ssh_auth;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &lcrt_ssh_auth, -1);
        if (lcrt_ssh_auth->auth == TRUE && 
            lqconnect->flag == LCRT_QCONNECT_SESSION_OPTION)
            lcrt_ssh_auth->callback(lcrt_ssh_auth->user_data);
    } 

}

void lcrt_ssh_on_authentication_selection_changed(GtkWidget *widget, gpointer user_data)
{
    struct lcrt_qconnect *lqconnect = (struct lcrt_qconnect *)user_data;
    struct lcrt_ssh_if *lssh = (struct lcrt_ssh_if *)lqconnect->private_data;
    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkTreeView *treeview = GTK_TREE_VIEW (lssh->authtreeview);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);;
    gboolean *selected;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &selected, -1);
        gtk_widget_set_sensitive(lssh->properties, *selected);
    } else {
        gtk_widget_set_sensitive(lssh->properties, FALSE);
    }
}

static void lcrt_ssh_show(struct lcrt_qconnect *lqconnect)
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
    GtkTreeSelection *selection;
    char *firewall[LCRT_FIREWALL_NUMBER] = {LCRT_FIREWALL_NAME};
    int i;
    struct lcrt_window *parent;
    const char *str_port[LCRT_PROTOCOL_NUMBER] = {LCRT_PROTOCOL_PORT};
    static struct lcrt_ssh_if slssh, *lssh = &slssh;

    memset(lssh, 0, sizeof(struct lcrt_ssh_if));
    lqconnect->private_data = lssh;

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
    lssh->hostname = entry_hostname;
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
    lssh->username = entry_username;
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
    lssh->firewall = combobox_firewall;
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
    lssh->port = entry_port;
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

    /* we should create button_properties before tree_view because 
     * we should change the status of button_properties by tree_view.
     */
    button_properties = gtk_button_new_with_mnemonic (lqconnect->config.value[LCRT_Q_BT_PROPERTIES]);
    lssh->properties= button_properties;
    gtk_widget_show (button_properties);
    gtk_widget_set_size_request (button_properties, 80, 30);
    gtk_widget_set_sensitive(button_properties, FALSE);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_size_request (scrolledwindow, 200, -1);
    gtk_widget_show (scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox_frame), scrolledwindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), 
              GTK_POLICY_NEVER, 
              GTK_POLICY_ALWAYS);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    list_store = gtk_list_store_new (2, GTK_TYPE_POINTER, G_TYPE_STRING);
    lssh->authliststore = list_store;

    tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list_store));
    lssh->authtreeview = tree_view;
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (tree_view), FALSE);
    gtk_widget_show (tree_view);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), tree_view);

    cell = gtk_cell_renderer_toggle_new ();
    g_signal_connect (cell, "toggled", 
            G_CALLBACK (__lcrt_set_authentication_func), lqconnect);
    gtk_tree_view_insert_column_with_data_func (GTK_TREE_VIEW (tree_view),
                            -1, "record", cell,
                            __lcrt_toggled_authentication, NULL, NULL);

    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (tree_view),
                             -1, "text", gtk_cell_renderer_text_new (),
                             "text", 1, NULL);

    g_object_unref (list_store);

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    g_signal_connect((gpointer)selection, "changed", 
                     G_CALLBACK(lcrt_ssh_on_authentication_selection_changed), 
                     lqconnect);

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

    gtk_box_pack_start (GTK_BOX (vbox2), button_properties, FALSE, FALSE, 0);
    g_signal_connect ((gpointer) button_properties, "clicked",
                      G_CALLBACK (lcrt_ssh_on_button_properties_clicked),
                      lqconnect);

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
            gtk_window_set_focus(GTK_WINDOW(lqconnect->q_connect), lssh->hostname);
            gtk_entry_set_text(GTK_ENTRY(lssh->hostname), user->hostname);
            gtk_entry_set_text(GTK_ENTRY(lssh->username), user->username);
            sprintf(tmp, "%d", user->port);
            gtk_entry_set_text(GTK_ENTRY(lssh->port), tmp);
            gtk_entry_set_text(GTK_ENTRY(lqconnect->q_et_default_command), user->command);
        }
    } else {
        gtk_window_set_focus(GTK_WINDOW(lqconnect->q_connect), lssh->hostname);
    	gtk_widget_set_sensitive(lqconnect->q_bt_connect, FALSE);
    }
    gtk_entry_set_text(GTK_ENTRY(lssh->port), str_port[lqconnect->nproto]);

}

static struct lcrtc_user *lcrt_ssh_create(struct lcrt_qconnect *lqconnect)
{
    lcrt_protocol_t protocol = lqconnect->nproto;
    struct lcrt_window *lwindow = lqconnect->parent;
    struct lcrt_ssh_if *lssh = (struct lcrt_ssh_if *)lqconnect->private_data;
    const char *hostname;
    struct lcrtc_user *user;
    int i = 1;
    char name[HOSTNAME_LEN + 256];
    debug_where();
    hostname = gtk_entry_get_text(GTK_ENTRY(lssh->hostname));
    
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
           gtk_entry_get_text(GTK_ENTRY(lssh->username)),
           NULL,
           gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
           atoi(gtk_entry_get_text(GTK_ENTRY(lssh->port))),
           TRUE,
           lqconnect->folder,
           0
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
               gtk_entry_get_text(GTK_ENTRY(lssh->username)),
               NULL,
               gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
               atoi(gtk_entry_get_text(GTK_ENTRY(lssh->port))),
               TRUE,
               NULL,
               -1
            );
        }
    }
    lcrtc_user_dump(user, __func__);
    return user;
}

void lcrt_ssh_username_changed(struct lcrt_terminal *lterminal, int change)
{
    struct lcrt_ssh_tm *tssh = (struct lcrt_ssh_tm *)lterminal->private_data;
    tssh->username_changed = change;
}
struct lcrt_protocol_callback lcrt_protocol_ssh2_callbacks = {
    .protocol  =  LCRT_PROTOCOL_SSH2,
    .receive    = lcrt_ssh_receive,
    .connect    = lcrt_ssh_connect,
    .disconnect = lcrt_ssh_disconnect,
    .show       = lcrt_ssh_show,
    .create     = lcrt_ssh_create,
    .changed    = lcrt_ssh_username_changed,
};

struct lcrt_protocol_callback lcrt_protocol_ssh1_callbacks = {
    .protocol   = LCRT_PROTOCOL_SSH1,
    .receive    = lcrt_ssh_receive,
    .connect    = lcrt_ssh_connect,
    .disconnect = lcrt_ssh_disconnect,
    .show       = lcrt_ssh_show,
    .create     = lcrt_ssh_create,
    .changed    = lcrt_ssh_username_changed,
};

