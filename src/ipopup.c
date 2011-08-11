#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <assert.h>

#include "iterminal.h"
#include "inotebook.h"
#include "iwindow.h"
#include "ipopup.h"
#include "cpopup.h"
#include "debug.h"

int lcrt_popup_init_config(struct lcrt_popup *lpopup);
int lcrt_popup_load_config(struct lcrt_popup *lpopup);
int lcrt_popup_create_config(struct lcrt_popup *lpopup);

struct lcrt_popup *lcrt_create_popup_menu(struct lcrt_terminal *parent)
{
    struct lcrt_popup *lpopup;
    struct lcrt_window *lwindow = parent->parent->parent;
    GtkWidget *popup_menu;
    GtkWidget *p_rename;
    GtkWidget *p_reconnect;
    GtkWidget *image1;
    GtkWidget *p_disconnect;
    GtkWidget *p_close;
    GtkWidget *p_separator1;
    GtkWidget *p_lock_session;
    GtkWidget *p_clone_session;
    GtkWidget *p_separator2;
    GtkWidget *p_session_options;
    GtkAccelGroup *accel_group;

    lpopup =(struct lcrt_popup *)calloc(1, sizeof(struct lcrt_popup));

    assert(lpopup);

    lcrt_popup_init_config(lpopup);
    lcrt_popup_load_config(lpopup);

    accel_group = gtk_accel_group_new();
    lpopup->parent = parent;
    popup_menu = gtk_menu_new();
    lpopup->popup_menu = popup_menu;

    p_rename = gtk_menu_item_new_with_mnemonic(lpopup->config.value[LCRT_P_RENAME]);
    lpopup->p_rename = p_rename;
    gtk_widget_show(p_rename);
    gtk_container_add(GTK_CONTAINER(popup_menu), p_rename);

    p_reconnect = gtk_image_menu_item_new_with_mnemonic(lpopup->config.value[LCRT_P_RECONNECT]);
    lpopup->p_reconnect = p_reconnect;
    gtk_widget_show(p_reconnect);
    gtk_container_add(GTK_CONTAINER(popup_menu), p_reconnect);

    image1 = gtk_image_new_from_stock("gtk-jump-to", GTK_ICON_SIZE_MENU);
    gtk_widget_show(image1);
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(p_reconnect), image1);

    p_disconnect = gtk_menu_item_new_with_mnemonic(lpopup->config.value[LCRT_P_DISCONNECT]);
    lpopup->p_disconnect = p_disconnect;
    gtk_widget_show(p_disconnect);
    gtk_container_add(GTK_CONTAINER(popup_menu), p_disconnect);

    p_close = gtk_menu_item_new_with_mnemonic(lpopup->config.value[LCRT_P_CLOSE]);
    lpopup->p_close = p_close;
    gtk_widget_show(p_close);
    gtk_container_add(GTK_CONTAINER(popup_menu), p_close);
    gtk_widget_add_accelerator(p_close, "activate", accel_group,
                                GDK_W,(GdkModifierType)GDK_CONTROL_MASK | GDK_SHIFT_MASK,
                                GTK_ACCEL_VISIBLE);

    p_separator1 = gtk_separator_menu_item_new();
    gtk_widget_show(p_separator1);
    gtk_container_add(GTK_CONTAINER(popup_menu), p_separator1);
    gtk_widget_set_sensitive(p_separator1, FALSE);
    if (parent->locked == FALSE)
        p_lock_session = gtk_menu_item_new_with_mnemonic(lpopup->config.value[LCRT_P_LOCK_SESSION]);
    else
        p_lock_session = gtk_menu_item_new_with_mnemonic(lpopup->config.value[LCRT_P_UNLOCK_SESSION]);
    lpopup->p_lock_session = p_lock_session;
    gtk_widget_show(p_lock_session);
    gtk_container_add(GTK_CONTAINER(popup_menu), p_lock_session);

    p_clone_session = gtk_menu_item_new_with_mnemonic(lpopup->config.value[LCRT_P_CLONE_SESSION]);
    lpopup->p_clone_session = p_clone_session;
    gtk_widget_show(p_clone_session);
    gtk_container_add(GTK_CONTAINER(popup_menu), p_clone_session);

    p_separator2 = gtk_separator_menu_item_new();
    gtk_widget_show(p_separator2);
    gtk_container_add(GTK_CONTAINER(popup_menu), p_separator2);
    gtk_widget_set_sensitive(p_separator2, FALSE);

    p_session_options = gtk_menu_item_new_with_mnemonic(lpopup->config.value[LCRT_P_SESSION_OPTIONS]);
    lpopup->p_session_options = p_session_options;
    
    gtk_widget_show(p_session_options);
    gtk_container_add(GTK_CONTAINER(popup_menu), p_session_options);

    g_signal_connect((gpointer)p_rename, "activate",
                      G_CALLBACK(lcrt_popup_on_rename_activate),
                      lwindow);
    g_signal_connect((gpointer)p_reconnect, "activate",
                      G_CALLBACK(lcrt_popup_on_reconnect_activate),
                      lwindow);
    g_signal_connect((gpointer)p_disconnect, "activate",
                      G_CALLBACK(lcrt_popup_on_disconnect_activate),
                      lwindow);
    g_signal_connect((gpointer)p_close, "activate",
                      G_CALLBACK(lcrt_popup_on_close_activate),
                      lwindow);
    g_signal_connect((gpointer)p_lock_session, "activate",
                      G_CALLBACK(lcrt_popup_on_lock_session_activate),
                      lwindow);
    g_signal_connect((gpointer)p_clone_session, "activate",
                      G_CALLBACK(lcrt_popup_on_clone_session_activate),
                      lwindow);
    g_signal_connect((gpointer)p_session_options, "activate",
                      G_CALLBACK(lcrt_popup_on_session_options_activate),
                      lwindow);
    g_signal_connect((gpointer) popup_menu, "deactivate",
                      G_CALLBACK(lcrt_popup_on_deactivate),
                      lpopup);

    //gtk_menu_set_accel_group(GTK_MENU(popup_menu), accel_group);
    gboolean connected = (parent->connected == LCRT_TERMINAL_CONNECTED);

    gtk_widget_set_sensitive(p_reconnect, !connected);
    gtk_widget_set_sensitive(p_disconnect, connected);
    gtk_widget_set_sensitive(p_clone_session, connected);

    if (parent->locked == TRUE) {
        gtk_widget_set_sensitive(p_rename, FALSE);
        gtk_widget_set_sensitive(p_reconnect, FALSE);
        gtk_widget_set_sensitive(p_disconnect, FALSE);
        gtk_widget_set_sensitive(p_clone_session, FALSE);
        gtk_widget_set_sensitive(p_session_options, FALSE);

    }

    return lpopup;
}
static const char *lcrt_popup_get_db_name(struct lcrt_popup *lpopup)
{
    return lpopup == NULL ? NULL : lpopup->config.db.db_name;
}
static const char *lcrt_popup_get_tb_name(struct lcrt_popup *lpopup)
{
    return lpopup == NULL ? NULL : lpopup->config.db.db_table;
}

