#ifndef __LCRT_IHELP_H__
#define __LCRT_IHELP_H__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "imenubar.h"

#define LCRT_IHELP_TABLE "ihelp"

enum {
    LCRT_H_MENUITEM,
    LCRT_H_HELP,
    LCRT_H_HOME_PAGE,
    LCRT_H_DOWNLOAD_PAGE,
    LCRT_H_ABOUT,
    LCRT_H_MISSING_DOC,
    LCRT_H_NO_NEW_VERSION,
    LCRT_H_NEW_VERSION,
    LCRT_H_CHECK_UPDATE_ERR,
    LCRT_H_NUMBER
};
#define LCRT_H_NAME \
    "h_menuitem", \
    "h_help", \
    "h_home_page", \
    "h_download_page", \
    "h_about", \
    "h_missing_doc", \
    "h_no_new_version",\
    "h_new_version", \
    "h_check_update_err"

#define LCRT_H_VALUE \
    "_Help", \
    "help", \
    "_Home Page", \
    "_Download new version", \
    "_About", \
    "Missing help document!", \
    "There is not new version!", \
    "There is a new verison:%s\nDownload page: %s", \
    "Check update error: may be network is unreachable!"

#define LCRT_H_SHORTCUT \
    {0, 0}, \
    {GDK_F1, 0}, \
    {0, 0}, \
    {0, 0}

struct lcrtc_help {
    struct lcrt_config db;
    const char *name[LCRT_H_NUMBER];
    char value[LCRT_H_NUMBER][LCRT_CONFIG_VALUE_LEN];
    int   shortcut[LCRT_H_NUMBER][2];
};
struct lcrt_help {
    struct lcrt_menubar *parent;

    GtkWidget *h_menuitem;
    GtkWidget *h_help;
    GtkWidget *h_home_page;
    GtkWidget *h_download_page;
    GtkWidget *h_about;

    struct lcrtc_help config;
    const char *(*get_db)(struct lcrt_help *ledit);
    const char *(*get_tb)(struct lcrt_help *ledit);
};

struct lcrt_help *lcrt_help_create_menuitem(struct lcrt_menubar *parent);
void lcrt_help_destroy_menuitem(struct lcrt_help *lhelp);
#endif
