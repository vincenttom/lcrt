/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Mon 17 Jan 2011 11:34:39 PM CST
 *
 * Description:  
 */
//#define __LCRT_DEBUG__
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "inotebook.h"
#include "iterminal.h"
#include "imenubar.h"
#include "cterminal.h"
#include "ifile.h"
#include "user.h"
#include "debug.h"
#include "cedit.h"
#include "itoolbar.h"
#include "cstatusbar.h"

void lcrt_notebook_on_switch_page(GtkNotebook *notebook, GtkNotebookPage *page,
                                  guint page_num, gpointer user_data)
{
    debug_where();
    struct lcrt_notebook *lnotebook = (struct lcrt_notebook *)user_data;
    struct lcrt_toolbar *ltoolbar = lnotebook->parent->w_toolbar;
    debug_where();
    struct lcrt_terminal *lterminal;
    debug_where();
    gboolean sensitive;
    GtkWidget *child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(lnotebook->notebook), page_num);

    debug_where();
    list_for_each_entry(lterminal, &lnotebook->child, brother) {
        debug_print("switch = %p %p %p\n", child, lterminal->scrolledwindow, lterminal);
        if (child == lterminal->scrolledwindow) {
            lnotebook->current_terminal = lterminal;
            lnotebook->parent->current_user = lterminal->user;
            lcrt_statusbar_set_user(lnotebook->parent->w_statusbar, lterminal->user);
            sensitive = lterminal->connected == LCRT_TERMINAL_CONNECTED;
            gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_RECONNECT], !sensitive);
            gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_DISCONNECT], sensitive);
            gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_PASTE], sensitive);
            break;
        }
    }
}
void lcrt_notebook_on_page_removed(GtkNotebook *notebook, GtkWidget *child,
                                   guint page_num, gpointer user_data)
{
    debug_where();
    struct lcrt_notebook *lnotebook = (struct lcrt_notebook *)user_data;
    struct lcrt_toolbar *ltoolbar = lnotebook->parent->w_toolbar;

    if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(lnotebook->notebook)) == 0) {
        debug_where();
        lnotebook->current_terminal = NULL;
        gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_RECONNECT], FALSE);
        gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_DISCONNECT], FALSE);
        gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_COPY], FALSE);
        gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_PASTE], FALSE);
    }
}
int lcrt_notebook_clone_terminal(gpointer user_data)
{
    struct lcrt_notebook *lnotebook = (struct lcrt_notebook *)user_data;
    struct lcrt_terminal *lterminal = lnotebook->current_terminal;

    if (lterminal == NULL || lterminal->connected != LCRT_TERMINAL_CONNECTED)
        return TRUE;
    int page_father, page_child;
    page_father = gtk_notebook_get_current_page(GTK_NOTEBOOK(lnotebook->notebook));
    lcrt_window_set_current_user(lterminal->parent->parent, lterminal->user);
    lcrt_create_terminal(lterminal->parent);
    page_child = gtk_notebook_get_current_page(GTK_NOTEBOOK(lnotebook->notebook));
    if (page_father + 1 != page_child)
        gtk_notebook_reorder_child(GTK_NOTEBOOK(lnotebook->notebook),
                gtk_notebook_get_nth_page(GTK_NOTEBOOK(lnotebook->notebook), page_child),
                page_father + 1);
    return TRUE;
}

int lcrt_notebook_control_terminal(GtkAccelGroup *accelgroup, 
                                  GObject *arg1, 
                                  guint key, 
                                  GdkModifierType key_mask, 
                                  gpointer user_data)
{
    struct lcrt_notebook *lnotebook = (struct lcrt_notebook *)user_data;
    GtkNotebook *notebook = GTK_NOTEBOOK(lnotebook->notebook);
    int current_page = gtk_notebook_get_current_page(notebook);
    int n_pages = gtk_notebook_get_n_pages(notebook);
    lcrt_nb_t nb;

    if (key_mask == GDK_MOD1_MASK && GDK_0 <= key && key <= GDK_9) {
        if (key - GDK_0 > n_pages || key - GDK_1 == current_page)
            return TRUE;
        gtk_notebook_set_current_page(notebook, key - GDK_1);
    }

    for (nb = 0; nb < LCRT_NB_NUMBER; nb++) {
        if (lnotebook->config.shortcut[nb][0] == key &&
            lnotebook->config.shortcut[nb][1] == key_mask)
            break;
    }
    if (nb == LCRT_NB_NUMBER || nb == LCRT_NB_NONE)
        return TRUE;
    
    switch (nb) {
    case LCRT_NB_COPY:
        lcrt_edit_on_copy_activate(NULL, lnotebook->parent);
        break;
    case LCRT_NB_PASTE:
        lcrt_edit_on_paste_activate(NULL, lnotebook->parent);
        break;
    case LCRT_NB_CLONE:
        lcrt_notebook_clone_terminal(lnotebook);
        break;
    case LCRT_NB_CLOSE:
        if (lnotebook->current_terminal == NULL)
            return TRUE;
        lcrt_terminal_on_button_close_clicked(NULL, lnotebook->current_terminal);
        break;
    case LCRT_NB_MOVE_NEXT:
        if (current_page < n_pages - 1) {
            gtk_notebook_reorder_child(notebook,
                                       gtk_notebook_get_nth_page(notebook, current_page),
                                       current_page + 1);
        }
        break;
    case LCRT_NB_MOVE_PREV:
        if (current_page > 0) {
            gtk_notebook_reorder_child(notebook,
                                       gtk_notebook_get_nth_page(notebook, current_page),
                                       current_page - 1);
        }
        break;
    case LCRT_NB_SWITCH_NEXT:
        if (current_page == n_pages - 1)
            gtk_notebook_set_current_page(notebook, 0);
        else
            gtk_notebook_next_page(notebook);
        break;
    case LCRT_NB_SWITCH_PREV:
        if (current_page == 0)
            gtk_notebook_set_current_page(notebook, n_pages - 1);
        else
            gtk_notebook_prev_page(notebook);
        break;
    case LCRT_NB_NONE:
    default:
        break;
    }

    return TRUE;
}

