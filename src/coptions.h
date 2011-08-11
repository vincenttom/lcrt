#ifndef __LCRT_COPTIONS_H__
#define __LCRT_COPTIONS_H__
#include <gtk/gtk.h>
void lcrt_options_on_session_options_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_options_on_global_options_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_options_on_auto_save_options_activate(GtkMenuItem *menuitem, gpointer user_data);
void lcrt_options_on_save_settings_now_activate(GtkMenuItem *menuitem, gpointer user_data);

#endif
