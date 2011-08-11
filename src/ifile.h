#ifndef __LCRT_IFILE_H__
#define __LCRT_IFILE_H__

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "imenubar.h"

#define LCRT_IFILE_TABLE "ifile"

enum {
    LCRT_F_MENUITEM,
    LCRT_F_CONNECT,
    LCRT_F_QUICK_CONNECT,
    LCRT_F_CONNECT_IN_TAB,
    LCRT_F_RECONNECT,
    LCRT_F_RECONNECT_ALL,
    LCRT_F_DISCONNECT,
    LCRT_F_DISCONNECT_ALL,
    LCRT_F_CLONE_SESSION,
    LCRT_F_LOCK_SESSION,
    LCRT_F_PRINT,
    LCRT_F_PRINT_SETUP,
    LCRT_F_LOG_SESSION,
    LCRT_F_RECENT_SESSION,
    LCRT_F_EXIT,
    LCRT_F_NUMBER
};
#define LCRT_F_NAME \
    "f_menuitem", \
    "f_connect", \
    "f_quick_connect", \
    "f_connect_in_tab", \
    "f_reconnect", \
    "f_reconnect_all", \
    "f_disconnect", \
    "f_disconnect_all", \
    "f_clone_session", \
    "f_lock_session", \
    "f_print", \
    "f_print_setup", \
    "f_log_session", \
    "f_recent_session", \
    "f_exit"
#define LCRT_F_VALUE \
    "_File", \
    "_Connect ...", \
    "_Quick Connect ...", \
    "Connect in Ta_b", \
    "Reconnect", \
    "Reconnect _All", \
    "_Disconnect", \
    "Disc_onnect All", \
    "Clo_ne Session", \
    "Loc_k Session ...", \
    "_Print", \
    "Print Set_up ...", \
    "_Log Session", \
    "Recent Session", \
    "E_xit"

#define LCRT_F_ICONS \
    "", \
    "lcrt-connect.png", \
    "lcrt-quick-connect.png", \
    "lcrt-connect-in-tab.png", \
    "lcrt-reconnect.png"
struct lcrtc_file {
    struct lcrt_config db;
    const char *name[LCRT_F_NUMBER];
    char value[LCRT_F_NUMBER][LCRT_CONFIG_VALUE_LEN];
    int   shortcut[LCRT_F_NUMBER][2];
};
struct lcrt_file {
    struct lcrt_menubar *parent;
    GtkWidget *f_menuitem;
    GtkWidget *f_connect;
    GtkWidget *f_quick_connect;
    GtkWidget *f_connect_in_tab;
    GtkWidget *f_reconnect;
    GtkWidget *f_reconnect_all;
    GtkWidget *f_disconnect;
    GtkWidget *f_disconnect_all;
    GtkWidget *f_clone_session;
    GtkWidget *f_lock_session;
    GtkWidget *f_print;
    GtkWidget *f_print_setup;
    GtkWidget *f_log_session;
    GtkWidget *f_recent_session;
    GtkWidget *f_exit;
    struct lcrtc_file config;
    const char *(*get_db)(struct lcrt_file *lfile);
    const char *(*get_tb)(struct lcrt_file *lfile);
};

struct lcrt_file *lcrt_file_create_menuitem(struct lcrt_menubar *parent);
void lcrt_file_destroy_menuitem(struct lcrt_file *lfile);
#endif
