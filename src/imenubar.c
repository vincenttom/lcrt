#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "iwindow.h"
#include "imenubar.h"
#include "ifile.h"
#include "iedit.h"
#include "iview.h"
#include "ioptions.h"
#include "ihelp.h"
#include "debug.h"

struct lcrt_menubar * lcrt_create_menubar(struct lcrt_window *parent)
{
    debug_where();
    struct lcrt_menubar *lmenubar;
    GtkWidget *menubar;

    lmenubar = (struct lcrt_menubar *)calloc(1, sizeof(struct lcrt_menubar));
    if (lmenubar == NULL)
        return NULL;

    lmenubar->parent = parent;

    menubar = gtk_menu_bar_new();
    lmenubar->menubar = menubar;
    gtk_widget_show (menubar);
    debug_where();
    lmenubar->m_file = lcrt_file_create_menuitem(lmenubar);
    assert(lmenubar->m_file);
    gtk_container_add(GTK_CONTAINER(menubar), lmenubar->m_file->f_menuitem);

    debug_where();
    lmenubar->m_edit = lcrt_edit_create_menuitem(lmenubar, FALSE);
    assert(lmenubar->m_edit);
    gtk_container_add(GTK_CONTAINER(menubar), lmenubar->m_edit->e_menuitem);

    debug_where();
    lmenubar->m_view = lcrt_view_create_menuitem(lmenubar);
    assert(lmenubar->m_view);
    gtk_container_add(GTK_CONTAINER(menubar), lmenubar->m_view->v_menuitem);

    debug_where();
    lmenubar->m_options = lcrt_options_create_menuitem(lmenubar);
    assert(lmenubar->m_options);
    gtk_container_add(GTK_CONTAINER(menubar), lmenubar->m_options->o_menuitem);

    debug_where();
    lmenubar->m_help = lcrt_help_create_menuitem(lmenubar);
    assert(lmenubar->m_help);
    gtk_container_add(GTK_CONTAINER(menubar), lmenubar->m_help->h_menuitem);

    return lmenubar;
}
void lcrt_destroy_menubar(struct lcrt_menubar *lmenubar)
{
    debug_where();
    if (lmenubar == NULL)
        return;
    lcrt_file_destroy_menuitem(lmenubar->m_file);
    lmenubar->m_file = NULL;

    lcrt_edit_destroy_menuitem(lmenubar->m_edit);
    lmenubar->m_edit = NULL;

    lcrt_view_destroy_menuitem(lmenubar->m_view);
    lmenubar->m_view = NULL;

    lcrt_options_destroy_menuitem(lmenubar->m_options);
    lmenubar->m_options = NULL;

    lcrt_help_destroy_menuitem(lmenubar->m_help);
    lmenubar->m_help = NULL;

    free(lmenubar);
}