int lcrt_popup_init_config(struct lcrt_popup *lpopup)
{
    char db_name[256];
    int i;
    static char *name[LCRT_P_NUMBER] = {LCRT_P_NAME};
    static char *value[LCRT_P_NUMBER] = {LCRT_P_VALUE};
    static int shortcut[LCRT_P_NUMBER][2] = {LCRT_P_SHORTCUT};

    if (lpopup == NULL)
        return EINVAL;

    memset(&lpopup->config, 0, sizeof(struct lcrtc_popup));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lpopup->config.db, db_name, LCRT_IPOPUP_TABLE);
    for (i = 0; i < LCRT_P_NUMBER; i++) {
        lpopup->config.name[i] = name[i];
        strncpy(lpopup->config.value[i], value[i], sizeof(lpopup->config.value[i]));
        lpopup->config.shortcut[i][0] = shortcut[i][0];
        lpopup->config.shortcut[i][1] = shortcut[i][1];
    }
    lpopup->get_db = lcrt_popup_get_db_name;
    lpopup->get_tb = lcrt_popup_get_tb_name;

    return 0;
}
int lcrt_popup_load_config(struct lcrt_popup *lpopup)
{
    int rv, i;

    if (lpopup == NULL)
        return EINVAL;

    rv = lpopup->config.db.select(&lpopup->config.db, "SELECT * FROM %s", lpopup->get_tb(lpopup));
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_popup_create_config(lpopup);
        return LCRTE_NO_CONFIG;
    }

    for (i = 0; i < LCRT_P_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lpopup->config.value[i], lpopup->config.db.get_text_col(&lpopup->config.db, 1), 
                sizeof(lpopup->config.value[i]));
        rv = lpopup->config.db.get_row(&lpopup->config.db);

        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s] shortcut = {%-5d, %d}\n",
                basename((char *)lpopup->get_db(lpopup)),
                lpopup->get_tb(lpopup),
                lpopup->config.name[i],
                lpopup->config.value[i],
                lpopup->config.shortcut[i][0],
                lpopup->config.shortcut[i][1]);
    }

    return rv;
}
int lcrt_popup_create_config(struct lcrt_popup *lpopup)
{
    int i;

    assert(lpopup);

    lpopup->config.db.exec(&lpopup->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255),  \
                          shortcut INTEGER, \
                          shortcut_mask INTEGER)",
                         lpopup->get_tb(lpopup));
    for (i = 0; i < LCRT_P_NUMBER; i++) {
        lpopup->config.db.exec(&lpopup->config.db,
                             "INSERT INTO %s VALUES('%s', '%s', %d, %d)",
                             lpopup->get_tb(lpopup),
                             lpopup->config.name[i], 
                             lpopup->config.value[i],
                             lpopup->config.shortcut[i][0],
                             lpopup->config.shortcut[i][1]);
    }
    lpopup->config.db.close(&lpopup->config.db);
    return 0;
}
