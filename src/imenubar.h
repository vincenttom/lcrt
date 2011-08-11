#ifndef __LCRT_IMENUBER_H__
#define __LCRT_IMENUBER_H__
#include <gtk/gtk.h>
#include "iwindow.h"

#define LCRT_MENUBAR_CONFIG_DIR "etc/menubar"
#define LCRT_MENUBAR_DB "menubar.db"

struct lcrt_file;
struct lcrt_edit;
struct lcrt_view;
struct lcrt_options;
struct lcrt_help;

struct lcrt_menubar {
    struct lcrt_window *parent;

    GtkWidget *menubar;

    struct lcrt_file *m_file;
    struct lcrt_edit *m_edit;
    struct lcrt_view *m_view;
    struct lcrt_options *m_options;
    struct lcrt_help *m_help;
};

struct lcrt_menubar * lcrt_create_menubar(struct lcrt_window *parent);
void lcrt_destroy_menubar(struct lcrt_menubar *lmenubar);
#endif
