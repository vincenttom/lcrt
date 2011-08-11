#ifndef __LCRT_CLOCK_H__
#define __LCRT_CLOCK_H__
#include <gtk/gtk.h>

void lcrt_lock_on_cancelbutton_clicked(GtkButton *button, gpointer user_data);
void lcrt_lock_on_okbutton_clicked(GtkButton *button, gpointer user_data);
gboolean lcrt_lock_on_window_delete(GtkWidget *widget, GdkEvent  *event, gpointer user_data);

#endif
