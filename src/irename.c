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
#include "irename.h"
#include "crename.h"

int lcrt_rename_create_config(struct lcrt_rename *lrename);
int lcrt_rename_load_config(struct lcrt_rename *lrename);
int lcrt_rename_init_config(struct lcrt_rename *lrename);

struct lcrt_rename *lcrt_create_dialog_rename(
        struct lcrt_window *parent,
        GtkWindow *window,
        const char *name,
        gboolean rename_terminal)
{
    struct lcrt_rename *lrename;
    GtkWidget *dialog_rename;
    GtkWidget *dialog_vbox;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *dialog_action_area;
    GtkWidget *cancelbutton;
    GtkWidget *alignment1;
    GtkWidget *hbox1;
    GtkWidget *image1;
    GtkWidget *label2;
    GtkWidget *okbutton;
    GtkWidget *alignment2;
    GtkWidget *hbox2;
    GtkWidget *image2;
    GtkWidget *label3;

    lrename =(struct lcrt_rename *)calloc(1, sizeof(struct lcrt_rename));
    if (lrename == NULL)
        goto err;

    lcrt_rename_init_config(lrename);
    lcrt_rename_load_config(lrename);
    lrename->parent = parent;
    lrename->rename_terminal = rename_terminal;

    if (rename_terminal == TRUE)
        strncpy(lrename->rename, 
                gtk_label_get_text(GTK_LABEL(parent->w_notebook->current_terminal->label)), 
                sizeof(lrename->rename));
    else
        strncpy(lrename->rename,
                name,
                sizeof(lrename->rename));
    dialog_rename = gtk_dialog_new();
    lrename->dialog = dialog_rename;

    gtk_widget_set_size_request(dialog_rename, 240, 105);
    gtk_window_set_transient_for(GTK_WINDOW(dialog_rename), 
            (window != NULL ? window : GTK_WINDOW(parent->window)));
    gtk_window_set_modal (GTK_WINDOW (dialog_rename), TRUE);
    gtk_window_set_title (GTK_WINDOW (dialog_rename), lrename->config.value[LCRT_R_TITLE]);
    gtk_window_set_position (GTK_WINDOW (dialog_rename), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_resizable (GTK_WINDOW (dialog_rename), FALSE);
    gtk_window_set_type_hint (GTK_WINDOW (dialog_rename), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_dialog_set_has_separator (GTK_DIALOG (dialog_rename), FALSE);

    dialog_vbox = GTK_DIALOG(dialog_rename)->vbox;
    gtk_widget_show(dialog_vbox);
    gtk_widget_set_size_request(dialog_vbox, 190, 105);
  
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox);
    gtk_box_pack_start(GTK_BOX(dialog_vbox), vbox, TRUE, TRUE, 0);
  
    label = gtk_label_new(lrename->config.value[LCRT_R_TIP]);
    gtk_widget_show(label);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    gtk_widget_set_size_request(label, -1, 30);
    gtk_misc_set_alignment(GTK_MISC(label), 0.01, 0.5);
  
    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), lrename->rename);
    lrename->entry_rename = entry;
    gtk_widget_show(entry);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_widget_set_size_request(entry, -1, 30);
    gtk_entry_set_invisible_char(GTK_ENTRY(entry), 9679);
    GTK_WIDGET_SET_FLAGS (entry, GTK_CAN_DEFAULT);
  
    dialog_action_area = GTK_DIALOG(dialog_rename)->action_area;
    gtk_widget_show(dialog_action_area);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area), GTK_BUTTONBOX_END);
  
    cancelbutton = gtk_button_new();
    gtk_widget_show(cancelbutton);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_rename), cancelbutton, GTK_RESPONSE_CANCEL);
  
    alignment1 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment1);
    gtk_container_add(GTK_CONTAINER(cancelbutton), alignment1);
  
    hbox1 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox1);
    gtk_container_add(GTK_CONTAINER(alignment1), hbox1);
  
    image1 = gtk_image_new_from_stock("gtk-cancel", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image1);
    gtk_box_pack_start(GTK_BOX(hbox1), image1, FALSE, FALSE, 0);
  
    label2 = gtk_label_new_with_mnemonic(lrename->config.value[LCRT_R_CANCEL]);
    gtk_widget_show(label2);
    gtk_box_pack_start(GTK_BOX(hbox1), label2, FALSE, FALSE, 0);
  
    okbutton = gtk_button_new();
    lrename->okbutton = okbutton;
    gtk_widget_show(okbutton);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_rename), okbutton, GTK_RESPONSE_OK);
  
    alignment2 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment2);
    gtk_container_add(GTK_CONTAINER(okbutton), alignment2);
  
    hbox2 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox2);
    gtk_container_add(GTK_CONTAINER(alignment2), hbox2);
  
    image2 = gtk_image_new_from_stock("gtk-ok", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image2);
    gtk_box_pack_start(GTK_BOX(hbox2), image2, FALSE, FALSE, 0);
  
    label3 = gtk_label_new_with_mnemonic(lrename->config.value[LCRT_R_OK]);
    gtk_widget_show(label3);
    gtk_box_pack_start(GTK_BOX(hbox2), label3, FALSE, FALSE, 0);
  
    g_signal_connect((gpointer)dialog_rename, "delete_event",
                      G_CALLBACK(lcrt_rename_on_delete_event),
                      lrename);
    g_signal_connect((gpointer)cancelbutton, "clicked",
                      G_CALLBACK(lcrt_rename_on_cancelbutton_clicked),
                      lrename);
    g_signal_connect((gpointer)okbutton, "clicked",
                        G_CALLBACK(lcrt_rename_on_okbutton_clicked),
                        lrename);
    g_signal_connect ((gpointer) entry, "changed",
                      G_CALLBACK (lcrt_rename_on_name_changed),
                      lrename);
    gtk_dialog_run(GTK_DIALOG(dialog_rename));

    return lrename;
