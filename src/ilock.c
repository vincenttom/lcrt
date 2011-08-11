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
#include "ilock.h"
#include "clock.h"

int lcrt_lock_create_config(struct lcrt_lock *llock);
int lcrt_lock_load_config(struct lcrt_lock *llock);
int lcrt_lock_init_config(struct lcrt_lock *llock);

struct lcrt_lock *lcrt_create_dialog_lock(struct lcrt_terminal *parent)
{
    struct lcrt_lock *llock;

    GtkWidget *dialog_lock;
    GtkWidget *dialog_vbox;
    GtkWidget *vbox;
    GtkWidget *label_tip;
    GtkWidget *hbox1;
    GtkWidget *label_password;
    GtkWidget *entry_password;
    GtkWidget *hbox2;
    GtkWidget *label_repassword;
    GtkWidget *entry_repassword;
    GtkWidget *dialog_action_area;
    GtkWidget *cancelbutton;
    GtkWidget *alignment1;
    GtkWidget *hbox3;
    GtkWidget *image1;
    GtkWidget *label4;
    GtkWidget *okbutton;
    GtkWidget *alignment2;
    GtkWidget *hbox4;
    GtkWidget *image2;
    GtkWidget *label5;
    GtkAccelGroup *accel_group;
    int title, tip;

    title = parent->locked == FALSE ? LCRT_K_LOCK_TITLE : LCRT_K_UNLOCK_TITLE;
    tip   = parent->locked == FALSE ? LCRT_K_LOCK_TIP : LCRT_K_UNLOCK_TIP;

    accel_group = gtk_accel_group_new ();

    llock =(struct lcrt_lock *)calloc(1, sizeof(struct lcrt_lock));
    if (llock == NULL)
        goto err;

    lcrt_lock_init_config(llock);
    lcrt_lock_load_config(llock);
    llock->parent = parent;

    dialog_lock = gtk_dialog_new();
    llock->dialog = dialog_lock;

    gtk_widget_set_size_request(dialog_lock, 300, 155);
    gtk_window_set_transient_for(GTK_WINDOW(dialog_lock), 
            GTK_WINDOW(llock->parent->parent->parent->window));
    gtk_window_set_title (GTK_WINDOW (dialog_lock), llock->config.value[title]);
    gtk_window_set_position (GTK_WINDOW (dialog_lock), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_resizable (GTK_WINDOW (dialog_lock), FALSE);
    gtk_window_set_type_hint (GTK_WINDOW (dialog_lock), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_dialog_set_has_separator (GTK_DIALOG (dialog_lock), FALSE);

    g_signal_connect ((gpointer) dialog_lock, "delete_event",
                    G_CALLBACK (lcrt_lock_on_window_delete),
                    llock);


    dialog_vbox = GTK_DIALOG(dialog_lock)->vbox;
    gtk_widget_show(dialog_vbox);

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox);
    gtk_box_pack_start(GTK_BOX(dialog_vbox), vbox, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 2);

    label_tip = gtk_label_new(llock->config.value[tip]);
    gtk_widget_show(label_tip);
    gtk_box_pack_start(GTK_BOX(vbox), label_tip, FALSE, FALSE, 0);
    gtk_widget_set_size_request(label_tip, 285, 48);
    gtk_misc_set_alignment(GTK_MISC(label_tip), 0, 0.5);

    hbox1 = gtk_hbox_new(FALSE, 0);
    gtk_widget_show(hbox1);
    gtk_box_pack_start(GTK_BOX(vbox), hbox1, TRUE, TRUE, 0);

    label_password = gtk_label_new(llock->config.value[LCRT_K_PASSWORD]);
    gtk_widget_show(label_password);
    gtk_box_pack_start(GTK_BOX(hbox1), label_password, FALSE, FALSE, 0);
    gtk_widget_set_size_request(label_password, 115, -1);
    gtk_misc_set_alignment(GTK_MISC(label_password), 0.01, 0.5);

    entry_password = gtk_entry_new();
    llock->entry_password = entry_password;
    gtk_widget_show(entry_password);
    gtk_box_pack_start(GTK_BOX(hbox1), entry_password, TRUE, TRUE, 0);
    gtk_entry_set_visibility(GTK_ENTRY(entry_password), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(entry_password), 9679);
    gtk_entry_set_max_length (GTK_ENTRY (entry_password), LCRT_PASSWORD_LEN);

    hbox2 = gtk_hbox_new(FALSE, 0);
    gtk_widget_show(hbox2);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, TRUE, TRUE, 0);
    if (parent->locked == FALSE) {
        label_repassword = gtk_label_new(llock->config.value[LCRT_K_REPASSWORD]);
        gtk_widget_show(label_repassword);
        gtk_box_pack_start(GTK_BOX(hbox2), label_repassword, FALSE, FALSE, 0);
        gtk_widget_set_size_request(label_repassword, 115, -1);
        gtk_misc_set_alignment(GTK_MISC(label_repassword), 0.01, 0.5);
    
        entry_repassword = gtk_entry_new();
        llock->entry_repassword = entry_repassword;
        gtk_widget_show(entry_repassword);
        gtk_box_pack_start(GTK_BOX(hbox2), entry_repassword, TRUE, TRUE, 0);
        gtk_entry_set_visibility(GTK_ENTRY(entry_repassword), FALSE);
        gtk_entry_set_invisible_char(GTK_ENTRY(entry_repassword), 9679);
        gtk_entry_set_max_length (GTK_ENTRY (entry_repassword), LCRT_PASSWORD_LEN);
    }

