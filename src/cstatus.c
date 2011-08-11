#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <assert.h>
#include "iwindow.h"
#include "cwindow.h"
#include "istatus.h"
#include "debug.h"
#include "mkconfig.h"

void lcrt_status_on_activate(GtkStatusIcon *status_icon, gpointer user_data)
{
    struct lcrt_status *lstatus = (struct lcrt_status *)user_data;
    struct lcrt_window *lwindow = lstatus->parent;
    debug_where();
    if (lwindow->hide == FALSE)
        gtk_widget_hide(lwindow->window);
    else {
        gtk_window_deiconify(GTK_WINDOW(lwindow->window));
        gtk_widget_show(lwindow->window);
    }
    lwindow->hide = !lwindow->hide;
}

gboolean lcrt_status_on_button_press_event(
        GtkStatusIcon  *status_icon,
        GdkEventButton *event,
        gpointer user_data)
{
    struct lcrt_status *lstatus = (struct lcrt_status *)user_data;

    if (event->type == GDK_BUTTON_PRESS && event->button == BUTTON_RIGHT) {
		GtkWidget *popup_menu = lcrt_status_create_popup_menu(lstatus);
		gtk_menu_popup (GTK_MENU(popup_menu),
			     NULL, NULL, NULL, NULL,
			     event->button, event->time);
        debug_where();
    }
    return FALSE;
}

void lcrt_status_on_show_window_active(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_status *lstatus = (struct lcrt_status *)user_data;
    
    if (lstatus->parent->hide == TRUE)
        lcrt_status_on_activate(NULL, lstatus);

}

void lcrt_status_on_quit_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_status *lstatus = (struct lcrt_status *)user_data;

    lcrt_window_on_delete_event(NULL, NULL, lstatus->parent);
}

