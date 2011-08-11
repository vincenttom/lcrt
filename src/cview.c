#include <gtk/gtk.h>
#include <stdio.h>
#include "message.h"
#include "debug.h"
#include "iview.h"
#include "iwindow.h"
#include "itoolbar.h"
#include "imenubar.h"
#include "istatusbar.h"
#include "isettings.h"

#ifdef LCRT_SHOW_MENUBAR
void lcrt_view_on_menubar_activate(GtkMenuItem *menuitem, gpointer user_data)
{
}
#endif

void lcrt_view_on_menuitem_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_view *lview = (struct lcrt_view *)user_data;
    struct lcrt_settings *lsettings = lview->parent->parent->w_settings;

    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (lview->v_toolbar), 
            lsettings->lt_g_show_toolbar);
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (lview->v_status_bar), 
            lsettings->lt_g_show_statusbar);
}
void lcrt_view_on_toolbar_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_view *lview = (struct lcrt_view *)user_data;
    struct lcrt_settings *lsettings = lview->parent->parent->w_settings;
    gboolean active;

    active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(lview->v_toolbar));

    if (active == TRUE) {
        gtk_widget_show(lview->parent->parent->w_toolbar->toolbar);
    } else {
        gtk_widget_hide(lview->parent->parent->w_toolbar->toolbar);
    }
    lsettings->lt_g_show_toolbar = active;
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lsettings->g_cb_show_toolbar), 
            lsettings->lt_g_show_toolbar);
}


void lcrt_view_on_session_tabs_activate(GtkMenuItem *menuitem, gpointer user_data)
{

}

void lcrt_view_on_status_bar_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_view *lview = (struct lcrt_view *)user_data;
    struct lcrt_settings *lsettings = lview->parent->parent->w_settings;
    gboolean active;
    
    active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(lview->v_status_bar));

    if (active == TRUE) {
        gtk_widget_show(lview->parent->parent->w_statusbar->statusbar);
    } else {
        gtk_widget_hide(lview->parent->parent->w_statusbar->statusbar);
    }
    lsettings->lt_g_show_statusbar = active;
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lsettings->g_cb_show_statusbar), 
            lsettings->lt_g_show_statusbar);
}

void lcrt_view_on_always_on_top_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_view *lview = (struct lcrt_view *)user_data;

    if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(lview->v_always_on_top)) == TRUE) {
        gtk_window_set_keep_above(GTK_WINDOW(lview->parent->parent->window), TRUE);
    } else {
        gtk_window_set_keep_above(GTK_WINDOW(lview->parent->parent->window), FALSE);
    }
}


void lcrt_view_on_full_screen_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_view *lview = (struct lcrt_view *)user_data;

    if (lview->parent->parent->full_screen == FALSE) {
        gtk_window_fullscreen(GTK_WINDOW(lview->parent->parent->window));
        lview->parent->parent->full_screen = TRUE;
    } else {
        gtk_window_unfullscreen(GTK_WINDOW(lview->parent->parent->window));
        lview->parent->parent->full_screen = FALSE;
    }
}

