#ifndef __LCRT_IEDIT_H__
#define __LCRT_IEDIT_H__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "imenubar.h"

#define LCRT_IEDIT_TABLE "iedit"

enum {
    LCRT_E_MENUITEM,
    LCRT_E_COPY,
    LCRT_E_PASTE,
    LCRT_E_COPY_AND_PASTE,
    LCRT_E_SELECT_ALL,
    LCRT_E_FIND,
    LCRT_E_CLEAR_SCROLLBACK,
    LCRT_E_CLEAR_SCREEN,
    LCRT_E_CLEAR_SCREEN_AND_SCROLLBACK,
    LCRT_E_RESET,
    LCRT_E_SHOW_MENUBAR,
    LCRT_E_NUMBER
};
#define LCRT_E_NAME \
    "e_menuitem", \
    "e_copy", \
    "e_paste", \
    "e_copy_and_paste", \
    "e_select_all", \
    "e_find", \
    "e_clear_scrollback", \
    "e_clear_screen", \
    "e_clear_screen_and_scrollback", \
    "e_reset", \
    "e_show_menubar"

#define LCRT_E_VALUE \
    "_Edit", \
    "_Copy", \
    "_Paste", \
    "C_opy and Paste", \
    "Select _All", \
    "_Find", \
    "Clear Scroll_back", \
    "Clear _Screen", \
    "Clear Screen and Scrollbac_k", \
    "_Reset", \
    "Show Menubar"

struct lcrtc_edit {
    struct lcrt_config db;
    const char *name[LCRT_E_NUMBER];
    char value[LCRT_E_NUMBER][LCRT_CONFIG_VALUE_LEN];
    int   shortcut[LCRT_E_NUMBER][2];
};
struct lcrt_edit {
    struct lcrt_menubar *parent;

    GtkWidget *e_menuitem;
    GtkWidget *e_copy;
    GtkWidget *e_paste;
    GtkWidget *e_copy_and_paste;
    GtkWidget *e_select_all;
    GtkWidget *e_find;
    GtkWidget *e_clear_scrollback;
    GtkWidget *e_clear_screen;
    GtkWidget *e_clear_screen_and_scrollback;
    GtkWidget *e_reset;

    struct lcrtc_edit config;
    const char *(*get_db)(struct lcrt_edit *ledit);
    const char *(*get_tb)(struct lcrt_edit *ledit);
};
struct lcrt_edit *lcrt_edit_create_menuitem(struct lcrt_menubar *parent, gboolean popup_menu);
void lcrt_edit_destroy_menuitem(struct lcrt_edit *ledit);

#endif
