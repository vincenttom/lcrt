//#define __LCRT_DEBUG__
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
static void lcrt_find_edit_callback(struct lcrt_find *lfind, int next)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)lfind->parent;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;

    const char *text, *pattern;

#if VTE_CHECK_VERSION(0, 26, 0)
    if (lfind->find_regex == 0) {
        text = gtk_entry_get_text(GTK_ENTRY(lfind->entry_find));
        pattern = g_regex_escape_string (text, -1);
        if (lfind->regex)
            g_regex_unref (lfind->regex);
        lfind->regex = g_regex_new(pattern, G_REGEX_CASELESS, 0, NULL);
        vte_terminal_search_set_gregex(VTE_TERMINAL(lterminal->terminal), lfind->regex);
        lfind->find_regex = 1;
    }
    if (next)
        vte_terminal_search_find_next(VTE_TERMINAL(lterminal->terminal));
    else
        vte_terminal_search_find_previous(VTE_TERMINAL(lterminal->terminal));
#else
    lcrt_message_info(lfind->parent_window, "Current VTE library is too old\n");
#endif
}
void lcrt_find_on_prev_button_clicked(GtkButton *button,  gpointer user_data)
{
    struct lcrt_find *lfind = (struct lcrt_find *)user_data;
    
    switch (lfind->flag) {
    case LCRT_FIND_FCONNECT:
        lcrt_find_connect_callback(lfind);
        break;
    case LCRT_FIND_FEDIT:
        lcrt_find_edit_callback(lfind, 0);
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
        lcrt_find_edit_callback(lfind, 1);
        break;
    }
}

void lcrt_find_on_cancelbutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_find *lfind = (struct lcrt_find *)user_data;

    if (lfind->flag == LCRT_FIND_FEDIT) {
            if (lfind->find_regex && lfind->regex)
                g_regex_unref (lfind->regex);
    }
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
            lfind->find_regex = 0;
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


