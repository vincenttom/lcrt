#define __LCRT_DEBUG__
#include "debug.h"
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
#include "iqconnect.h"
#include "debug.h"
#include "message.h"
#include "cqconnect.h"
#include "serial.h"

int lcrt_qconnect_init_config(struct lcrt_qconnect *lqconnect);
int lcrt_qconnect_load_config(struct lcrt_qconnect *lqconnect);
int lcrt_qconnect_create_config(struct lcrt_qconnect *lqconnect);

int lcrt_create_qconnect(
    struct lcrt_window *parent, 
    GtkWindow *window, 
    lcrt_qconnect_flag_t flag, 
    const char *uname)
{
    struct lcrt_qconnect *lqconnect;

    debug_where();
    GtkWidget *quick_connect;
    GtkWidget *vbox;
    GtkWidget *hbox1;
    GtkWidget *label_protocol;
    GtkWidget *combobox_protocol;
    GtkWidget *label_default_command;
    GtkWidget *entry_default_command;
    GtkWidget *hbox9;
    GtkWidget *checkbutton_show_quick_connect_on_startup;
    GtkWidget *hbox6;
    GtkWidget *fixed2;
    GtkWidget *checkbutton_save_session;
    GtkWidget *checkbutton_open_in_a_tab;
    GtkWidget *dialog_action_area;
    GtkWidget *button_connect;
    GtkWidget *button_cancel;
    GtkAccelGroup *accel_group;
    GtkWidget *hbox7;
    GtkWidget *hbox8;
    GtkWidget *fixed1;
    int i;
    static const char *proto[LCRT_PROTOCOL_NUMBER] = {LCRT_PROTOCOL_NAME};
    debug_where();
    lqconnect = (struct lcrt_qconnect *)calloc(1, sizeof(struct lcrt_qconnect));
    
    if (lqconnect == NULL)
        goto err;

    lqconnect->parent = parent;
    lqconnect->flag = flag;
    lqconnect->uname = uname;
    debug_where();

    lcrt_qconnect_init_config(lqconnect);
    lcrt_qconnect_load_config(lqconnect);

    accel_group = gtk_accel_group_new ();

    quick_connect = gtk_dialog_new ();
    gtk_widget_set_size_request (quick_connect, 320, 370);
    gtk_window_set_transient_for(GTK_WINDOW(quick_connect), 
            window != NULL ? window : GTK_WINDOW(lqconnect->parent->window));
    gtk_widget_show(quick_connect);
    lqconnect->q_connect = quick_connect;
    vbox = GTK_DIALOG (lqconnect->q_connect)->vbox;
    if (flag == LCRT_QCONNECT_SESSION_OPTION)
        gtk_window_set_title (GTK_WINDOW (quick_connect), lqconnect->config.value[LCRT_Q_SESSION_OPTION]);
    else
        gtk_window_set_title (GTK_WINDOW (quick_connect), lqconnect->config.value[LCRT_Q_CONNECT]);
    gtk_window_set_modal(GTK_WINDOW(quick_connect), TRUE);
    gtk_window_set_position (GTK_WINDOW (quick_connect), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_resizable (GTK_WINDOW (quick_connect), FALSE);
    //gtk_window_set_type_hint (GTK_WINDOW (quick_connect), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_dialog_set_has_separator (GTK_DIALOG (quick_connect), FALSE);

    g_signal_connect ((gpointer) quick_connect, "delete_event",
                    G_CALLBACK (lcrt_qconnect_on_window_delete),
                    lqconnect);
    vbox = GTK_DIALOG (quick_connect)->vbox;
	gtk_widget_show(vbox);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (vbox), hbox1, TRUE, TRUE, 0);
    gtk_box_reorder_child (GTK_BOX (vbox), hbox1, 0);
    gtk_widget_set_size_request (hbox1, -1, 25);

    label_protocol = gtk_label_new (lqconnect->config.value[LCRT_Q_LB_PROTOCOL]);
    gtk_widget_show (label_protocol);
    gtk_box_pack_start (GTK_BOX (hbox1), label_protocol, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_protocol, 90, 25);
    gtk_misc_set_alignment (GTK_MISC (label_protocol), 0, 0.5);

    combobox_protocol = gtk_combo_box_entry_new_text ();
    lqconnect->q_cb_protocol = combobox_protocol;
    gtk_widget_show (combobox_protocol);
    gtk_box_pack_start (GTK_BOX (hbox1), combobox_protocol, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_protocol, SERIAL_COMBOBOX_WIDTH, 25);
    g_signal_connect ((gpointer) combobox_protocol, "changed",
                    G_CALLBACK (lcrt_qconnect_on_protocol_changed),
                    lqconnect);
    
    for (i = 0; i < LCRT_PROTOCOL_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_protocol), proto[i]);
    }
    gtk_entry_set_editable(GTK_ENTRY(GTK_BIN(combobox_protocol)->child), FALSE);
    if (flag == LCRT_QCONNECT_SESSION_OPTION)
        gtk_widget_set_sensitive(combobox_protocol, FALSE);

    //lcrt_qconnect_create_nonserial_window(lqconnect);
    //lcrt_qconnect_create_serial_window(lqconnect);

    hbox9 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox9);
    gtk_box_pack_start (GTK_BOX (vbox), hbox9, TRUE, TRUE, 0);
    gtk_box_reorder_child (GTK_BOX (vbox), hbox9, 2);
    gtk_widget_set_size_request (hbox9, -1, 30);

    label_default_command = gtk_label_new(lqconnect->config.value[LCRT_Q_LB_DEFAULT_COMMAND]);
    gtk_widget_show (label_default_command);
    gtk_box_pack_start (GTK_BOX (hbox9), label_default_command, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_default_command, 90, 30);
    gtk_misc_set_alignment (GTK_MISC (label_default_command), 0, 0.5);

    entry_default_command = gtk_entry_new_with_max_length(DEFAULT_COMMAND_LEN);
    lqconnect->q_et_default_command = entry_default_command;
    gtk_widget_show (entry_default_command);
    gtk_box_pack_start (GTK_BOX (hbox9), entry_default_command, FALSE, TRUE, 0);
    gtk_widget_set_size_request (entry_default_command, 220, 30);

    hbox7 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox7);
    gtk_box_pack_start (GTK_BOX (vbox), hbox7, FALSE, FALSE, 0);
    gtk_box_reorder_child (GTK_BOX (vbox), hbox7, 3);

    hbox8 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox8);
    gtk_box_pack_start (GTK_BOX (hbox7), hbox8, TRUE, TRUE, 0);

    fixed1 = gtk_fixed_new ();
    gtk_widget_show (fixed1);
    gtk_box_pack_start (GTK_BOX (hbox8), fixed1, FALSE, FALSE, 0);
    gtk_widget_set_size_request (fixed1, 15, -1);

    debug_where();
    checkbutton_show_quick_connect_on_startup = 
        gtk_check_button_new_with_mnemonic (lqconnect->config.value[LCRT_Q_CB_SHOW_QCONNECT]);
    lqconnect->q_cb_show_qconnect = checkbutton_show_quick_connect_on_startup;
    gtk_widget_show (checkbutton_show_quick_connect_on_startup);
    gtk_box_pack_start (GTK_BOX (hbox8), checkbutton_show_quick_connect_on_startup, FALSE, FALSE, 0);

    hbox6 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox6);
    gtk_box_pack_start (GTK_BOX (vbox), hbox6, TRUE, TRUE, 0);
    gtk_box_reorder_child (GTK_BOX (vbox), hbox6, 4);

    fixed2 = gtk_fixed_new ();
    gtk_widget_show (fixed2);
    gtk_box_pack_start (GTK_BOX (hbox6), fixed2, FALSE, FALSE, 0);
    gtk_widget_set_size_request (fixed2, 15, -1);

    checkbutton_save_session = gtk_check_button_new_with_mnemonic (lqconnect->config.value[LCRT_Q_CB_SAVE_SESSION]);
    lqconnect->q_cb_save_session = checkbutton_save_session;
    gtk_widget_show (checkbutton_save_session);
    gtk_box_pack_start (GTK_BOX (hbox6), checkbutton_save_session, FALSE, FALSE, 0);
    gtk_widget_set_size_request (checkbutton_save_session, 150, -1);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_save_session), TRUE);

    checkbutton_open_in_a_tab = gtk_check_button_new_with_mnemonic (lqconnect->config.value[LCRT_Q_CB_OPEN_IN_A_TAB]);
    lqconnect->q_cb_open_in_a_tab = checkbutton_open_in_a_tab;
    gtk_widget_show (checkbutton_open_in_a_tab);
    gtk_box_pack_start (GTK_BOX (hbox6), checkbutton_open_in_a_tab, FALSE, FALSE, 0);

    dialog_action_area = GTK_DIALOG (quick_connect)->action_area;
    gtk_widget_show (dialog_action_area);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);


    button_cancel = gtk_button_new_with_mnemonic (lqconnect->config.value[LCRT_Q_BT_CANCEL]);
    lqconnect->q_bt_cancel = button_cancel;
    gtk_widget_show (button_cancel);
    gtk_dialog_add_action_widget (GTK_DIALOG (quick_connect), button_cancel, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS (button_cancel, GTK_CAN_DEFAULT);
    g_signal_connect ((gpointer) button_cancel, "clicked",
                    G_CALLBACK (lcrt_qconnect_on_button_cancel_clicked),
                    lqconnect);
    if (flag == LCRT_QCONNECT_SESSION_OPTION)
        button_connect = gtk_button_new_with_mnemonic (lqconnect->config.value[LCRT_Q_BT_OK]);
    else
        button_connect = gtk_button_new_with_mnemonic (lqconnect->config.value[LCRT_Q_BT_CONNECT]);
    lqconnect->q_bt_connect = button_connect;
    gtk_widget_show (button_connect);
    gtk_dialog_add_action_widget (GTK_DIALOG (quick_connect), button_connect, GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS (button_connect, GTK_CAN_DEFAULT);
    g_signal_connect ((gpointer) button_connect, "clicked",
                    G_CALLBACK (lcrt_qconnect_on_button_connect_clicked),
                    lqconnect);

    gtk_widget_add_accelerator(button_connect, "clicked", accel_group,
                               GDK_Return, (GdkModifierType) 0,
                               GTK_ACCEL_VISIBLE);

    debug_where();
    gtk_window_add_accel_group (GTK_WINDOW (quick_connect), accel_group);
    /* quick connect dialog is created for modify session options */
    if (flag == LCRT_QCONNECT_SESSION_OPTION) {
        struct lcrtc_user *user;
        if ((user = lcrt_user_find_by_name(&parent->u_config, uname)) != NULL) {
            gtk_combo_box_set_active(GTK_COMBO_BOX(lqconnect->q_cb_protocol), user->protocol);
        }
    } else {
        /* by default, we select ssh2 protocol */
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_protocol), LCRT_PROTOCOL_SSH2);
    }
    debug_where();
    int ret;
    do {
        ret = gtk_dialog_run(GTK_DIALOG(quick_connect));
        debug_where();
    } while (parent->q_status == GTK_RESPONSE_CANCEL);
    return ret;
