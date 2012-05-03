//#define __LCRT_DEBUG__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "iwindow.h"
#include "debug.h"
#include "message.h"
#include "iterminal.h"
#include "ilock.h"
#include "clock.h"

void lcrt_lock_on_okbutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_lock *llock = (struct lcrt_lock *)user_data;
    struct lcrt_terminal *lterminal = llock->parent;
    char *password, *repassword;

    password   = (char *)gtk_entry_get_text(GTK_ENTRY(llock->entry_password));
    if (lterminal->locked == FALSE) {
        repassword = (char *)gtk_entry_get_text(GTK_ENTRY(llock->entry_repassword));
        if (strcmp(password, repassword) == 0) {

            strcpy(lterminal->lock_password, password);
            gtk_widget_set_sensitive(GTK_WIDGET(lterminal->scrolledwindow), FALSE);
            GdkColor xcolor_bg = { 0, 0xD3D3, 0xD3D3, 0xD3D3 };
            vte_terminal_set_color_background(lterminal->terminal, &xcolor_bg);
            lcrt_lock_on_cancelbutton_clicked(NULL, llock);
            lterminal->locked = TRUE;
        } else {

            lcrt_message_error(lterminal->parent->parent->window, llock->config.value[LCRT_K_ERROR]);
        }
    } else {

        if (strcmp(lterminal->lock_password, password) == 0) {
            GdkColor xcolor_bg = { 0, 0xFFFF, 0xFFFF, 0xFFFF };
            vte_terminal_set_color_background(lterminal->terminal, &xcolor_bg);
            gtk_widget_set_sensitive(GTK_WIDGET(lterminal->scrolledwindow), TRUE);
            lcrt_lock_on_cancelbutton_clicked(NULL, llock);
            lterminal->locked = FALSE;
        } else {
            lcrt_message_error(lterminal->parent->parent->window, llock->config.value[LCRT_K_ERROR]);
        }
    }
}

void lcrt_lock_on_cancelbutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_lock *llock = (struct lcrt_lock *)user_data;
    debug_where();
    gtk_widget_destroy(llock->dialog);
    lcrt_destroy_lock(llock);
}
gboolean lcrt_lock_on_window_delete(GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
    lcrt_lock_on_cancelbutton_clicked(NULL, user_data);
    return FALSE;
}
