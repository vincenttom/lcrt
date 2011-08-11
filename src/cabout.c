/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Wed 02 Mar 2011 08:08:32 PM CST
 *
 * Description:  
 */
#include <gtk/gtk.h>

void lcrt_about_url_hook(GtkAboutDialog *about, const char *link, gpointer user_data)
{
    gtk_show_uri(gtk_widget_get_screen(GTK_WIDGET(about)), link, gtk_get_current_event_time(), NULL);
}

void lcrt_about_email_hook(GtkAboutDialog * about, const char *email_address, gpointer user_data)
{
    char *escaped, *uri;

    escaped = g_uri_escape_string(email_address, NULL, FALSE);
    uri = g_strdup_printf("mailto:%s", escaped);
    g_free(escaped);

    lcrt_about_url_hook(about, uri, user_data);
    g_free(uri);
}
