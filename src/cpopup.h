#ifndef LCRT_CPOPUP_H__
#define LCRT_CPOPUP_H__
#include <gtk/gtk.h>
void lcrt_popup_on_rename_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_popup_on_reconnect_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_popup_on_disconnect_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_popup_on_close_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_popup_on_lock_session_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_popup_on_clone_session_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_popup_on_session_options_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_popup_on_deactivate(GtkObject *object, gpointer user_data);
#endif
