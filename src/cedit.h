#ifndef __LCRT_CEDIT_H__
#define __LCRT_CEDIT_H__
#include <gtk/gtk.h>
void lcrt_edit_on_copy_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_edit_on_paste_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_edit_on_copy_and_paste_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_edit_on_select_all_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_edit_on_find_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_edit_on_clear_scrollback_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_edit_on_clear_screen_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_edit_on_clear_screen_and_scrollback_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_edit_on_reset_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_edit_on_show_menubar_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_edit_on_popup_menu_deactivate(GtkObject *object, gpointer user_data);
void lcrt_edit_on_menuitem_activate(GtkMenuItem *menuitem, gpointer user_data);
#endif
