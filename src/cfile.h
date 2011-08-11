#ifndef __LCRT_CFILE_H__
#define __LCRT_CFILE_H__
#include <gtk/gtk.h>

void lcrt_file_on_connect_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_quick_connect_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_connect_in_tab_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_reconnect_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_reconnect_all_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_disconnect_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_disconnect_all_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_clone_session_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_lock_session_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_print_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_print_setup_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_log_session_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_recent_session_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_exit_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_file_on_menuitem_activate(GtkMenuItem *menuitem, gpointer user_data);

#endif
