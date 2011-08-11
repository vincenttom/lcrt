#include <gtk/gtk.h>
#include <stdio.h>
#include "iwindow.h"
#include "cwindow.h"
#include "imenubar.h"
#include "message.h"
#include "debug.h"
#include "ioptions.h"
#include "iqconnect.h"
#include "isettings.h"
#include "inotebook.h"

void lcrt_options_on_session_options_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_options *loptions = (struct lcrt_options *)user_data;
    struct lcrt_settings *lsettings = loptions->parent->parent->w_settings;
    int rv;

    if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(lsettings->parent->w_notebook->notebook)) == 0)
        return;
#if 0
    lsettings->session = TRUE;
    gtk_notebook_set_current_page(GTK_NOTEBOOK(lsettings->notebook), 2);
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(lsettings->notebook), FALSE);
    gtk_widget_show(lsettings->dialog_settings);
#else
    lcrt_create_qconnect(loptions->parent->parent, 
                         NULL, 
                         LCRT_QCONNECT_SESSION_OPTION, 
                         loptions->parent->parent->current_user->name);

#endif
}

void lcrt_options_on_global_options_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_options *loptions = (struct lcrt_options *)user_data;
    gtk_widget_show(loptions->parent->parent->w_settings->dialog_settings);
}

void lcrt_options_on_auto_save_options_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_options *loptions = (struct lcrt_options *)user_data;
    loptions->parent->parent->w_settings->auto_save_config =
        gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
}

void lcrt_options_on_save_settings_now_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_options *loptions = (struct lcrt_options *)user_data;
    lcrt_settings_save_local_config(loptions->parent->parent->w_settings);
}


