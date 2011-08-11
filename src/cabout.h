#ifndef __LCRT_CABOUT_H__
#define __LCRT_CABOUT_H__
#include <gtk/gtk.h>
void lcrt_about_url_hook(GtkAboutDialog *about, const char *link, gpointer user_data);
void lcrt_about_email_hook(GtkAboutDialog * about, const char *email_address, gpointer user_data);
#endif
