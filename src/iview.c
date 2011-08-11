//#define __LCRT_DEBUG__
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <libgen.h>
#include "iwindow.h"
#include "imenubar.h"
#include "iview.h"
#include "debug.h"
#include "mkconfig.h"
#include "iview.h"
#include "cview.h"
#include "foperate.h"
#include "isettings.h"

int lcrt_view_load_config(struct lcrt_view *lview);
int lcrt_view_init_config(struct lcrt_view *lview);

struct lcrt_view *lcrt_view_create_menuitem(struct lcrt_menubar *parent)
{
    struct lcrt_view *lview;

    GtkWidget *v_menuitem;
    GtkWidget *v_menuitem_menu;
    //GtkWidget *v_menubar;
    GtkWidget *v_toolbar;
    GtkWidget *v_status_bar;
    GtkWidget *v_separator_1;
    GtkWidget *v_always_on_top;
    GtkWidget *v_full_screen;
    GtkAccelGroup *accel_group = parent->parent->w_accel;
    struct lcrt_settings *lsettings = parent->parent->w_settings;

    lview = (struct lcrt_view *)calloc(1, sizeof(struct lcrt_view));

    assert(lview);

    lview->parent = parent;

    lcrt_view_init_config(lview);
    lcrt_view_load_config(lview);

    v_menuitem = gtk_menu_item_new_with_mnemonic (lview->config.value[LCRT_V_MENUITEM]);
    lview->v_menuitem = v_menuitem;
    gtk_widget_show (v_menuitem);
    g_signal_connect ((gpointer) v_menuitem, "activate",
                    G_CALLBACK (lcrt_view_on_menuitem_activate),
                    lview);


    v_menuitem_menu = gtk_menu_new ();
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (v_menuitem), v_menuitem_menu);
#ifdef LCRT_SHOW_MENUBAR    
    v_menubar = gtk_check_menu_item_new_with_mnemonic (lview->config.value[LCRT_V_MENUBAR]);
    lview->v_menubar = v_menubar;
    gtk_widget_show (v_menubar);
    gtk_container_add (GTK_CONTAINER (v_menuitem_menu), v_menubar);
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (v_menubar), 
            lview->config.status[LCRT_V_MENUBAR]);
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (v_menubar), TRUE);
    g_signal_connect ((gpointer) v_menubar, "activate",
                    G_CALLBACK (lcrt_view_on_menubar_activate),
                    lview);
#endif
    v_toolbar = gtk_check_menu_item_new_with_mnemonic (lview->config.value[LCRT_V_TOOLBAR]);
    lview->v_toolbar = v_toolbar;
    gtk_widget_show (v_toolbar);
    gtk_container_add (GTK_CONTAINER (v_menuitem_menu), v_toolbar);
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (v_toolbar), 
            lsettings->lt_g_show_toolbar);
    g_signal_connect ((gpointer) v_toolbar, "activate",
                    G_CALLBACK (lcrt_view_on_toolbar_activate),
                    lview);

    v_status_bar = gtk_check_menu_item_new_with_mnemonic (lview->config.value[LCRT_V_STATUS_BAR]);
    lview->v_status_bar = v_status_bar;
    gtk_widget_show (v_status_bar);
    gtk_container_add (GTK_CONTAINER (v_menuitem_menu), v_status_bar);
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (v_status_bar), 
            lsettings->lt_g_show_statusbar);
    g_signal_connect ((gpointer) v_status_bar, "activate",
                    G_CALLBACK (lcrt_view_on_status_bar_activate),
                    lview);

    v_separator_1 = gtk_separator_menu_item_new ();
    gtk_widget_show (v_separator_1);
    gtk_container_add (GTK_CONTAINER (v_menuitem_menu), v_separator_1);
    gtk_widget_set_sensitive (v_separator_1, FALSE);

    v_always_on_top = gtk_check_menu_item_new_with_mnemonic (lview->config.value[LCRT_V_ALWAYS_ON_TOP]);
    lview->v_always_on_top = v_always_on_top;
    gtk_widget_show (v_always_on_top);
    gtk_container_add (GTK_CONTAINER (v_menuitem_menu), v_always_on_top);
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (v_always_on_top), 
            FALSE);
    g_signal_connect ((gpointer) v_always_on_top, "activate",
                    G_CALLBACK (lcrt_view_on_always_on_top_activate),
                    lview);

    v_full_screen = gtk_menu_item_new_with_mnemonic (lview->config.value[LCRT_V_FULL_SCREEN]);
    lview->v_full_screen = v_full_screen;
    gtk_widget_show (v_full_screen);
    gtk_container_add (GTK_CONTAINER (v_menuitem_menu), v_full_screen);
    gtk_widget_add_accelerator (v_full_screen, "activate", accel_group,
                                lview->config.shortcut[LCRT_V_FULL_SCREEN][0],
                               (GdkModifierType) lview->config.shortcut[LCRT_V_FULL_SCREEN][1],
                                GTK_ACCEL_VISIBLE);
    g_signal_connect ((gpointer) v_full_screen, "activate",
                    G_CALLBACK (lcrt_view_on_full_screen_activate),
                    lview);

    return lview;
}
void lcrt_view_destroy_menuitem(struct lcrt_view *lview)
{
    gtk_widget_destroy(lview->v_menuitem);
    if (lview)
        free(lview);
}
static const char *lcrt_view_get_db_name(struct lcrt_view *lview)
{
    return lview == NULL ? NULL : lview->config.db.db_name;
}
static const char *lcrt_view_get_tb_name(struct lcrt_view *lview)
{
    return lview == NULL ? NULL : lview->config.db.db_table;
}

int lcrt_view_init_config(struct lcrt_view *lview)
{
    char db_name[256];
    int i;
    static char *name[LCRT_V_NUMBER] = {LCRT_V_NAME};
    static char *value[LCRT_V_NUMBER] = {LCRT_V_VALUE};
    int shortcut[LCRT_V_NUMBER][2] = {LCRT_V_SHORTCUT};

    assert(lview);

    memset(&lview->config, 0, sizeof(struct lcrtc_view));

    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lview->config.db, db_name, LCRT_IVIEW_TABLE);

    for (i = 0; i < LCRT_V_NUMBER; i++) {
        lview->config.name[i] = name[i];
        strncpy(lview->config.value[i], value[i], sizeof(lview->config.value[i]));
        lview->config.shortcut[i][0] = shortcut[i][0];
        lview->config.shortcut[i][1] = shortcut[i][1];
    }
    lview->get_db = lcrt_view_get_db_name;
    lview->get_tb = lcrt_view_get_tb_name;

    return 0;
}
int lcrt_view_load_config(struct lcrt_view *lview)
{
    int rv, i;

    assert(lview);

    rv = lview->config.db.select(&lview->config.db, "SELECT * FROM %s", lview->get_tb(lview));
    if (rv == LCRTE_NO_TABLE) {
        return LCRTE_NO_CONFIG;
    }

    for (i = 0; i < LCRT_V_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lview->config.value[i], lview->config.db.get_text_col(&lview->config.db, 1),
                sizeof(lview->config.value[i]));
        rv = lview->config.db.get_row(&lview->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s]\n",
                basename((char *)lview->get_db(lview)),
                lview->get_tb(lview),
                lview->config.name[i],
                lview->config.value[i]);

    }

    return rv;
}

