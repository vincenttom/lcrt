#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <assert.h>
#include "support.h"
#include "iwindow.h"
#include "imenubar.h"
#include "debug.h"
#include "mkconfig.h"
#include "iedit.h"
#include "cedit.h"
#include "iview.h"
#include "iterminal.h"
#include "cterminal.h"
#include "iaccels.h"

int lcrt_edit_create_config(struct lcrt_edit *ledit);
int lcrt_edit_load_config(struct lcrt_edit *ledit);
int lcrt_edit_init_config(struct lcrt_edit *ledit);

struct lcrt_edit *lcrt_edit_create_menuitem(struct lcrt_menubar *parent, gboolean popup_menu)
{
    struct lcrt_edit *ledit;
    struct lcrt_window *lwindow;

    GtkWidget *e_menuitem;
    GtkWidget *e_menuitem_menu;
    GtkWidget *e_copy;
    GtkWidget *image235;
    GtkWidget *e_paste;
    GtkWidget *image236;
    GtkWidget *e_copy_and_paste;
    GtkWidget *e_select_all;
    GtkWidget *e_find;
    GtkWidget *image237;
    GtkWidget *e_separator_1;
    GtkWidget *e_clear_scrollback;
    GtkWidget *e_clear_screen;
    GtkWidget *e_clear_screen_and_scrollback;
    GtkWidget *e_separator_2;
    GtkWidget *e_reset;
#ifdef LCRT_SHOW_MENUBAR    
    GtkWidget *e_show_menubar;
    GtkWidget *e_separator_3;
#endif
    GtkAccelGroup *accel_group = parent->parent->w_accel;
    gboolean has_selection;

    ledit =(struct lcrt_edit *)calloc(1, sizeof(struct lcrt_edit));

    assert(ledit);

    ledit->parent = parent;
    lwindow = parent->parent;

    lcrt_edit_init_config(ledit);
    lcrt_edit_load_config(ledit);
    has_selection = lcrt_terminal_has_selection(parent->parent);
    if (!popup_menu) {
        e_menuitem = gtk_menu_item_new_with_mnemonic (ledit->config.value[LCRT_E_MENUITEM]);
        g_signal_connect ((gpointer) e_menuitem, "activate",
                    G_CALLBACK (lcrt_edit_on_menuitem_activate),
                    lwindow);
        ledit->e_menuitem = e_menuitem;
        gtk_widget_show (e_menuitem);

        e_menuitem_menu = gtk_menu_new ();
        gtk_menu_item_set_submenu (GTK_MENU_ITEM (e_menuitem), e_menuitem_menu);
    } else {
        e_menuitem_menu = gtk_menu_new ();
        ledit->e_menuitem = e_menuitem_menu;
    }

