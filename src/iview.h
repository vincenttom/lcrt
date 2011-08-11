#ifndef __LCRT_IVIEW_H__
#define __LCRT_IVIEW_H__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"

#define LCRT_IVIEW_TABLE "iview"

enum {
    LCRT_V_MENUITEM,
    LCRT_V_MENUBAR,
    LCRT_V_TOOLBAR,
    LCRT_V_STATUS_BAR,
    LCRT_V_ALWAYS_ON_TOP,
    LCRT_V_FULL_SCREEN,
    LCRT_V_NUMBER
};
#define LCRT_V_NAME \
    "v_menuitem", \
    "v_menubar", \
    "v_toolbar", \
    "v_status_bar", \
    "v_always_on_top", \
    "v_full_screen"

#define LCRT_V_VALUE \
    "_View", \
    "_Menu Bar", \
    "_Toolbar", \
    "_Status Bar", \
    "_Always on Top", \
    "_Full Screen"

#define LCRT_V_SHORTCUT \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {GDK_F11, 0}

struct lcrtc_view {
    struct lcrt_config db;
    const char *name[LCRT_V_NUMBER];
    char value[LCRT_V_NUMBER][LCRT_CONFIG_VALUE_LEN];
    int shortcut[LCRT_V_NUMBER][2];
};
struct lcrt_view {
    struct lcrt_menubar *parent;
    GtkWidget *v_menuitem;
    GtkWidget *v_menubar;
    GtkWidget *v_toolbar;
    GtkWidget *v_status_bar;
    GtkWidget *v_always_on_top;
    GtkWidget *v_full_screen;

    struct lcrtc_view config;
    const char *(*get_db)(struct lcrt_view *lview);
    const char *(*get_tb)(struct lcrt_view *lview);
};

struct lcrt_view *lcrt_view_create_menuitem(struct lcrt_menubar *parent);
void lcrt_view_destroy_menuitem(struct lcrt_view *lview);
int lcrt_view_save_local_config(struct lcrt_view *lview);
#endif
