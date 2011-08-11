#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <assert.h>
#include "iwindow.h"
#include "imenubar.h"
#include "ifile.h"
#include "debug.h"
#include "mkconfig.h"
#include "cfile.h"
#include "support.h"
#include "iaccels.h"

int lcrt_file_init_config(struct lcrt_file *lfile);
int lcrt_file_load_config(struct lcrt_file *lfile);
int lcrt_file_create_config(struct lcrt_file *lfile);

struct lcrt_file *lcrt_file_create_menuitem(struct lcrt_menubar *parent)
{
    debug_where();
    struct lcrt_file *lfile;

    GtkWidget *f_menuitem;
    GtkWidget *f_menuitem_menu;
    GtkWidget *f_connect;
    GtkWidget *image228;
    GtkWidget *f_quick_connect;
    GtkWidget *image229;
    GtkWidget *f_connect_in_tab;
    GtkWidget *image230;
    GtkWidget *f_separator_1;
    GtkWidget *f_reconnect;
    GtkWidget *image231;
    GtkWidget *f_reconnect_all;
    GtkWidget *image232;
    GtkWidget *f_disconnect;
    GtkWidget *image233;
    GtkWidget *f_disconnect_all;
    GtkWidget *image234;
    GtkWidget *f_separator_2;
    GtkWidget *f_clone_session;
    GtkWidget *f_separator_3;
    GtkWidget *f_lock_session;
    GtkWidget *f_separator_4;
    GtkWidget *f_print;
    GtkWidget *f_print_setup;
    GtkWidget *f_separator_5;
    GtkWidget *f_log_session;
    GtkWidget *f_separator_6;
    GtkWidget *f_recent_session;
    GtkWidget *f_separator_7;
    GtkWidget *f_exit;
    GtkAccelGroup *accel_group = parent->parent->w_accel;

    lfile = (struct lcrt_file *)calloc(1, sizeof(struct lcrt_file));
    char *image[LCRT_F_NUMBER] = {LCRT_F_ICONS};
    
    assert(lfile);

    lfile->parent = parent;

    lcrt_file_init_config(lfile);
    lcrt_file_load_config(lfile);

