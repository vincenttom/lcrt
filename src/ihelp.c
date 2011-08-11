#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <assert.h>
#include "iwindow.h"
#include "imenubar.h"
#include "debug.h"
#include "mkconfig.h"
#include "ihelp.h"
#include "chelp.h"

int lcrt_help_create_config(struct lcrt_help *lhelp);
int lcrt_help_load_config(struct lcrt_help *lhelp);
int lcrt_help_init_config(struct lcrt_help *lhelp);

struct lcrt_help *lcrt_help_create_menuitem(struct lcrt_menubar *parent)
{
    struct lcrt_help *lhelp;

    GtkWidget *h_menuitem;
    GtkWidget *h_menuitem_menu;
    GtkWidget *h_help;
    GtkWidget *h_home_page;
    GtkWidget *h_download_page;
    GtkWidget *h_separator_1;
    GtkWidget *h_about;
    GtkAccelGroup *accel_group = parent->parent->w_accel;

    lhelp =(struct lcrt_help *)calloc(1, sizeof(struct lcrt_help));
    assert(lhelp);

    lcrt_help_init_config(lhelp);
    lcrt_help_load_config(lhelp);
    lhelp->parent = parent;

    h_menuitem = gtk_menu_item_new_with_mnemonic (lhelp->config.value[LCRT_H_MENUITEM]);
    lhelp->h_menuitem = h_menuitem;
    gtk_widget_show (h_menuitem);

    h_menuitem_menu = gtk_menu_new ();
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (h_menuitem), h_menuitem_menu);

    h_help = gtk_menu_item_new_with_mnemonic (lhelp->config.value[LCRT_H_HELP]);
    lhelp->h_help = h_help;
    gtk_widget_show (h_help);
    gtk_container_add (GTK_CONTAINER (h_menuitem_menu), h_help);
    gtk_widget_add_accelerator (h_help, "activate", accel_group,
                                lhelp->config.shortcut[LCRT_H_HELP][0],
                               (GdkModifierType) lhelp->config.shortcut[LCRT_H_HELP][1],
                                GTK_ACCEL_VISIBLE);
    g_signal_connect ((gpointer) h_help, "activate",
                    G_CALLBACK (lcrt_help_on_help_activate),
                    lhelp);

    h_home_page = gtk_menu_item_new_with_mnemonic (lhelp->config.value[LCRT_H_HOME_PAGE]);
    lhelp->h_home_page = h_home_page;
    gtk_widget_show (h_home_page);
    gtk_container_add (GTK_CONTAINER (h_menuitem_menu), h_home_page);
    g_signal_connect ((gpointer) h_home_page, "activate",
                    G_CALLBACK (lcrt_help_on_home_page_activate),
                    lhelp);

    h_download_page = gtk_menu_item_new_with_mnemonic (lhelp->config.value[LCRT_H_DOWNLOAD_PAGE]);
    lhelp->h_download_page = h_download_page;
    gtk_widget_show (h_download_page);
    gtk_container_add (GTK_CONTAINER (h_menuitem_menu), h_download_page);
    g_signal_connect ((gpointer) h_download_page, "activate",
                    G_CALLBACK (lcrt_help_on_download_page_activate),
                    lhelp);

    h_separator_1 = gtk_separator_menu_item_new ();
    gtk_widget_show (h_separator_1);
    gtk_container_add (GTK_CONTAINER (h_menuitem_menu), h_separator_1);
    gtk_widget_set_sensitive (h_separator_1, FALSE);

    h_about = gtk_menu_item_new_with_mnemonic (lhelp->config.value[LCRT_H_ABOUT]);
    lhelp->h_about = h_about;
    gtk_widget_show (h_about);
    gtk_container_add (GTK_CONTAINER (h_menuitem_menu), h_about);
    g_signal_connect ((gpointer) h_about, "activate",
                    G_CALLBACK (h_on_about_activate),
                    lhelp);

    return lhelp;
}
void lcrt_help_destroy_menuitem(struct lcrt_help *lhelp)
{
    gtk_widget_destroy(lhelp->h_menuitem);
    if (lhelp)
        free(lhelp);
}
static const char *lcrt_help_get_db_name(struct lcrt_help *lhelp)
{
    return lhelp == NULL ? NULL : lhelp->config.db.db_name;
}
static const char *lcrt_help_get_tb_name(struct lcrt_help *lhelp)
{
    return lhelp == NULL ? NULL : lhelp->config.db.db_table;
}

int lcrt_help_init_config(struct lcrt_help *lhelp)
{
    char db_name[256];
    int i;
    static char *name[LCRT_H_NUMBER] = {LCRT_H_NAME};
    static char *value[LCRT_H_NUMBER] = {LCRT_H_VALUE};
    static int shortcut[LCRT_H_NUMBER][2] = {LCRT_H_SHORTCUT};

    assert(lhelp);

    memset(&lhelp->config, 0, sizeof(struct lcrtc_help));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lhelp->config.db, db_name, LCRT_IHELP_TABLE);
    for (i = 0; i < LCRT_H_NUMBER; i++) {
        lhelp->config.name[i] = name[i];
        strncpy(lhelp->config.value[i], value[i], sizeof(lhelp->config.value[i]));
        lhelp->config.shortcut[i][0] = shortcut[i][0];
        lhelp->config.shortcut[i][1] = shortcut[i][1];
    }
    lhelp->get_db = lcrt_help_get_db_name;
    lhelp->get_tb = lcrt_help_get_tb_name;

    return 0;
}
int lcrt_help_load_config(struct lcrt_help *lhelp)
{
    int rv, i;

    assert(lhelp);

    rv = lhelp->config.db.select(&lhelp->config.db, "SELECT * FROM %s", lhelp->get_tb(lhelp));
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_help_create_config(lhelp);
        return LCRTE_NO_CONFIG;
    }

    for (i = 0; i < LCRT_H_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lhelp->config.value[i], lhelp->config.db.get_text_col(&lhelp->config.db, 1),
                sizeof(lhelp->config.value[i]));
        rv = lhelp->config.db.get_row(&lhelp->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s] shortcut = {%-5d, %d}\n",
                basename((char *)lhelp->get_db(lhelp)),
                lhelp->get_tb(lhelp),
                lhelp->config.name[i],
                lhelp->config.value[i],
                lhelp->config.shortcut[i][0],
                lhelp->config.shortcut[i][1]);
    }

    return rv;
}
int lcrt_help_create_config(struct lcrt_help *lhelp)
{
    int i;

    assert(lhelp);

    lhelp->config.db.exec(&lhelp->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255),  \
                          shortcut INTEGER, \
                          shortcut_mask INTEGER)",
                         lhelp->get_tb(lhelp));
    for (i = 0; i < LCRT_H_NUMBER; i++) {
        lhelp->config.db.exec(&lhelp->config.db,
                             "INSERT INTO %s VALUES('%s', '%s', %d, %d)",
                             lhelp->get_tb(lhelp),
                             lhelp->config.name[i], 
                             lhelp->config.value[i],
                             lhelp->config.shortcut[i][0],
                             lhelp->config.shortcut[i][1]);
    }
    lhelp->config.db.close(&lhelp->config.db);
    return 0;
}