    e_copy = gtk_image_menu_item_new_with_mnemonic (ledit->config.value[LCRT_E_COPY]);
    ledit->e_copy = e_copy;
    gtk_widget_show (e_copy);
    gtk_widget_set_sensitive (e_copy, has_selection);
    gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_copy);

    g_signal_connect ((gpointer) e_copy, "activate",
                    G_CALLBACK (lcrt_edit_on_copy_activate),
                    lwindow);
    gtk_widget_add_accelerator (e_copy, "activate", accel_group,
                                ledit->config.shortcut[LCRT_E_COPY][0],
                               (GdkModifierType) ledit->config.shortcut[LCRT_E_COPY][1],
                                GTK_ACCEL_VISIBLE);


    image235 = create_pixmap(NULL, "lcrt-copy.png");
    gtk_widget_show (image235);
    gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (e_copy), image235);

    e_paste = gtk_image_menu_item_new_with_mnemonic (ledit->config.value[LCRT_E_PASTE]);
    ledit->e_paste = e_paste;
    gtk_widget_show (e_paste);
    gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_paste);
    g_signal_connect ((gpointer) e_paste, "activate",
                    G_CALLBACK (lcrt_edit_on_paste_activate),
                    lwindow);
    gtk_widget_add_accelerator (e_paste, "activate", accel_group,
                                ledit->config.shortcut[LCRT_E_PASTE][0],
                               (GdkModifierType) ledit->config.shortcut[LCRT_E_PASTE][1],
                                GTK_ACCEL_VISIBLE);

    image236 = create_pixmap(NULL, "lcrt-paste.png");
    gtk_widget_show (image236);
    gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (e_paste), image236);

    e_copy_and_paste = gtk_menu_item_new_with_mnemonic (ledit->config.value[LCRT_E_COPY_AND_PASTE]);
    ledit->e_copy_and_paste = e_copy_and_paste;
    gtk_widget_show (e_copy_and_paste);
    gtk_widget_set_sensitive (e_copy_and_paste, has_selection);
    gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_copy_and_paste);
    g_signal_connect ((gpointer) e_copy_and_paste, "activate",
                    G_CALLBACK (lcrt_edit_on_copy_and_paste_activate),
                    lwindow);


    e_select_all = gtk_menu_item_new_with_mnemonic (ledit->config.value[LCRT_E_SELECT_ALL]);
    ledit->e_select_all = e_select_all;
    gtk_widget_show (e_select_all);
    gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_select_all);
    g_signal_connect ((gpointer) e_select_all, "activate",
                    G_CALLBACK (lcrt_edit_on_select_all_activate),
                    lwindow);

    e_find = gtk_image_menu_item_new_with_mnemonic (ledit->config.value[LCRT_E_FIND]);
    ledit->e_find = e_find;
    gtk_widget_show (e_find);
    gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_find);
    g_signal_connect ((gpointer) e_find, "activate",
                    G_CALLBACK (lcrt_edit_on_find_activate),
                    lwindow);
    gtk_widget_add_accelerator (e_find, "activate", accel_group,
                                ledit->config.shortcut[LCRT_E_FIND][0],
                               (GdkModifierType) ledit->config.shortcut[LCRT_E_FIND][1],
                                GTK_ACCEL_VISIBLE);

    image237 = gtk_image_new_from_stock ("gtk-find", GTK_ICON_SIZE_MENU);
    gtk_widget_show (image237);
    gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (e_find), image237);

    e_separator_1 = gtk_separator_menu_item_new ();
    gtk_widget_show (e_separator_1);
    gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_separator_1);
    gtk_widget_set_sensitive (e_separator_1, FALSE);

    e_clear_scrollback = gtk_menu_item_new_with_mnemonic (ledit->config.value[LCRT_E_CLEAR_SCROLLBACK]);
    ledit->e_clear_scrollback = e_clear_scrollback;
    gtk_widget_show (e_clear_scrollback);
    gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_clear_scrollback);
    g_signal_connect ((gpointer) e_clear_scrollback, "activate",
                    G_CALLBACK (lcrt_edit_on_clear_scrollback_activate),
                    lwindow);

    e_clear_screen = gtk_menu_item_new_with_mnemonic (ledit->config.value[LCRT_E_CLEAR_SCREEN]);
    ledit->e_clear_screen = e_clear_screen;
    gtk_widget_show (e_clear_screen);
    gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_clear_screen);
    g_signal_connect ((gpointer) e_clear_screen, "activate",
                    G_CALLBACK (lcrt_edit_on_clear_screen_activate),
                    lwindow);

    e_clear_screen_and_scrollback = 
        gtk_menu_item_new_with_mnemonic(ledit->config.value[LCRT_E_CLEAR_SCREEN_AND_SCROLLBACK]);
    ledit->e_clear_screen_and_scrollback = e_clear_screen_and_scrollback;
    gtk_widget_show (e_clear_screen_and_scrollback);
    gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_clear_screen_and_scrollback);
    g_signal_connect ((gpointer) e_clear_screen_and_scrollback, "activate",
                    G_CALLBACK (lcrt_edit_on_clear_screen_and_scrollback_activate),
                    lwindow);

    e_separator_2 = gtk_separator_menu_item_new ();
    gtk_widget_show (e_separator_2);
    gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_separator_2);
    gtk_widget_set_sensitive (e_separator_2, FALSE);

    e_reset = gtk_menu_item_new_with_mnemonic (ledit->config.value[LCRT_E_RESET]);
    ledit->e_reset = e_reset;
    gtk_widget_show (e_reset);
    gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_reset);
    g_signal_connect ((gpointer) e_reset, "activate",
                    G_CALLBACK (lcrt_edit_on_reset_activate),
                    lwindow);
