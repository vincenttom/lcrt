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
#include "iedit.h"
#include "imenubar.h"
#include "ifind.h"
#include "cfind.h"

int lcrt_find_create_config(struct lcrt_find *lfind);
int lcrt_find_load_config(struct lcrt_find *lfind);
int lcrt_find_init_config(struct lcrt_find *lfind);

struct lcrt_find *lcrt_create_dialog_find(void *parent, int flag)
{
    struct lcrt_find *lfind;
    GtkWidget *dialog_find;
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
    struct lcrt_connect *parent_connect;
    struct lcrt_edit *parent_edit;

    lfind =(struct lcrt_find *)calloc(1, sizeof(struct lcrt_find));
    if (lfind == NULL)
        goto err;

    lcrt_find_init_config(lfind);
    lcrt_find_load_config(lfind);
    lfind->parent = parent;
    lfind->flag = flag;

    dialog_find = gtk_dialog_new();
    lfind->dialog = dialog_find;

    gtk_widget_set_size_request(dialog_find, 280, 105);
    switch (lfind->flag) {
    case LCRT_FIND_FCONNECT:
        parent_connect = (struct lcrt_connect *)lfind->parent;
	lfind->parent_window = parent_connect->c_connect;
	break;
    case LCRT_FIND_FEDIT:
        parent_edit = (struct lcrt_edit *)lfind->parent;
	lfind->parent_window = parent_connect->parent->window;
	break;
    default:
	printf("%s: unkown flag\n");
	return;
    }
    gtk_window_set_transient_for(GTK_WINDOW(dialog_find), 
			GTK_WINDOW(lfind->parent_window));

    gtk_window_set_modal (GTK_WINDOW (dialog_find), TRUE);
    gtk_window_set_title (GTK_WINDOW (dialog_find), lfind->config.value[LCRT_I_TITLE]);
    gtk_window_set_position (GTK_WINDOW (dialog_find), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_resizable (GTK_WINDOW (dialog_find), FALSE);
    gtk_window_set_type_hint (GTK_WINDOW (dialog_find), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_dialog_set_has_separator (GTK_DIALOG (dialog_find), FALSE);

    dialog_vbox = GTK_DIALOG(dialog_find)->vbox;
    gtk_widget_show(dialog_vbox);
    gtk_widget_set_size_request(dialog_vbox, 190, 105);
  
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox);
    gtk_box_pack_start(GTK_BOX(dialog_vbox), vbox, TRUE, TRUE, 0);
  
    label = gtk_label_new(lfind->config.value[LCRT_I_TIP]);
    gtk_widget_show(label);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    gtk_widget_set_size_request(label, -1, 30);
    gtk_misc_set_alignment(GTK_MISC(label), 0.01, 0.5);
  
    entry = gtk_entry_new();
    lfind->entry_find = entry;
    gtk_widget_show(entry);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    gtk_widget_set_size_request(entry, -1, 30);
    gtk_entry_set_invisible_char(GTK_ENTRY(entry), 9679);
    GTK_WIDGET_SET_FLAGS (entry, GTK_CAN_DEFAULT);
  
    dialog_action_area = GTK_DIALOG(dialog_find)->action_area;
    gtk_widget_show(dialog_action_area);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area), GTK_BUTTONBOX_END);
  
    cancelbutton = gtk_button_new();
    gtk_widget_show(cancelbutton);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_find), cancelbutton, GTK_RESPONSE_CANCEL);
  
    alignment1 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment1);
    gtk_container_add(GTK_CONTAINER(cancelbutton), alignment1);
  
    hbox1 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox1);
    gtk_container_add(GTK_CONTAINER(alignment1), hbox1);
  
    image1 = gtk_image_new_from_stock("gtk-cancel", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image1);
    gtk_box_pack_start(GTK_BOX(hbox1), image1, FALSE, FALSE, 0);
  
    label2 = gtk_label_new_with_mnemonic(lfind->config.value[LCRT_I_CANCEL]);
    gtk_widget_show(label2);
    gtk_box_pack_start(GTK_BOX(hbox1), label2, FALSE, FALSE, 0);
  
    okbutton = gtk_button_new();
    lfind->okbutton = okbutton;
    gtk_widget_set_sensitive(okbutton, FALSE);
    gtk_widget_show(okbutton);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_find), okbutton, GTK_RESPONSE_OK);
  
    alignment2 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment2);
    gtk_container_add(GTK_CONTAINER(okbutton), alignment2);
  
    hbox2 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox2);
    gtk_container_add(GTK_CONTAINER(alignment2), hbox2);
  
    image2 = gtk_image_new_from_stock("gtk-ok", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image2);
    gtk_box_pack_start(GTK_BOX(hbox2), image2, FALSE, FALSE, 0);
  
    label3 = gtk_label_new_with_mnemonic(lfind->config.value[LCRT_I_OK]);
    gtk_widget_show(label3);
    gtk_box_pack_start(GTK_BOX(hbox2), label3, FALSE, FALSE, 0);
  
    g_signal_connect((gpointer)dialog_find, "delete_event",
                      G_CALLBACK(lcrt_find_on_delete_event),
                      lfind);
    g_signal_connect((gpointer)cancelbutton, "clicked",
                      G_CALLBACK(lcrt_find_on_cancelbutton_clicked),
                      lfind);
    g_signal_connect((gpointer)okbutton, "clicked",
                        G_CALLBACK(lcrt_find_on_okbutton_clicked),
                        lfind);
    g_signal_connect ((gpointer) entry, "changed",
                      G_CALLBACK (lcrt_find_on_name_changed),
                      lfind);
    gtk_dialog_run(GTK_DIALOG(dialog_find));

    return lfind;
