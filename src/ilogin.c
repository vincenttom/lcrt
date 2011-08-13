/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sun 23 Jan 2011 02:36:22 AM CST
 *
 *
 * Description:  
 */
//#define __LCRT_DEBUG__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <libgen.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "iwindow.h"
#include "debug.h"
#include "message.h"
#include "iterminal.h"
#include "ilogin.h"
#include "clogin.h"
#include "debug.h"

int lcrt_login_init_config(struct lcrt_login *llogin);
int lcrt_login_load_config(struct lcrt_login *llogin);
int lcrt_login_create_config(struct lcrt_login *llogin);

struct lcrt_login *lcrt_create_login(struct lcrt_terminal *parent, int again)
{
    struct lcrt_login *llogin;

    GtkWidget *dialog_login;
    GtkWidget *dialog_vbox1;
    GtkWidget *label_info;
    GtkWidget *vbox1;
    GtkWidget *table1;
    GtkWidget *label_username;
    GtkWidget *label_passwd;
    GtkWidget *entry_username;
    GtkWidget *entry_password;
    GtkWidget *checkbutton_remember;
    GtkWidget *dialog_action_area1;
    GtkWidget *cancelbutton;
    GtkWidget *okbutton;
    GtkAccelGroup *accel_group;

    accel_group = gtk_accel_group_new ();

    debug_where();
    llogin = (struct lcrt_login *)calloc(1, sizeof(struct lcrt_login));
    
    if (llogin == NULL)
        goto err;

    llogin->parent = parent;
    debug_where();

    lcrt_login_init_config(llogin);
    lcrt_login_load_config(llogin);

    dialog_login = gtk_dialog_new ();
    llogin->login = dialog_login;
    gtk_window_set_transient_for(GTK_WINDOW(dialog_login), 
            GTK_WINDOW(llogin->parent->parent->parent->window));

