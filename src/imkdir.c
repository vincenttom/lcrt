/**
 * @file imkdir.c
 * <h2>License</h2>
 * Copyright (c) 2012 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * @author Niu Tao <niutao0602@gmail.com>
 * @version $Id
 * @date Sun 29 Apr 2012 12:04:41 AM CST
 *
 * @brief  
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
#include "iconnect.h"
#include "debug.h"
#include "message.h"
#include "iterminal.h"
#include "imkdir.h"
#include "cmkdir.h"

int lcrt_mkdir_create_config(struct lcrt_mkdir *lmkdir);
int lcrt_mkdir_load_config(struct lcrt_mkdir *lmkdir);
int lcrt_mkdir_init_config(struct lcrt_mkdir *lmkdir);

struct lcrt_mkdir *lcrt_create_dialog_mkdir(
        struct lcrt_connect *parent,
        GtkWindow *window)
{
    struct lcrt_mkdir *lmkdir;
    GtkWidget *dialog_mkdir;
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

    lmkdir =(struct lcrt_mkdir *)calloc(1, sizeof(struct lcrt_mkdir));
    if (lmkdir == NULL)
        goto err;

    lcrt_mkdir_init_config(lmkdir);
    lcrt_mkdir_load_config(lmkdir);
    lmkdir->parent = parent;

    dialog_mkdir = gtk_dialog_new();
    lmkdir->dialog = dialog_mkdir;

    gtk_widget_set_size_request(dialog_mkdir, 240, 105);
    gtk_window_set_transient_for(GTK_WINDOW(dialog_mkdir), window);
    gtk_window_set_modal (GTK_WINDOW (dialog_mkdir), TRUE);
    gtk_window_set_title (GTK_WINDOW (dialog_mkdir), lmkdir->config.value[LCRT_M_TITLE]);
    gtk_window_set_position (GTK_WINDOW (dialog_mkdir), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_resizable (GTK_WINDOW (dialog_mkdir), FALSE);
    gtk_window_set_type_hint (GTK_WINDOW (dialog_mkdir), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_dialog_set_has_separator (GTK_DIALOG (dialog_mkdir), FALSE);

    dialog_vbox = GTK_DIALOG(dialog_mkdir)->vbox;
    gtk_widget_show(dialog_vbox);
    gtk_widget_set_size_request(dialog_vbox, 190, 105);
  
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox);
    gtk_box_pack_start(GTK_BOX(dialog_vbox), vbox, TRUE, TRUE, 0);
  
    label = gtk_label_new(lmkdir->config.value[LCRT_M_TIP]);
    gtk_widget_show(label);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    gtk_widget_set_size_request(label, -1, 30);
    gtk_misc_set_alignment(GTK_MISC(label), 0.01, 0.5);
  
    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), lmkdir->mkdir);
    lmkdir->entry_mkdir = entry;
    gtk_widget_show(entry);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_widget_set_size_request(entry, -1, 30);
    gtk_entry_set_invisible_char(GTK_ENTRY(entry), 9679);
    GTK_WIDGET_SET_FLAGS (entry, GTK_CAN_DEFAULT);
  
    dialog_action_area = GTK_DIALOG(dialog_mkdir)->action_area;
    gtk_widget_show(dialog_action_area);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area), GTK_BUTTONBOX_END);
  
    cancelbutton = gtk_button_new();
    gtk_widget_show(cancelbutton);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_mkdir), cancelbutton, GTK_RESPONSE_CANCEL);
  
    alignment1 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment1);
    gtk_container_add(GTK_CONTAINER(cancelbutton), alignment1);
  
    hbox1 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox1);
    gtk_container_add(GTK_CONTAINER(alignment1), hbox1);
  
    image1 = gtk_image_new_from_stock("gtk-cancel", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image1);
    gtk_box_pack_start(GTK_BOX(hbox1), image1, FALSE, FALSE, 0);
  
    label2 = gtk_label_new_with_mnemonic(lmkdir->config.value[LCRT_M_CANCEL]);
    gtk_widget_show(label2);
    gtk_box_pack_start(GTK_BOX(hbox1), label2, FALSE, FALSE, 0);
  
    okbutton = gtk_button_new();
    lmkdir->okbutton = okbutton;
    gtk_widget_show(okbutton);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_mkdir), okbutton, GTK_RESPONSE_OK);
  
    alignment2 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment2);
    gtk_container_add(GTK_CONTAINER(okbutton), alignment2);
  
    hbox2 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox2);
    gtk_container_add(GTK_CONTAINER(alignment2), hbox2);
  
    image2 = gtk_image_new_from_stock("gtk-ok", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image2);
    gtk_box_pack_start(GTK_BOX(hbox2), image2, FALSE, FALSE, 0);
  
    label3 = gtk_label_new_with_mnemonic(lmkdir->config.value[LCRT_M_OK]);
    gtk_widget_show(label3);
    gtk_box_pack_start(GTK_BOX(hbox2), label3, FALSE, FALSE, 0);
  
    g_signal_connect((gpointer)dialog_mkdir, "delete_event",
                      G_CALLBACK(lcrt_mkdir_on_delete_event),
                      lmkdir);
    g_signal_connect((gpointer)cancelbutton, "clicked",
                      G_CALLBACK(lcrt_mkdir_on_cancelbutton_clicked),
                      lmkdir);
    g_signal_connect((gpointer)okbutton, "clicked",
                        G_CALLBACK(lcrt_mkdir_on_okbutton_clicked),
                        lmkdir);
    g_signal_connect ((gpointer) entry, "changed",
                      G_CALLBACK (lcrt_mkdir_on_name_changed),
                      lmkdir);
    gtk_dialog_run(GTK_DIALOG(dialog_mkdir));

    return lmkdir;
err:
    lcrt_message_error(parent->parent->window, lmkdir->config.value[LCRT_M_ERR_MSG]);
    return NULL;

}
void lcrt_destroy_mkdir(struct lcrt_mkdir *lmkdir)
{
    if (lmkdir)
        free(lmkdir);
}
static const char *lcrt_mkdir_get_db_name(struct lcrt_mkdir *lmkdir)
{
    return lmkdir->config.db.db_name;
}
static const char *lcrt_mkdir_get_tb_name(struct lcrt_mkdir *lmkdir)
{
    return lmkdir->config.db.db_table;
}

int lcrt_mkdir_init_config(struct lcrt_mkdir *lmkdir)
{
    char db_name[256];
    int i;
    static char *name[LCRT_M_NUMBER] = {LCRT_M_NAME};
    static char *value[LCRT_M_NUMBER] = {LCRT_M_VALUE};

    assert(lmkdir);

    memset(&lmkdir->config, 0, sizeof(struct lcrtc_mkdir));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lmkdir->config.db, db_name, LCRT_IMKDIR_TABLE);

    for (i = 0; i < LCRT_M_NUMBER; i++) {
        lmkdir->config.name[i] = name[i];
        strncpy(lmkdir->config.value[i], value[i], sizeof(lmkdir->config.value[i]));
    }
    lmkdir->get_db = lcrt_mkdir_get_db_name;
    lmkdir->get_tb = lcrt_mkdir_get_tb_name;

    return 0;
}
int lcrt_mkdir_load_config(struct lcrt_mkdir *lmkdir)
{
    int rv, i;

    assert(lmkdir);

    rv = lmkdir->config.db.select(&lmkdir->config.db, "SELECT * FROM %s", lmkdir->get_tb(lmkdir));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_mkdir_create_config(lmkdir);
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_M_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lmkdir->config.value[i], lmkdir->config.db.get_text_col(&lmkdir->config.db, 1), 
                sizeof(lmkdir->config.value[i]));
        rv = lmkdir->config.db.get_row(&lmkdir->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s]\n",
                basename((char *)lmkdir->get_db(lmkdir)),
                lmkdir->get_tb(lmkdir),
                lmkdir->config.name[i],
                lmkdir->config.value[i]);
    }

    return rv;
}
int lcrt_mkdir_create_config(struct lcrt_mkdir *lmkdir)
{
    int i;

    assert(lmkdir);
    lmkdir->config.db.exec(&lmkdir->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255))",
                         lmkdir->get_tb(lmkdir));
    for (i = 0; i < LCRT_M_NUMBER; i++) {
        lmkdir->config.db.exec(&lmkdir->config.db,
                             "INSERT INTO %s VALUES('%s', '%s')",
                             lmkdir->get_tb(lmkdir),
                             lmkdir->config.name[i], 
                             lmkdir->config.value[i]);
    }
    lmkdir->config.db.close(&lmkdir->config.db);
    return 0;
}