    dialog_action_area = GTK_DIALOG(dialog_lock)->action_area;
    gtk_widget_show(dialog_action_area);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area), GTK_BUTTONBOX_END);

    cancelbutton = gtk_button_new();
    gtk_widget_show(cancelbutton);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_lock), cancelbutton, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(cancelbutton, GTK_CAN_DEFAULT);

    alignment1 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment1);
    gtk_container_add(GTK_CONTAINER(cancelbutton), alignment1);

    hbox3 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox3);
    gtk_container_add(GTK_CONTAINER(alignment1), hbox3);

    image1 = gtk_image_new_from_stock("gtk-cancel", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image1);
    gtk_box_pack_start(GTK_BOX(hbox3), image1, FALSE, FALSE, 0);

    label4 = gtk_label_new_with_mnemonic(llock->config.value[LCRT_K_CANCEL]);
    gtk_widget_show(label4);
    gtk_box_pack_start(GTK_BOX(hbox3), label4, FALSE, FALSE, 0);

    okbutton = gtk_button_new();
    gtk_widget_show(okbutton);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_lock), okbutton, GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(okbutton, GTK_CAN_DEFAULT);
    gtk_widget_add_accelerator(okbutton, "clicked", accel_group,
                               GDK_Return, (GdkModifierType) 0,
                               GTK_ACCEL_VISIBLE);

    alignment2 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment2);
    gtk_container_add(GTK_CONTAINER(okbutton), alignment2);

    hbox4 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox4);
    gtk_container_add(GTK_CONTAINER(alignment2), hbox4);

    image2 = gtk_image_new_from_stock("gtk-ok", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image2);
    gtk_box_pack_start(GTK_BOX(hbox4), image2, FALSE, FALSE, 0);

    label5 = gtk_label_new_with_mnemonic(llock->config.value[LCRT_K_OK]);
    gtk_widget_show(label5);
    gtk_box_pack_start(GTK_BOX(hbox4), label5, FALSE, FALSE, 0);

    g_signal_connect((gpointer)cancelbutton, "clicked",
                      G_CALLBACK(lcrt_lock_on_cancelbutton_clicked),
                      llock);
    g_signal_connect((gpointer)okbutton, "clicked",
                      G_CALLBACK(lcrt_lock_on_okbutton_clicked),
                      llock);

    gtk_dialog_run(GTK_DIALOG(dialog_lock));

    return llock;
err:
    lcrt_message_error(parent->parent->parent->window, llock->config.value[LCRT_K_ERR_MSG]);
    return NULL;

}
void lcrt_destroy_lock(struct lcrt_lock *llock)
{
    if (llock)
        free(llock);
}
static const char *lcrt_lock_get_db_name(struct lcrt_lock *llock)
{
    return llock->config.db.db_name;
}
static const char *lcrt_lock_get_tb_name(struct lcrt_lock *llock)
{
    return llock->config.db.db_table;
}

int lcrt_lock_init_config(struct lcrt_lock *llock)
{
    char db_name[256];
    int i;
    static char *name[LCRT_K_NUMBER] = {LCRT_K_NAME};
    static char *value[LCRT_K_NUMBER] = {LCRT_K_VALUE};

    assert(llock);

    memset(&llock->config, 0, sizeof(struct lcrtc_lock));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&llock->config.db, db_name, LCRT_ILOCK_TABLE);

    for (i = 0; i < LCRT_K_NUMBER; i++) {
        llock->config.name[i] = name[i];
        strncpy(llock->config.value[i], value[i], sizeof(llock->config.value[i]));
    }
    llock->get_db = lcrt_lock_get_db_name;
    llock->get_tb = lcrt_lock_get_tb_name;

    return 0;
}
int lcrt_lock_load_config(struct lcrt_lock *llock)
{
    int rv, i;

    assert(llock);

    rv = llock->config.db.select(&llock->config.db, "SELECT * FROM %s", llock->get_tb(llock));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_lock_create_config(llock);
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_K_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(llock->config.value[i], llock->config.db.get_text_col(&llock->config.db, 1), 
                sizeof(llock->config.value[i]));
        rv = llock->config.db.get_row(&llock->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s]\n",
                basename((char *)llock->get_db(llock)),
                llock->get_tb(llock),
                llock->config.name[i],
                llock->config.value[i]);
    }

    return rv;
}
int lcrt_lock_create_config(struct lcrt_lock *llock)
{
    int i;

    assert(llock);
    llock->config.db.exec(&llock->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255))",
                         llock->get_tb(llock));
    for (i = 0; i < LCRT_K_NUMBER; i++) {
        llock->config.db.exec(&llock->config.db,
                             "INSERT INTO %s VALUES('%s', '%s')",
                             llock->get_tb(llock),
                             llock->config.name[i], 
                             llock->config.value[i]);
    }
    llock->config.db.close(&llock->config.db);
    return 0;
}