    gtk_window_set_title (GTK_WINDOW (dialog_login), llogin->config.value[LCRT_L_LOGIN]);
    gtk_window_set_position (GTK_WINDOW (dialog_login), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_resizable (GTK_WINDOW (dialog_login), FALSE);
    gtk_window_set_type_hint (GTK_WINDOW (dialog_login), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_dialog_set_has_separator (GTK_DIALOG (dialog_login), FALSE);

    g_signal_connect ((gpointer) dialog_login, "delete_event",
                    G_CALLBACK (lcrt_login_on_window_delete),
                    llogin);

    dialog_vbox1 = GTK_DIALOG (dialog_login)->vbox;
    gtk_widget_show (dialog_vbox1);
    gtk_widget_set_size_request (dialog_vbox1, 254, 169);
    if (again == FALSE) {
        label_info = gtk_label_new (llogin->config.value[LCRT_L_INFO]);
    } else {
        label_info = gtk_label_new (llogin->config.value[LCRT_L_AGAIN]);
    }
    gtk_widget_show (label_info);
    gtk_box_pack_start (GTK_BOX (dialog_vbox1), label_info, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_info, 40, 30);
    gtk_misc_set_alignment (GTK_MISC (label_info), 0, 0.5);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

    table1 = gtk_table_new (2, 2, FALSE);
    gtk_widget_show (table1);
    gtk_box_pack_start (GTK_BOX (vbox1), table1, FALSE, FALSE, 0);

    label_username = gtk_label_new (llogin->config.value[LCRT_L_USERNAME]);
    gtk_widget_show (label_username);
    gtk_table_attach (GTK_TABLE (table1), label_username, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_widget_set_size_request (label_username, 80, 30);
    gtk_misc_set_alignment (GTK_MISC (label_username), 0, 0.5);

    entry_username = gtk_entry_new ();
    llogin->l_username = entry_username;
    gtk_widget_show (entry_username);
    gtk_table_attach (GTK_TABLE (table1), entry_username, 1, 2, 0, 1,
                      (GtkAttachOptions) (0),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_widget_set_size_request (entry_username, 160, 30);
    gtk_entry_set_invisible_char (GTK_ENTRY (entry_username), 9679);

    g_signal_connect ((gpointer) entry_username, "changed",
                      G_CALLBACK (lcrt_login_on_username_changed),
                      llogin);

    label_passwd = gtk_label_new (llogin->config.value[LCRT_L_PASSWORD]);
    gtk_widget_show (label_passwd);
    gtk_table_attach (GTK_TABLE (table1), label_passwd, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_widget_set_size_request (label_passwd, 80, 30);
    gtk_misc_set_alignment (GTK_MISC (label_passwd), 0, 0.5);

    entry_password = gtk_entry_new ();
    llogin->l_password = entry_password;
    gtk_widget_show (entry_password);
    gtk_entry_set_visibility(GTK_ENTRY(entry_password),FALSE);
    gtk_table_attach (GTK_TABLE (table1), entry_password, 1, 2, 1, 2,
                      (GtkAttachOptions) (0),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_widget_set_size_request (entry_password, 160, 30);
    gtk_entry_set_invisible_char (GTK_ENTRY (entry_password), 9679);

    g_signal_connect ((gpointer) entry_password, "changed",
                      G_CALLBACK (lcrt_login_on_password_changed),
                      llogin);

    checkbutton_remember = gtk_check_button_new_with_mnemonic (llogin->config.value[LCRT_L_REMEMBER]);
    llogin->l_remember = checkbutton_remember;
    gtk_widget_show (checkbutton_remember);
    gtk_box_pack_start (GTK_BOX (vbox1), checkbutton_remember, FALSE, FALSE, 0);

    dialog_action_area1 = GTK_DIALOG (dialog_login)->action_area;
    gtk_widget_show (dialog_action_area1);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

    cancelbutton = gtk_button_new_with_mnemonic (llogin->config.value[LCRT_L_CANCELBUTTON]);
    gtk_widget_show (cancelbutton);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog_login), cancelbutton, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);

    g_signal_connect ((gpointer) cancelbutton, "clicked",
                      G_CALLBACK (lcrt_login_on_cancelbutton_clicked),
                      llogin);

    okbutton = gtk_button_new_with_mnemonic(llogin->config.value[LCRT_L_OKBUTTON]);
    gtk_widget_show (okbutton);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog_login), okbutton, GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
    gtk_widget_add_accelerator(okbutton, "clicked", accel_group,
                               GDK_Return, (GdkModifierType) 0,
                               GTK_ACCEL_VISIBLE);

    g_signal_connect ((gpointer) okbutton, "clicked",
                      G_CALLBACK (lcrt_login_on_okbutton_clicked),
                      llogin);

    if (strlen(parent->user->username) != 0) {
        gtk_entry_set_text(GTK_ENTRY(entry_username), parent->user->username);
        gtk_window_set_focus(GTK_WINDOW(dialog_login), entry_password);
    } else {
        gtk_window_set_focus(GTK_WINDOW(dialog_login), entry_username);
    }
    gtk_window_add_accel_group (GTK_WINDOW (dialog_login), accel_group);
    gtk_widget_show(dialog_login);
    gtk_dialog_run(GTK_DIALOG(dialog_login));

    return llogin;
err:
    lcrt_message_error(parent->parent->parent->window, llogin->config.value[LCRT_L_ERR_MSG]);
    return NULL;
}
void lcrt_destroy_login(struct lcrt_login *llogin)
{
    if (llogin)
        free(llogin);
}
static const char *lcrt_login_get_db_name(struct lcrt_login *llogin)
{
    return llogin->config.db.db_name;
}
static const char *lcrt_login_get_tb_name(struct lcrt_login *llogin)
{
    return llogin->config.db.db_table;
}

int lcrt_login_init_config(struct lcrt_login *llogin)
{
    char db_name[256];
    int i;
    static char *name[LCRT_L_NUMBER] = {LCRT_L_NAME};
    static char *value[LCRT_L_NUMBER] = {LCRT_L_VALUE};
    static int shortcut[LCRT_L_NUMBER][2] = {LCRT_L_SHORTCUT};

    assert(llogin);

    memset(&llogin->config, 0, sizeof(struct lcrtc_login));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&llogin->config.db, db_name, LCRT_ILOGIN_TABLE);

    for (i = 0; i < LCRT_L_NUMBER; i++) {
        llogin->config.name[i] = name[i];
        strncpy(llogin->config.value[i], value[i], sizeof(llogin->config.value[i]));
        llogin->config.shortcut[i][0] = shortcut[i][0];
        llogin->config.shortcut[i][1] = shortcut[i][1];
    }
    llogin->get_db = lcrt_login_get_db_name;
    llogin->get_tb = lcrt_login_get_tb_name;

    return 0;
}
int lcrt_login_load_config(struct lcrt_login *llogin)
{
    int rv, i;

    assert(llogin);

    rv = llogin->config.db.select(&llogin->config.db, "SELECT * FROM %s", llogin->get_tb(llogin));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_login_create_config(llogin);
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_L_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(llogin->config.value[i], llogin->config.db.get_text_col(&llogin->config.db, 1), 
                sizeof(llogin->config.value[i]));
        rv = llogin->config.db.get_row(&llogin->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s] shortcut = {%-5d, %d}\n",
                basename((char *)llogin->get_db(llogin)),
                llogin->get_tb(llogin),
                llogin->config.name[i],
                llogin->config.value[i],
                llogin->config.shortcut[i][0],
                llogin->config.shortcut[i][1]);
    }

    return rv;
}
int lcrt_login_create_config(struct lcrt_login *llogin)
{
    int i;

    assert(llogin);
    llogin->config.db.exec(&llogin->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255),  \
                          shortcut INTEGER, \
                          shortcut_mask INTEGER)",
                         llogin->get_tb(llogin));
    for (i = 0; i < LCRT_L_NUMBER; i++) {
        llogin->config.db.exec(&llogin->config.db,
                             "INSERT INTO %s VALUES('%s', '%s', %d, %d)",
                             llogin->get_tb(llogin),
                             llogin->config.name[i], 
                             llogin->config.value[i],
                             llogin->config.shortcut[i][0],
                             llogin->config.shortcut[i][1]);
    }
    llogin->config.db.close(&llogin->config.db);
    return 0;
}
