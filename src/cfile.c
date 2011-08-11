//#define __LCRT_DEBUG__
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "message.h"
#include "debug.h"
#include "ifile.h"
#include "iqconnect.h"
#include "imenubar.h"
#include "iwindow.h"
#include "iconnect.h"
#include "iterminal.h"
#include "cwindow.h"
#include "ilock.h"

void lcrt_file_on_menuitem_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_file *lfile = lwindow->w_menubar->m_file;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;

    int has_terminal = (lterminal != NULL);

    gtk_widget_set_sensitive(lfile->f_reconnect, has_terminal);
    gtk_widget_set_sensitive(lfile->f_disconnect, has_terminal);
    gtk_widget_set_sensitive(lfile->f_lock_session, has_terminal);
    gtk_widget_set_sensitive(lfile->f_clone_session, has_terminal);

    if (has_terminal && lterminal->connected == LCRT_TERMINAL_CONNECTED)
        gtk_widget_set_sensitive(lfile->f_reconnect, FALSE);
    else
        gtk_widget_set_sensitive(lfile->f_disconnect, FALSE);

    has_terminal = (gtk_notebook_get_n_pages(GTK_NOTEBOOK(lwindow->w_notebook->notebook)) != 0);

    gtk_widget_set_sensitive(lfile->f_reconnect_all, has_terminal);
    gtk_widget_set_sensitive(lfile->f_disconnect_all, has_terminal);
}

void lcrt_file_on_connect_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_file *lfile = (struct lcrt_file *)user_data;
    struct lcrt_window *lwindow = lfile->parent->parent;

    int rv = lcrt_create_connect(lwindow, FALSE);
    debug_print("rv = %d\n", rv);
    if (rv != GTK_RESPONSE_OK && rv != GTK_RESPONSE_NONE)
        return;
    debug_where();
    lcrt_window_create_new(lwindow);
}

void lcrt_file_on_quick_connect_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_file *lfile = (struct lcrt_file *)user_data;
    lcrt_create_qconnect(lfile->parent->parent, NULL, LCRT_QCONNECT_IN_TAB, NULL);
}

void lcrt_file_on_connect_in_tab_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_file *lfile = (struct lcrt_file *)user_data;
    lcrt_create_connect(lfile->parent->parent, TRUE);
}

void lcrt_file_on_reconnect_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_file *lfile = (struct lcrt_file *)user_data;
    struct lcrt_terminal *lterminal = lfile->parent->parent->w_notebook->current_terminal;

    if (lterminal == NULL || lterminal->connected != LCRT_TERMINAL_DISCONNECT)
        return;

    lcrt_terminal_fork(lterminal);
}


void lcrt_file_on_reconnect_all_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_file *lfile = (struct lcrt_file *)user_data;
    struct lcrt_notebook *lnotebook = lfile->parent->parent->w_notebook;

    struct lcrt_terminal *lterminal;
    list_for_each_entry(lterminal, &lnotebook->child, brother) {
        if (lterminal->connected == LCRT_TERMINAL_CONNECTED) {
            vte_terminal_feed_child(lterminal->terminal, 
                LCRT_TERMINAL_EXIT_CMD, strlen(LCRT_TERMINAL_EXIT_CMD));
        }
        lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_DISCONNECT);
        lcrt_terminal_fork(lterminal);
    }
}


void lcrt_file_on_disconnect_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_file *lfile = (struct lcrt_file *)user_data;
    struct lcrt_terminal *lterminal = lfile->parent->parent->w_notebook->current_terminal;

    if (lterminal == NULL || lterminal->connected != LCRT_TERMINAL_CONNECTED)
        return;

     vte_terminal_feed_child(lterminal->terminal, 
                LCRT_TERMINAL_EXIT_CMD, strlen(LCRT_TERMINAL_EXIT_CMD));
}


void lcrt_file_on_disconnect_all_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_file *lfile = (struct lcrt_file *)user_data;
    struct lcrt_notebook *lnotebook = lfile->parent->parent->w_notebook;

    struct lcrt_terminal *lterminal;
    list_for_each_entry(lterminal, &lnotebook->child, brother) {
        if (lterminal->connected) {
            vte_terminal_feed_child(lterminal->terminal, 
                LCRT_TERMINAL_EXIT_CMD, strlen(LCRT_TERMINAL_EXIT_CMD));
        }
        lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_DISCONNECT);
    }
}


void lcrt_file_on_clone_session_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_file *lfile = (struct lcrt_file *)user_data;
    struct lcrt_notebook *lnotebook = lfile->parent->parent->w_notebook;
    struct lcrt_terminal *lterminal = lnotebook->current_terminal;

    if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(lnotebook->notebook)) == 0 ||
        lterminal == NULL || lterminal->connected != LCRT_TERMINAL_CONNECTED)
        return;

    lcrt_window_set_current_user(lterminal->parent->parent, lterminal->user);
    lcrt_create_terminal(lterminal->parent);
}


void lcrt_file_on_lock_session_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_file *lfile = (struct lcrt_file *)user_data;
    struct lcrt_notebook *lnotebook = lfile->parent->parent->w_notebook;
    struct lcrt_terminal *lterminal = lnotebook->current_terminal;

    if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(lnotebook->notebook)) == 0 ||
        lterminal == NULL || lterminal->connected != LCRT_TERMINAL_CONNECTED)
        return;
    lcrt_create_dialog_lock(lterminal);
}


void lcrt_file_on_print_activate(GtkMenuItem *menuitem, gpointer user_data)
{

}


void lcrt_file_on_print_setup_activate(GtkMenuItem *menuitem, gpointer user_data)
{

}


void lcrt_file_on_log_session_activate(GtkMenuItem *menuitem, gpointer user_data)
{

}


void lcrt_file_on_recent_session_activate(GtkMenuItem *menuitem, gpointer user_data)
{

}


void lcrt_file_on_exit_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_file *lfile = (struct lcrt_file *)user_data;

    lcrt_window_on_delete_event(NULL, NULL, lfile->parent->parent);
}
