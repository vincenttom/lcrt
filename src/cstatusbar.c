#include <gtk/gtk.h>
#include "istatusbar.h"
#include "debug.h"

void lcrt_statusbar_set_message(struct lcrt_statusbar *lstatusbar, const char *format, ...)
{
    unsigned int id;
    char buffer[1024];
    va_list args;

    if (lstatusbar == NULL)
        return;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    id = gtk_statusbar_get_context_id(GTK_STATUSBAR(lstatusbar->statusbar), buffer);
    gtk_statusbar_push(GTK_STATUSBAR(lstatusbar->statusbar), id, buffer);
}
void lcrt_statusbar_set_user(struct lcrt_statusbar *lstatusbar, struct lcrtc_user *user)
{
    char *proto[LCRT_PROTOCOL_NUMBER] = {LCRT_PROTOCOL_NAME};
    if (user == NULL)
        return;

     lcrt_statusbar_set_message(lstatusbar,
                                "[%s] %s@%s", 
                                proto[user->protocol],
                                user->username,
                                user->hostname);
}
