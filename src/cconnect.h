#ifndef __LCRT_CCONNECT_H__
#define __LCRT_CCONNECT_H__
#include <gtk/gtk.h>
void lcrt_connect_on_connectbutton_clicked(GtkButton *button, gpointer user_data);
void lcrt_connect_on_cancelbutton_clicked (GtkButton *button, gpointer user_data);
gboolean lcrt_connect_on_window_delete(GtkWidget *widget, GdkEvent  *event, gpointer user_data);
void lcrt_connect_on_quick_connect_activate(GtkWidget *toolitem, gpointer user_data);
void lcrt_connect_on_rename_activate(GtkWidget *toolitem, gpointer user_data);
void lcrt_connect_on_delete_activate(GtkWidget *toolitem, gpointer user_data);
void lcrt_connect_on_find_activate(GtkWidget *toolitem, gpointer user_data);
void lcrt_connect_on_session_option_activate(GtkWidget *toolitem, gpointer user_data);
gboolean lcrt_connect_on_button_press_event(GtkWidget *widget,
            GdkEventButton  *event, gpointer user_data);
void lcrt_connect_on_selection_changed(GtkWidget *selection, gpointer user_data);
#endif