err:
    lcrt_message_error(parent->window, lrename->config.value[LCRT_R_ERR_MSG]);
    return NULL;

}
void lcrt_destroy_rename(struct lcrt_rename *lrename)
{
    if (lrename)
        free(lrename);
}
static const char *lcrt_rename_get_db_name(struct lcrt_rename *lrename)
{
    return lrename->config.db.db_name;
}
static const char *lcrt_rename_get_tb_name(struct lcrt_rename *lrename)
{
    return lrename->config.db.db_table;
}

int lcrt_rename_init_config(struct lcrt_rename *lrename)
{
    char db_name[256];
    int i;
    static char *name[LCRT_R_NUMBER] = {LCRT_R_NAME};
    static char *value[LCRT_R_NUMBER] = {LCRT_R_VALUE};

    assert(lrename);

    memset(&lrename->config, 0, sizeof(struct lcrtc_rename));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lrename->config.db, db_name, LCRT_IRENAME_TABLE);

    for (i = 0; i < LCRT_R_NUMBER; i++) {
        lrename->config.name[i] = name[i];
        strncpy(lrename->config.value[i], value[i], sizeof(lrename->config.value[i]));
    }
    lrename->get_db = lcrt_rename_get_db_name;
    lrename->get_tb = lcrt_rename_get_tb_name;

    return 0;
}
int lcrt_rename_load_config(struct lcrt_rename *lrename)
{
    int rv, i;

    assert(lrename);

    rv = lrename->config.db.select(&lrename->config.db, "SELECT * FROM %s", lrename->get_tb(lrename));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_rename_create_config(lrename);
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_R_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lrename->config.value[i], lrename->config.db.get_text_col(&lrename->config.db, 1), 
                sizeof(lrename->config.value[i]));
        rv = lrename->config.db.get_row(&lrename->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s]\n",
                basename((char *)lrename->get_db(lrename)),
                lrename->get_tb(lrename),
                lrename->config.name[i],
                lrename->config.value[i]);
    }

    return rv;
}
int lcrt_rename_create_config(struct lcrt_rename *lrename)
{
    int i;

    assert(lrename);
    lrename->config.db.exec(&lrename->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255))",
                         lrename->get_tb(lrename));
    for (i = 0; i < LCRT_R_NUMBER; i++) {
        lrename->config.db.exec(&lrename->config.db,
                             "INSERT INTO %s VALUES('%s', '%s')",
                             lrename->get_tb(lrename),
                             lrename->config.name[i], 
                             lrename->config.value[i]);
    }
    lrename->config.db.close(&lrename->config.db);
    return 0;
}

