#ifndef __LCRT_CVIEW_H__
#define __LCRT_CVIEW_H__
#include <gtk/gtk.h>
void lcrt_view_on_menuitem_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_view_on_menubar_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_view_on_toolbar_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_view_on_session_tabs_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_view_on_status_bar_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_view_on_always_on_top_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_view_on_full_screen_activate(GtkMenuItem *menuitem, gpointer user_data);

#endif
