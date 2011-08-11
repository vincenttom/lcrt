#ifndef __LCRT_IOPTIONS_H__
#define __LCRT_IOPTIONS_H__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "imenubar.h"

#define LCRT_IOPTIONS_TABLE "ioptions"

enum {
    LCRT_O_MENUITEM,
    LCRT_O_SESSION_OPTIONS,
    LCRT_O_GLOBAL_OPTIONS,
    LCRT_O_AUTO_SAVE_OPTIONS,
    LCRT_O_SAVE_SETTINGS_NOW,
    LCRT_O_NUMBER
};
#define LCRT_O_NAME \
    "o_menuitem", \
    "o_session_options", \
    "o_global_options", \
    "o_auto_save_options", \
    "o_save_settings_now"

#define LCRT_O_VALUE \
    "_Options", \
    "_Session Options ...", \
    "_Global Options ...", \
    "_Auto Save Options", \
    "Save Settings _Now"

#define LCRT_O_SHORTCUT \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {0, 0}

struct lcrtc_options {
    struct lcrt_config db;
    const char *name[LCRT_O_NUMBER];
    char value[LCRT_O_NUMBER][LCRT_CONFIG_VALUE_LEN];
    int   shortcut[LCRT_O_NUMBER][2];
};
struct lcrt_options {
    struct lcrt_menubar *parent;

    GtkWidget *o_menuitem;
    GtkWidget *o_session_options;
    GtkWidget *o_global_options;
    GtkWidget *o_keyboard_shortcuts;
    GtkWidget *o_auto_save_options;
    GtkWidget *o_save_settings_now;

    struct lcrtc_options config;
    const char *(*get_db)(struct lcrt_options *loptions);
    const char *(*get_tb)(struct lcrt_options *loptions);
};

struct lcrt_options *lcrt_options_create_menuitem(struct lcrt_menubar *parent);
void lcrt_options_destroy_menuitem(struct lcrt_options *loptions);
#endif