    f_menuitem = gtk_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_MENUITEM]);
    lfile->f_menuitem = f_menuitem;
    gtk_widget_show(f_menuitem);
    g_signal_connect ((gpointer) f_menuitem, "activate",
                    G_CALLBACK (lcrt_file_on_menuitem_activate),
                    parent->parent);

    f_menuitem_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(f_menuitem), f_menuitem_menu);

    f_connect = gtk_image_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_CONNECT]);
    lfile->f_connect = f_connect;
    gtk_widget_show(f_connect);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_connect);
    
    gtk_widget_add_accelerator(f_connect, "activate", accel_group,
                                lfile->config.shortcut[LCRT_F_CONNECT][0],
                               (GdkModifierType) lfile->config.shortcut[LCRT_F_CONNECT][1],
                                GTK_ACCEL_VISIBLE);
    g_signal_connect((gpointer) f_connect, "activate",
                    G_CALLBACK(lcrt_file_on_connect_activate),
                    lfile);
    image228 = create_pixmap (NULL, image[LCRT_F_CONNECT]);
    //image228 = gtk_image_new_from_stock("gtk-connect", GTK_ICON_SIZE_MENU);
    gtk_widget_show(image228);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(f_connect), image228);

    f_quick_connect = gtk_image_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_QUICK_CONNECT]);
    lfile->f_quick_connect = f_quick_connect;
    gtk_widget_show(f_quick_connect);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_quick_connect);
    gtk_widget_add_accelerator(f_quick_connect, "activate", accel_group,
                                lfile->config.shortcut[LCRT_F_QUICK_CONNECT][0],
                               (GdkModifierType) lfile->config.shortcut[LCRT_F_QUICK_CONNECT][1],
                                GTK_ACCEL_VISIBLE);
    g_signal_connect((gpointer) f_quick_connect, "activate",
                    G_CALLBACK(lcrt_file_on_quick_connect_activate),
                    lfile);

    image229 = create_pixmap(NULL, image[LCRT_F_QUICK_CONNECT]);
    //image229 = gtk_image_new_from_stock("gtk-disconnect", GTK_ICON_SIZE_MENU);
    gtk_widget_show(image229);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(f_quick_connect), image229);

    f_connect_in_tab = gtk_image_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_CONNECT_IN_TAB]);
    lfile->f_connect_in_tab = f_connect_in_tab;
    gtk_widget_show(f_connect_in_tab);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_connect_in_tab);
    gtk_widget_add_accelerator(f_connect_in_tab, "activate", accel_group,
                                lfile->config.shortcut[LCRT_F_CONNECT_IN_TAB][0],
                               (GdkModifierType) lfile->config.shortcut[LCRT_F_CONNECT_IN_TAB][1],
                                GTK_ACCEL_VISIBLE);
    g_signal_connect((gpointer) f_connect_in_tab, "activate",
                    G_CALLBACK(lcrt_file_on_connect_in_tab_activate),
                    lfile);


    image230 = create_pixmap(NULL, image[LCRT_F_CONNECT_IN_TAB]);
    //image230 = gtk_image_new_from_stock("gtk-missing-image", GTK_ICON_SIZE_MENU);
    gtk_widget_show(image230);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(f_connect_in_tab), image230);

    f_separator_1 = gtk_separator_menu_item_new();
    gtk_widget_show(f_separator_1);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_separator_1);
    gtk_widget_set_sensitive(f_separator_1, FALSE);

    f_reconnect = gtk_image_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_RECONNECT]);
    lfile->f_reconnect = f_reconnect;
    gtk_widget_show(f_reconnect);
    gtk_widget_set_sensitive(f_reconnect, FALSE);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_reconnect);
    gtk_widget_add_accelerator(f_reconnect, "activate", accel_group,
                                lfile->config.shortcut[LCRT_F_RECONNECT][0],
                               (GdkModifierType) lfile->config.shortcut[LCRT_F_RECONNECT][1],
                                GTK_ACCEL_VISIBLE);
    g_signal_connect((gpointer) f_reconnect, "activate",
                    G_CALLBACK(lcrt_file_on_reconnect_activate),
                    lfile);

    image231 = create_pixmap(NULL, "lcrt-reconnect.png");
    gtk_widget_show(image231);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(f_reconnect), image231);

    f_reconnect_all = gtk_image_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_RECONNECT_ALL]);
    lfile->f_reconnect_all = f_reconnect_all;
    gtk_widget_show(f_reconnect_all);
    gtk_widget_set_sensitive(f_reconnect_all, FALSE);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_reconnect_all);
    g_signal_connect((gpointer) f_reconnect_all, "activate",
                    G_CALLBACK(lcrt_file_on_reconnect_all_activate),
                    lfile);

    image232 = gtk_image_new_from_stock("gtk-refresh", GTK_ICON_SIZE_MENU);
    gtk_widget_show(image232);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(f_reconnect_all), image232);

    f_disconnect = gtk_image_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_DISCONNECT]);
    lfile->f_disconnect = f_disconnect;
    gtk_widget_show(f_disconnect);
    gtk_widget_set_sensitive(f_disconnect, FALSE);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_disconnect);
    g_signal_connect((gpointer) f_disconnect, "activate",
                    G_CALLBACK(lcrt_file_on_disconnect_activate),
                    lfile);

    image233 = create_pixmap(NULL, "lcrt-disconnect.png");
    gtk_widget_show(image233);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(f_disconnect), image233);

    f_disconnect_all = gtk_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_DISCONNECT_ALL]);
    lfile->f_disconnect_all = f_disconnect_all;
    gtk_widget_show(f_disconnect_all);
    gtk_widget_set_sensitive(f_disconnect_all, FALSE);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_disconnect_all);
    g_signal_connect((gpointer) f_disconnect_all, "activate",
                    G_CALLBACK(lcrt_file_on_disconnect_all_activate),
                    lfile);

    f_separator_2 = gtk_separator_menu_item_new();
    gtk_widget_show(f_separator_2);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_separator_2);
    gtk_widget_set_sensitive(f_separator_2, FALSE);

    f_clone_session = gtk_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_CLONE_SESSION]);
    lfile->f_clone_session = f_clone_session;
    gtk_widget_show(f_clone_session);
    gtk_widget_set_sensitive(f_clone_session, FALSE);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_clone_session);
    g_signal_connect((gpointer) f_clone_session, "activate",
                    G_CALLBACK(lcrt_file_on_clone_session_activate),
                    lfile);

    f_separator_3 = gtk_separator_menu_item_new();
    gtk_widget_show(f_separator_3);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_separator_3);
    gtk_widget_set_sensitive(f_separator_3, FALSE);

    f_lock_session = gtk_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_LOCK_SESSION]);
    lfile->f_lock_session = f_lock_session;
    gtk_widget_show(f_lock_session);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_lock_session);
    g_signal_connect((gpointer) f_lock_session, "activate",
                    G_CALLBACK(lcrt_file_on_lock_session_activate),
                    lfile);

    f_separator_4 = gtk_separator_menu_item_new();
    gtk_widget_show(f_separator_4);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_separator_4);
    gtk_widget_set_sensitive(f_separator_4, FALSE);

    f_print = gtk_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_PRINT]);
    lfile->f_print = f_print;
    gtk_widget_show(f_print);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_print);
    g_signal_connect((gpointer) f_print, "activate",
                    G_CALLBACK(lcrt_file_on_print_activate),
                    lfile);

    f_print_setup = gtk_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_PRINT_SETUP]);
    lfile->f_print_setup = f_print_setup;
    gtk_widget_show(f_print_setup);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_print_setup);
    g_signal_connect((gpointer) f_print_setup, "activate",
                    G_CALLBACK(lcrt_file_on_print_setup_activate),
                    lfile);

    f_separator_5 = gtk_separator_menu_item_new();
    gtk_widget_show(f_separator_5);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_separator_5);
    gtk_widget_set_sensitive(f_separator_5, FALSE);

    f_log_session = gtk_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_LOG_SESSION]);
    lfile->f_log_session = f_log_session;
    gtk_widget_show(f_log_session);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_log_session);
    g_signal_connect((gpointer) f_log_session, "activate",
                    G_CALLBACK(lcrt_file_on_log_session_activate),
                    lfile);

    f_separator_6 = gtk_separator_menu_item_new();
    gtk_widget_show(f_separator_6);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_separator_6);
    gtk_widget_set_sensitive(f_separator_6, FALSE);

    f_recent_session = gtk_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_RECENT_SESSION]);
    lfile->f_recent_session = f_recent_session;
    gtk_widget_show(f_recent_session);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_recent_session);
    g_signal_connect((gpointer) f_recent_session, "activate",
                    G_CALLBACK(lcrt_file_on_recent_session_activate),
                    lfile);

    f_separator_7 = gtk_separator_menu_item_new();
    gtk_widget_show(f_separator_7);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_separator_7);
    gtk_widget_set_sensitive(f_separator_7, FALSE);

    f_exit = gtk_image_menu_item_new_with_mnemonic(lfile->config.value[LCRT_F_EXIT]);
    lfile->f_exit = f_exit;
    gtk_widget_show(f_exit);
    gtk_container_add(GTK_CONTAINER(f_menuitem_menu), f_exit);
    g_signal_connect((gpointer) f_exit, "activate",
                    G_CALLBACK(lcrt_file_on_exit_activate),
                    lfile);

    image234 = gtk_image_new_from_stock("gtk-quit", GTK_ICON_SIZE_MENU);
    gtk_widget_show(image234);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(f_exit), image234);

    return lfile;
}

