#include <gtk/gtk.h>
#include <stdarg.h>
#include "support.h"
#include "message.h"

void lcrt_message_warning(GtkWidget *window, const char *format, ...)
{
	GtkWidget *dialog;
    va_list args;
    char msg[4096];
    va_start(args, format);
    vsprintf(msg, format, args);
    va_end(args);

	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0,
				GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, msg,NULL);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);	
}
void lcrt_message_error(GtkWidget *window, const char *format, ...)
{
	GtkWidget *dialog;
    va_list args;
    char msg[4096];
    va_start(args, format);
    vsprintf(msg, format, args);
    va_end(args);

	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0,
				GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, msg,NULL);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
gboolean lcrt_message_choose(GtkWidget *window, GtkMessageType type, const char *format, ...)
{
	GtkWidget *dialog;
	int choose;
    va_list args;
    char msg[4096];
    va_start(args, format);
    vsprintf(msg, format, args);
    va_end(args);

	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0,
				type, GTK_BUTTONS_OK_CANCEL, msg,NULL);
	choose = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	if (choose == GTK_RESPONSE_OK)
		return TRUE;
	return FALSE;
}
void lcrt_message_info(GtkWidget *window, const char *format, ...)
{
	GtkWidget *dialog;
    va_list args;
    char msg[4096];
    va_start(args, format);
    vsprintf(msg, format, args);
    va_end(args);

	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0,
			GTK_MESSAGE_INFO, GTK_BUTTONS_OK, msg,NULL);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);	
}


