#include <gtk/gtk.h>
#include <stdlib.h>
#include "iterminal.h"
#include "cterminal.h"
#include "inotebook.h"
#include "cnotebook.h"
#include "iwindow.h"
#include "imenubar.h"
#include "ipopup.h"
#include "ifile.h"
#include "cfile.h"
#include "debug.h"
#include "ilock.h"
#include "irename.h"
#include "coptions.h"

void lcrt_popup_on_rename_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    lcrt_create_dialog_rename(lwindow, NULL, NULL, TRUE);
}

void lcrt_popup_on_reconnect_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;

    lcrt_file_on_reconnect_activate(NULL, lwindow->w_menubar->m_file);
}

void lcrt_popup_on_disconnect_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;

    lcrt_file_on_disconnect_activate(NULL, lwindow->w_menubar->m_file);
}

void lcrt_popup_on_close_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;

    if (lwindow->w_notebook->current_terminal != NULL)
        lcrt_terminal_on_button_close_clicked(NULL, lwindow->w_notebook->current_terminal);
}

void lcrt_popup_on_lock_session_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    lcrt_create_dialog_lock(lwindow->w_notebook->current_terminal);
}

void lcrt_popup_on_clone_session_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    lcrt_file_on_clone_session_activate(NULL, lwindow->w_menubar->m_file);
}

void lcrt_popup_on_session_options_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    lcrt_options_on_session_options_activate(NULL, lwindow->w_menubar->m_options);
}
void lcrt_popup_on_deactivate(GtkObject *object, gpointer user_data)
{
    struct lcrt_popup *lpopup = (struct lcrt_popup *)user_data;
    debug_where();

    free(lpopup);
}
