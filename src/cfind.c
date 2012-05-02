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
#include "iconnect.h"

static void lcrt_find_connect_callback(struct lcrt_find *lfind)
{
    const char *name = gtk_entry_get_text(GTK_ENTRY(lfind->entry_find));
    struct lcrtc_user *user;
    struct lcrt_connect *lconnect = (struct lcrt_connect *)lfind->parent;

    if ((user = lcrt_user_find_by_name(&lconnect->parent->u_config, name)) == NULL) {
        lcrt_message_info(lfind->dialog, lfind->config.value[LCRT_I_INFO]);
        return;
    }
    strncpy(lconnect->parent->current_uname, name, HOSTNAME_LEN);
    lcrt_connect_on_find_callback(lconnect);
}
static void lcrt_find_edit_callback(struct lcrt_find *lfind)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)lfind->parent;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;

}
void lcrt_find_on_prev_button_clicked(GtkButton *button,  gpointer user_data)
{
    struct lcrt_find *lfind = (struct lcrt_find *)user_data;
    
    switch (lfind->flag) {
    case LCRT_FIND_FCONNECT:
        lcrt_find_connect_callback(lfind);
        break;
    case LCRT_FIND_FEDIT:
        lcrt_find_edit_callback(lfind);
        break;
    }
}
void lcrt_find_on_next_button_clicked(GtkButton *button,  gpointer user_data)
{
    struct lcrt_find *lfind = (struct lcrt_find *)user_data;
    
    switch (lfind->flag) {
    case LCRT_FIND_FCONNECT:
        lcrt_find_connect_callback(lfind);
        break;
    case LCRT_FIND_FEDIT:
        lcrt_find_edit_callback(lfind);
        break;
    }
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
    if (gtk_entry_get_text_length(GTK_ENTRY(editable)) > 0) {
        switch (lfind->flag) {
        case LCRT_FIND_FCONNECT:
            gtk_widget_set_sensitive(lfind->prev_button, FALSE);
            break;
        case LCRT_FIND_FEDIT:
            gtk_widget_set_sensitive(lfind->prev_button, TRUE);
            break;
        }
        gtk_widget_set_sensitive(lfind->next_button, TRUE);
    } else {
        gtk_widget_set_sensitive(lfind->prev_button, FALSE);
        gtk_widget_set_sensitive(lfind->next_button, FALSE);
    }

}
gboolean lcrt_find_on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer         user_data)
{
    lcrt_find_on_cancelbutton_clicked(NULL, user_data);

    return FALSE;
}


