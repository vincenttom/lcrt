#ifndef __LCRT_CRENAME_H__
#define __LCRT_CRENAME_H__
#include <gtk/gtk.h>

void lcrt_find_on_prev_button_clicked(GtkButton *button,  gpointer user_data);
void lcrt_find_on_next_button_clicked(GtkButton *button,  gpointer user_data);
void lcrt_find_on_cancelbutton_clicked(GtkButton *button, gpointer user_data);
void lcrt_find_on_name_changed(GtkEditable *editable, gpointer user_data);
gboolean lcrt_find_on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer         user_data);

#endif
