#ifndef __LCRT_CWINDOW_H__
#define __LCRT_CWINDOW_H__
#include <gtk/gtk.h>
gboolean lcrt_window_on_delete_event(GtkWidget *widget, GdkEvent  *event, gpointer user_data);
gboolean lcrt_window_on_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer user_data);
gboolean lcrt_window_on_state_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean lcrt_window_get_auto_save(struct lcrt_window *lwindow);
void lcrt_window_create_new(struct lcrt_window *lwindow);
gboolean lcrt_window_on_key_press_event(GtkWidget*widget,
            GdkEventKey *event, gpointer user_data);
#endif
