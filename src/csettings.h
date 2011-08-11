#ifndef __LCRT_CSETTINGS_H__
#define __LCRT_CSETTINGS_H__
#include <gtk/gtk.h>

void lcrt_settings_on_applybutton_clicked(GtkButton *button, gpointer user_data);
void lcrt_settings_on_cancelbutton_clicked(GtkButton *button, gpointer user_data);
void lcrt_settings_on_okbutton_clicked(GtkButton *button, gpointer user_data);
void lcrt_settings_on_button_background_image_clicked(GtkButton *button, gpointer user_data);
gboolean lcrt_settings_on_window_delete(GtkWidget *widget, GdkEvent  *event, gpointer user_data);
void lcrt_settings_terminal_font_button_clicked(GtkFontButton *widget, gpointer user_data);
void lcrt_settings_terminal_color_button_clicked(GtkColorButton *widget, gpointer user_data);
void lcrt_settings_terminal_on_restore_default_clicked(GtkButton *widget, gpointer user_data);
void lcrt_settings_terminal_on_transparent_value_changed(GtkButton *widget, gpointer user_data);
int lcrt_file_choose_dialog(GtkWidget *parent, const char *prompt, char *file);
#endif