err:
    lcrt_message_error(parent->window, lqconnect->config.value[LCRT_Q_ERR_MSG]);
    return GTK_RESPONSE_DELETE_EVENT;
}

void lcrt_destroy_qconnect(struct lcrt_qconnect *lqconnect)
{
    if (lqconnect)
        free(lqconnect);
}
static const char *lcrt_qconnect_get_db_name(struct lcrt_qconnect *lqconnect)
{
    return lqconnect->config.db.db_name;
}
static const char *lcrt_qconnect_get_tb_name(struct lcrt_qconnect *lqconnect)
{
    return lqconnect->config.db.db_table;
}

int lcrt_qconnect_init_config(struct lcrt_qconnect *lqconnect)
{
    char db_name[256];
    int i;
    static char *name[LCRT_Q_NUMBER] = {LCRT_Q_NAME};
    static char *value[LCRT_Q_NUMBER] = {LCRT_Q_VALUE};
    static int shortcut[LCRT_Q_NUMBER][2] = {LCRT_Q_SHORTCUT};

    assert(lqconnect);

    memset(&lqconnect->config, 0, sizeof(struct lcrtc_qconnect));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lqconnect->config.db, db_name, LCRT_IQCONNECT_TABLE);

    for (i = 0; i < LCRT_Q_NUMBER; i++) {
        lqconnect->config.name[i] = name[i];
        strncpy(lqconnect->config.value[i], value[i], sizeof(lqconnect->config.value[i]));
        lqconnect->config.shortcut[i][0] = shortcut[i][0];
        lqconnect->config.shortcut[i][1] = shortcut[i][1];
    }
    lqconnect->get_db = lcrt_qconnect_get_db_name;
    lqconnect->get_tb = lcrt_qconnect_get_tb_name;

    return 0;
}
int lcrt_qconnect_load_config(struct lcrt_qconnect *lqconnect)
{
    int rv, i;

    assert(lqconnect);

    rv = lqconnect->config.db.select(&lqconnect->config.db, "SELECT * FROM %s", lqconnect->get_tb(lqconnect));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_qconnect_create_config(lqconnect);
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_Q_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lqconnect->config.value[i], lqconnect->config.db.get_text_col(&lqconnect->config.db, 1), 
                sizeof(lqconnect->config.value[i]));
        rv = lqconnect->config.db.get_row(&lqconnect->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s] shortcut = {%-5d, %d}\n",
                basename((char *)lqconnect->get_db(lqconnect)),
                lqconnect->get_tb(lqconnect),
                lqconnect->config.name[i],
                lqconnect->config.value[i],
                lqconnect->config.shortcut[i][0],
                lqconnect->config.shortcut[i][1]);
    }

    return rv;
}
int lcrt_qconnect_create_config(struct lcrt_qconnect *lqconnect)
{
    int i;

    assert(lqconnect);
    lqconnect->config.db.exec(&lqconnect->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255),  \
                          shortcut INTEGER, \
                          shortcut_mask INTEGER)",
                         lqconnect->get_tb(lqconnect));
    for (i = 0; i < LCRT_Q_NUMBER; i++) {
        lqconnect->config.db.exec(&lqconnect->config.db,
                             "INSERT INTO %s VALUES('%s', '%s', %d, %d)",
                             lqconnect->get_tb(lqconnect),
                             lqconnect->config.name[i], 
                             lqconnect->config.value[i],
                             lqconnect->config.shortcut[i][0],
                             lqconnect->config.shortcut[i][1]);
    }
    lqconnect->config.db.close(&lqconnect->config.db);
    return 0;
}
