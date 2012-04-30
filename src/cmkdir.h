#ifndef __LCRT_CMKDIR_H__
#define __LCRT_CMKDIR_H__
#include <gtk/gtk.h>

void lcrt_mkdir_on_okbutton_clicked(GtkButton *button,  gpointer user_data);
void lcrt_mkdir_on_cancelbutton_clicked(GtkButton *button, gpointer user_data);
void lcrt_mkdir_on_name_changed(GtkEditable *editable, gpointer user_data);
gboolean lcrt_mkdir_on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer         user_data);

#endif