#ifdef LCRT_SHOW_MENUBAR
    if (popup_menu) {
        e_separator_3 = gtk_separator_menu_item_new ();
        gtk_widget_show (e_separator_3);
        gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_separator_3);
        gtk_widget_set_sensitive (e_separator_3, FALSE);
        e_show_menubar = gtk_check_menu_item_new_with_mnemonic (ledit->config.value[LCRT_E_SHOW_MENUBAR]);

        gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (e_show_menubar), 
            parent->m_view->config.status[LCRT_V_MENUBAR]);
        gtk_widget_show (e_show_menubar);
        gtk_container_add (GTK_CONTAINER (e_menuitem_menu), e_show_menubar);
        g_signal_connect ((gpointer) e_show_menubar, "activate",
                    G_CALLBACK (lcrt_edit_on_show_menubar_activate),
                    lwindow);
    }
#endif
    return ledit;
}
void lcrt_edit_destroy_menuitem(struct lcrt_edit *ledit)
{
    gtk_widget_destroy(ledit->e_menuitem);
    if (ledit)
        free(ledit);
}

static const char *lcrt_edit_get_db_name(struct lcrt_edit *ledit)
{
    return ledit == NULL ? NULL : ledit->config.db.db_name;
}
static const char *lcrt_edit_get_tb_name(struct lcrt_edit *ledit)
{
    return ledit == NULL ? NULL : ledit->config.db.db_table;
}

int lcrt_edit_init_config(struct lcrt_edit *ledit)
{
    char db_name[256];
    int i;
    static char *name[LCRT_E_NUMBER] = {LCRT_E_NAME};
    static char *value[LCRT_E_NUMBER] = {LCRT_E_VALUE};

    if (ledit == NULL)
        return EINVAL;

    memset(&ledit->config, 0, sizeof(struct lcrtc_edit));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&ledit->config.db, db_name, LCRT_IEDIT_TABLE);
    for (i = 0; i < LCRT_E_NUMBER; i++) {
        ledit->config.name[i] = name[i];
        strncpy(ledit->config.value[i], value[i], sizeof(ledit->config.value[i]));
    }
    ledit->get_db = lcrt_edit_get_db_name;
    ledit->get_tb = lcrt_edit_get_tb_name;

    return 0;
}
int lcrt_edit_load_config(struct lcrt_edit *ledit)
{
    int rv, i;
    debug_where();
    struct lcrt_accels *laccels = ledit->parent->parent->w_accels;
    debug_where();
    if (ledit == NULL)
        return EINVAL;
    debug_where();
    memcpy(ledit->config.shortcut[LCRT_E_COPY], 
           laccels->config.key[LCRT_KB_E_COPY], 
           sizeof(int) * 2);
    memcpy(ledit->config.shortcut[LCRT_E_PASTE], 
           laccels->config.key[LCRT_KB_E_PASTE], 
           sizeof(int) * 2);
    memcpy(ledit->config.shortcut[LCRT_E_FIND], 
           laccels->config.key[LCRT_KB_E_FIND], 
           sizeof(int) * 2);

    debug_where();
    rv = ledit->config.db.select(&ledit->config.db, "SELECT * FROM %s", ledit->get_tb(ledit));
    if (rv == LCRTE_NO_TABLE) {
        return LCRTE_NO_CONFIG;
    }

    debug_where();
    for (i = 0; i < LCRT_E_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(ledit->config.value[i], ledit->config.db.get_text_col(&ledit->config.db, 1), 
                sizeof(ledit->config.value[i]));
        rv = ledit->config.db.get_row(&ledit->config.db);

        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s] shortcut = {%-5d, %d}\n",
                basename((char *)ledit->get_db(ledit)),
                ledit->get_tb(ledit),
                ledit->config.name[i],
                ledit->config.value[i],
                ledit->config.shortcut[i][0],
                ledit->config.shortcut[i][1]);
    }

    return rv;
}