void lcrt_file_destroy_menuitem(struct lcrt_file *lfile)
{
    gtk_widget_destroy(lfile->f_menuitem);
    if (lfile)
        free(lfile);
}
static const char *lcrt_file_get_db_name(struct lcrt_file *lfile)
{
    return lfile == NULL ? NULL : lfile->config.db.db_name;
}
static const char *lcrt_file_get_tb_name(struct lcrt_file *lfile)
{
    return lfile == NULL ? NULL : lfile->config.db.db_table;
}

int lcrt_file_init_config(struct lcrt_file *lfile)
{
    char db_name[256];
    int i;
    static char *name[LCRT_F_NUMBER] = {LCRT_F_NAME};
    static char *value[LCRT_F_NUMBER] = {LCRT_F_VALUE};

    assert(lfile);

    memset(&lfile->config, 0, sizeof(struct lcrtc_file));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lfile->config.db, db_name, LCRT_IFILE_TABLE);
    for (i = 0; i < LCRT_F_NUMBER; i++) {
        lfile->config.name[i] = name[i];
        strncpy(lfile->config.value[i], value[i], sizeof(lfile->config.value[i]));
    }
    lfile->get_db = lcrt_file_get_db_name;
    lfile->get_tb = lcrt_file_get_tb_name;

    return 0;
}
int lcrt_file_load_config(struct lcrt_file *lfile)
{
    int rv, i;
    struct lcrt_accels *laccels = lfile->parent->parent->w_accels;

    assert(lfile);

    memcpy(lfile->config.shortcut[LCRT_F_CONNECT], 
           laccels->config.key[LCRT_KB_F_CONNECT], 
           sizeof(int) * 2);
    memcpy(lfile->config.shortcut[LCRT_F_QUICK_CONNECT], 
           laccels->config.key[LCRT_KB_F_QUICK_CONNECT],
           sizeof(int) * 2);
    memcpy(lfile->config.shortcut[LCRT_F_CONNECT_IN_TAB],
           laccels->config.key[LCRT_KB_F_CONNECT_IN_TAB],
           sizeof(int) * 2);
    memcpy(lfile->config.shortcut[LCRT_F_RECONNECT],
           laccels->config.key[LCRT_KB_F_RECONNECT],
           sizeof(int) * 2);

    rv = lfile->config.db.select(&lfile->config.db, "SELECT * FROM %s", lfile->get_tb(lfile));

    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_F_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lfile->config.value[i], lfile->config.db.get_text_col(&lfile->config.db, 1), 
                sizeof(lfile->config.value[i]));
        rv = lfile->config.db.get_row(&lfile->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s] shortcut = {%-5d, %d}\n",
                basename((char *)lfile->get_db(lfile)),
                lfile->get_tb(lfile),
                lfile->config.name[i],
                lfile->config.value[i],
                lfile->config.shortcut[i][0],
                lfile->config.shortcut[i][1]);
    }

    return rv;
}

