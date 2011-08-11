#define ___LCRT_DEBUG__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "iwindow.h"
#include "debug.h"
#include "message.h"
#include "iterminal.h"
#include "ifind.h"
#include "cfind.h"

void lcrt_find_on_okbutton_clicked(GtkButton *button,  gpointer user_data)
{
    struct lcrt_find *lfind = (struct lcrt_find *)user_data;
    const char *name = gtk_entry_get_text(GTK_ENTRY(lfind->entry_find));
    struct lcrtc_user *user;

    if ((user = lcrt_user_find_by_name(&lfind->parent->parent->u_config, name)) == NULL) {
        lcrt_message_info(lfind->dialog, lfind->config.value[LCRT_I_INFO]);
        return;
    }
    strncpy(lfind->parent->parent->current_uname, name, HOSTNAME_LEN);
    lcrt_find_on_cancelbutton_clicked(NULL, user_data);
}

void lcrt_find_on_cancelbutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_find *lfind = (struct lcrt_find *)user_data;
    gtk_widget_destroy(lfind->dialog);
    lcrt_destroy_find(lfind);
}

void lcrt_find_on_name_changed(GtkEditable *editable, gpointer user_data)
{
    struct lcrt_find *lfind = (struct lcrt_find *)user_data;
    if (gtk_entry_get_text_length(GTK_ENTRY(editable)) > 0)
        gtk_widget_set_sensitive(lfind->okbutton, TRUE);
    else
        gtk_widget_set_sensitive(lfind->okbutton, FALSE);

}
gboolean lcrt_find_on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer         user_data)
{
    lcrt_find_on_cancelbutton_clicked(NULL, user_data);

    return FALSE;
}