err:
    lcrt_message_error(lfind->parent_window, lfind->config.value[LCRT_I_ERR_MSG]);
    return NULL;

}
void lcrt_destroy_find(struct lcrt_find *lfind)
{
    if (lfind)
        free(lfind);
}
static const char *lcrt_find_get_db_name(struct lcrt_find *lfind)
{
    return lfind->config.db.db_name;
}
static const char *lcrt_find_get_tb_name(struct lcrt_find *lfind)
{
    return lfind->config.db.db_table;
}

int lcrt_find_init_config(struct lcrt_find *lfind)
{
    char db_name[256];
    int i;
    static char *name[LCRT_I_NUMBER] = {LCRT_I_NAME};
    static char *value[LCRT_I_NUMBER] = {LCRT_I_VALUE};

    assert(lfind);

    memset(&lfind->config, 0, sizeof(struct lcrtc_find));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lfind->config.db, db_name, LCRT_IFIND_TABLE);

    for (i = 0; i < LCRT_I_NUMBER; i++) {
        lfind->config.name[i] = name[i];
        strncpy(lfind->config.value[i], value[i], sizeof(lfind->config.value[i]));
    }
    lfind->get_db = lcrt_find_get_db_name;
    lfind->get_tb = lcrt_find_get_tb_name;

    return 0;
}
int lcrt_find_load_config(struct lcrt_find *lfind)
{
    int rv, i;

    assert(lfind);

    rv = lfind->config.db.select(&lfind->config.db, "SELECT * FROM %s", lfind->get_tb(lfind));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_find_create_config(lfind);
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_I_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lfind->config.value[i], lfind->config.db.get_text_col(&lfind->config.db, 1), 
                sizeof(lfind->config.value[i]));
        rv = lfind->config.db.get_row(&lfind->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s]\n",
                basename((char *)lfind->get_db(lfind)),
                lfind->get_tb(lfind),
                lfind->config.name[i],
                lfind->config.value[i]);
    }

    return rv;
}
int lcrt_find_create_config(struct lcrt_find *lfind)
{
    int i;

    assert(lfind);
    lfind->config.db.exec(&lfind->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255))",
                         lfind->get_tb(lfind));
    for (i = 0; i < LCRT_I_NUMBER; i++) {
        lfind->config.db.exec(&lfind->config.db,
                             "INSERT INTO %s VALUES('%s', '%s')",
                             lfind->get_tb(lfind),
                             lfind->config.name[i], 
                             lfind->config.value[i]);
    }
    lfind->config.db.close(&lfind->config.db);
    return 0;
}

