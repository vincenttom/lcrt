#define ___LCRT_DEBUG__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "iwindow.h"
#include "debug.h"
#include "message.h"
#include "iterminal.h"
#include "irename.h"
#include "crename.h"

void lcrt_rename_on_okbutton_clicked(GtkButton *button,  gpointer user_data)
{
    struct lcrt_rename *lrename = (struct lcrt_rename *)user_data;
    const char *name = gtk_entry_get_text(GTK_ENTRY(lrename->entry_rename));
    struct lcrtc_user *user, *new;

    if (lrename->rename_terminal == TRUE) {
        lcrt_terminal_set_status(lrename->parent->w_notebook->current_terminal,
                                 (char *)name,
                                 lrename->parent->w_notebook->current_terminal->connected);
        lcrt_rename_on_cancelbutton_clicked(NULL, user_data);
    } else {
        if ((user = lcrt_user_find_by_name(&lrename->parent->u_config, lrename->rename)) == NULL) {
            lcrt_rename_on_cancelbutton_clicked(NULL, user_data);
        } else {
            if ((new = lcrt_user_find_by_name(&lrename->parent->u_config, name)) != NULL) {
                if (user != new)
                    lcrt_message_error(lrename->dialog, lrename->config.value[LCRT_R_ERROR]);
                else
                    lcrt_rename_on_cancelbutton_clicked(NULL, user_data);
            } else {
                strncpy(lrename->parent->current_uname, name, HOSTNAME_LEN);
                lcrt_rename_on_cancelbutton_clicked(NULL, user_data);
            }
        }
        
    }
}

void lcrt_rename_on_cancelbutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_rename *lrename = (struct lcrt_rename *)user_data;
    gtk_widget_destroy(lrename->dialog);
    lcrt_destroy_rename(lrename);

}

void lcrt_rename_on_name_changed(GtkEditable *editable, gpointer user_data)
{
    struct lcrt_rename *lrename = (struct lcrt_rename *)user_data;
    if (gtk_entry_get_text_length(GTK_ENTRY(editable)) > 0)
        gtk_widget_set_sensitive(lrename->okbutton, TRUE);
    else
        gtk_widget_set_sensitive(lrename->okbutton, FALSE);

}
gboolean lcrt_rename_on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer         user_data)
{
    lcrt_rename_on_cancelbutton_clicked(NULL, user_data);

    return FALSE;
}


