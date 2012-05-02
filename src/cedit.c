//#define __LCRT_DEBUG__
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include "debug.h"
#include "iedit.h"
#include "imenubar.h"
#include "iwindow.h"
#include "inotebook.h"
#include "iterminal.h"
#include "cterminal.h"
#include "iview.h"
#include "ifind.h"

void lcrt_edit_on_menuitem_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_edit *ledit = lwindow->w_menubar->m_edit;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;
    gboolean has_selection, has_terminal;

    has_selection = lcrt_terminal_has_selection(lwindow);
    has_terminal = (lterminal != NULL);
    debug_where();

    gtk_widget_set_sensitive (ledit->e_copy, has_selection);
    gtk_widget_set_sensitive (ledit->e_paste, 
            has_terminal && lterminal->connected == LCRT_TERMINAL_CONNECTED);
    gtk_widget_set_sensitive (ledit->e_copy_and_paste, has_selection);
    
    gtk_widget_set_sensitive (ledit->e_find, has_terminal);

    gtk_widget_set_sensitive (ledit->e_select_all, has_terminal);
    gtk_widget_set_sensitive (ledit->e_clear_scrollback, has_terminal);
    gtk_widget_set_sensitive (ledit->e_clear_screen, has_terminal);
    gtk_widget_set_sensitive (ledit->e_clear_screen_and_scrollback, has_terminal);
    gtk_widget_set_sensitive (ledit->e_reset, has_terminal);
}
void lcrt_edit_on_copy_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;

    debug_where();
    if (lcrt_terminal_has_selection(lwindow) == FALSE)
        return;
    debug_where();
    vte_terminal_copy_clipboard(lterminal->terminal);
}


void lcrt_edit_on_paste_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;

    debug_where();
    if (lterminal == NULL || lterminal->connected != LCRT_TERMINAL_CONNECTED)
        return;

    debug_print("current_terminal = %p\n", lterminal);
    vte_terminal_paste_clipboard(lterminal->terminal);
}


void lcrt_edit_on_copy_and_paste_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;

    if (lterminal == NULL)
        return;

    debug_where();
    vte_terminal_copy_clipboard(lterminal->terminal);
    vte_terminal_paste_clipboard(lterminal->terminal);
}

void lcrt_edit_on_select_all_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;

    vte_terminal_select_all(lterminal->terminal);
}

void lcrt_edit_on_find_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;

    lcrt_create_dialog_find(lwindow, LCRT_FIND_FEDIT);
}


void lcrt_edit_on_clear_scrollback_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;
    GtkWidget *vscrollbar;

    vscrollbar = gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(lterminal->scrolledwindow));
#if 1
    gtk_adjustment_set_value(
            gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(lterminal->scrolledwindow)), 
            0);
#endif
}


void lcrt_edit_on_clear_screen_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;

    if (lterminal)
        //g_signal_emit_by_name(lterminal->terminal, "text-scrolled");
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
}


void lcrt_edit_on_clear_screen_and_scrollback_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;

    if (lterminal)
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);
}

void lcrt_edit_on_reset_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_terminal *lterminal = lwindow->w_notebook->current_terminal;

    if (lterminal)
        vte_terminal_reset(VTE_TERMINAL(lterminal->terminal), TRUE, TRUE);

}
#ifdef LCRT_SHOW_MENUBAR
void lcrt_edit_on_show_menubar_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_view *lview = lwindow->w_menubar->m_view;

    if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem)) == TRUE) {
        gtk_widget_show(lwindow->w_menubar->menubar);
        lview->config.status[LCRT_V_MENUBAR] = TRUE;
    } else {
        gtk_widget_hide(lwindow->w_menubar->menubar);
        lview->config.status[LCRT_V_MENUBAR] = FALSE;
    }
}
#endif
void lcrt_edit_on_popup_menu_deactivate(GtkObject *object, gpointer user_data)
{
    struct lcrt_edit *ledit = (struct lcrt_edit *)user_data;
    debug_where();

    free(ledit);
}
