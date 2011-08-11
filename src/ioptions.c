#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include "assert.h"
#include "iwindow.h"
#include "imenubar.h"
#include "debug.h"
#include "mkconfig.h"
#include "ioptions.h"
#include "coptions.h"
#include "isettings.h"

int lcrt_options_create_config(struct lcrt_options *loptions);
int lcrt_options_load_config(struct lcrt_options *loptions);
int lcrt_options_init_config(struct lcrt_options *loptions);

struct lcrt_options *lcrt_options_create_menuitem(struct lcrt_menubar *parent)
{
    struct lcrt_options *loptions;

    GtkWidget *o_menuitem;
    GtkWidget *o_menuitem_menu;
    GtkWidget *o_session_options;
    GtkWidget *image238;
    GtkWidget *o_global_options;
    GtkWidget *o_separator_2;
    GtkWidget *o_auto_save_options;
    GtkWidget *o_save_settings_now;
    //GtkAccelGroup *accel_group = parent->parent->w_accel;

    loptions =(struct lcrt_options *)calloc(1,sizeof(struct lcrt_options));
    assert(loptions);

    lcrt_options_init_config(loptions);
    lcrt_options_load_config(loptions);
    loptions->parent = parent;

    o_menuitem = gtk_menu_item_new_with_mnemonic (loptions->config.value[LCRT_O_MENUITEM]);
    loptions->o_menuitem = o_menuitem;
    gtk_widget_show (o_menuitem);

    o_menuitem_menu = gtk_menu_new ();
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (o_menuitem), o_menuitem_menu);

    o_session_options = gtk_image_menu_item_new_with_mnemonic (loptions->config.value[LCRT_O_SESSION_OPTIONS]);
    loptions->o_session_options = o_session_options;
    gtk_widget_show (o_session_options);
    gtk_container_add (GTK_CONTAINER (o_menuitem_menu), o_session_options);
    g_signal_connect ((gpointer) o_session_options, "activate",
                    G_CALLBACK (lcrt_options_on_session_options_activate),
                    loptions);

    image238 = gtk_image_new_from_stock ("gtk-properties", GTK_ICON_SIZE_MENU);
    gtk_widget_show (image238);
    gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (o_session_options), image238);

    o_global_options = gtk_menu_item_new_with_mnemonic (loptions->config.value[LCRT_O_GLOBAL_OPTIONS]);
    loptions->o_global_options = o_global_options;
    gtk_widget_show (o_global_options);
    gtk_container_add (GTK_CONTAINER (o_menuitem_menu), o_global_options);
    g_signal_connect ((gpointer) o_global_options, "activate",
                    G_CALLBACK (lcrt_options_on_global_options_activate),
                    loptions);

    o_separator_2 = gtk_separator_menu_item_new ();
    gtk_widget_show (o_separator_2);
    gtk_container_add (GTK_CONTAINER (o_menuitem_menu), o_separator_2);
    gtk_widget_set_sensitive (o_separator_2, FALSE);

    o_auto_save_options = gtk_check_menu_item_new_with_mnemonic (loptions->config.value[LCRT_O_AUTO_SAVE_OPTIONS]);
    loptions->o_auto_save_options = o_auto_save_options;
    gtk_widget_show (o_auto_save_options);
    gtk_container_add (GTK_CONTAINER (o_menuitem_menu), o_auto_save_options);
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (o_auto_save_options), 
            parent->parent->w_settings->auto_save_config);
    g_signal_connect ((gpointer) o_auto_save_options, "activate",
                    G_CALLBACK (lcrt_options_on_auto_save_options_activate),
                    loptions);

    o_save_settings_now = gtk_menu_item_new_with_mnemonic (loptions->config.value[LCRT_O_SAVE_SETTINGS_NOW]);
    loptions->o_save_settings_now = o_save_settings_now;
    gtk_widget_show (o_save_settings_now);
    gtk_container_add (GTK_CONTAINER (o_menuitem_menu), o_save_settings_now);
    g_signal_connect ((gpointer) o_save_settings_now, "activate",
                    G_CALLBACK (lcrt_options_on_save_settings_now_activate),
                    loptions);

    return loptions;
}
void lcrt_options_destroy_menuitem(struct lcrt_options *loptions)
{
    gtk_widget_destroy(loptions->o_menuitem);
    if (loptions)
        free(loptions);
}
static const char *lcrt_options_get_db_name(struct lcrt_options *loptions)
{
    return loptions == NULL ? NULL : loptions->config.db.db_name;
}
static const char *lcrt_options_get_tb_name(struct lcrt_options *loptions)
{
    return loptions == NULL ? NULL : loptions->config.db.db_table;
}

int lcrt_options_init_config(struct lcrt_options *loptions)
{
    char db_name[256];
    int i;
    static char *name[LCRT_O_NUMBER] = {LCRT_O_NAME};
    static char *value[LCRT_O_NUMBER] = {LCRT_O_VALUE};
    static int shortcut[LCRT_O_NUMBER][2] = {LCRT_O_SHORTCUT};

    if (loptions == NULL)
        return EINVAL;

    memset(&loptions->config, 0, sizeof(struct lcrtc_options));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&loptions->config.db, db_name, LCRT_IOPTIONS_TABLE);
    for (i = 0; i < LCRT_O_NUMBER; i++) {
        loptions->config.name[i] = name[i];
        strncpy(loptions->config.value[i], value[i], sizeof(loptions->config.value[i]));
        loptions->config.shortcut[i][0] = shortcut[i][0];
        loptions->config.shortcut[i][1] = shortcut[i][1];
    }
    loptions->get_db = lcrt_options_get_db_name;
    loptions->get_tb = lcrt_options_get_tb_name;

    return 0;
}
int lcrt_options_load_config(struct lcrt_options *loptions)
{
    int rv, i;

    if (loptions == NULL)
        return EINVAL;

    rv = loptions->config.db.select(&loptions->config.db, "SELECT * FROM %s", loptions->get_tb(loptions));
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_options_create_config(loptions);
        return LCRTE_NO_CONFIG;
    }

    for (i = 0; i < LCRT_O_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(loptions->config.value[i], loptions->config.db.get_text_col(&loptions->config.db, 1),
                sizeof(loptions->config.value[i]));
        rv = loptions->config.db.get_row(&loptions->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s] shortcut = {%-5d, %d}\n",
                basename((char *)loptions->get_db(loptions)),
                loptions->get_tb(loptions),
                loptions->config.name[i],
                loptions->config.value[i],
                loptions->config.shortcut[i][0],
                loptions->config.shortcut[i][1]);
    }

    return rv;
}
int lcrt_options_create_config(struct lcrt_options *loptions)
{
    int i;
    if (loptions == NULL)
        return EINVAL;
    loptions->config.db.exec(&loptions->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255),  \
                          shortcut INTEGER, \
                          shortcut_mask INTEGER)",
                         loptions->get_tb(loptions));
    for (i = 0; i < LCRT_O_NUMBER; i++) {
        loptions->config.db.exec(&loptions->config.db,
                             "INSERT INTO %s VALUES('%s', '%s', %d, %d)",
                             loptions->get_tb(loptions),
                             loptions->config.name[i], 
                             loptions->config.value[i],
                             loptions->config.shortcut[i][0],
                             loptions->config.shortcut[i][1]);
    }
    loptions->config.db.close(&loptions->config.db);
    return 0;
}
