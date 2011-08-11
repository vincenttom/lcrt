#ifndef _LCRT_MESSAGE_H
#define _LCRT_MESSAGE_H

#include <gtk/gtk.h>

void lcrt_message_warning(GtkWidget *window, const char *format, ...);
void lcrt_message_error(GtkWidget *window, const char *format, ...);
gboolean lcrt_message_choose(GtkWidget *window, GtkMessageType type, const char *format, ...);
void lcrt_message_info(GtkWidget *window, const char *format, ...);
#endif 
