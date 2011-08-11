#ifndef __LCRT_IPOPUP_H__
#define __LCRT_IPOPUP_H__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "imenubar.h"

#define LCRT_IPOPUP_TABLE "ipopup"

enum {
    LCRT_P_RENAME,
    LCRT_P_RECONNECT,
    LCRT_P_DISCONNECT,
    LCRT_P_CLOSE,
    LCRT_P_LOCK_SESSION,
    LCRT_P_UNLOCK_SESSION,
    LCRT_P_CLONE_SESSION,
    LCRT_P_SESSION_OPTIONS,
    LCRT_P_NUMBER
};
#define LCRT_P_NAME \
    "p_rename", \
    "p_reconnect", \
    "p_disconnect", \
    "p_close", \
    "p_lock_session", \
    "p_unlock_session", \
    "p_clone_session", \
    "p_session_options"

#define LCRT_P_VALUE \
    "Rena_me", \
    "_Reconnect", \
    "_Disconnect", \
    "_Close", \
    "Loc_k Session ...", \
    "Unloc_k Session ...", \
    "Clo_ne Session", \
    "Session _Options ..."

#define LCRT_P_SHORTCUT \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {GDK_W, GDK_CONTROL_MASK | GDK_SHIFT_MASK}

struct lcrtc_popup {
    struct lcrt_config db;
    const char *name[LCRT_P_NUMBER];
    char value[LCRT_P_NUMBER][LCRT_CONFIG_VALUE_LEN];
    int  shortcut[LCRT_P_NUMBER][2];
};
struct lcrt_popup {
    struct lcrt_terminal *parent;

    GtkWidget *popup_menu;
    GtkWidget *p_rename;
    GtkWidget *p_reconnect;
    GtkWidget *p_disconnect;
    GtkWidget *p_close;
    GtkWidget *p_lock_session;
    GtkWidget *p_clone_session;
    GtkWidget *p_session_options;

    struct lcrtc_popup config;
    const char *(*get_db)(struct lcrt_popup *lpopup);
    const char *(*get_tb)(struct lcrt_popup *lpopup);
};
struct lcrt_popup *lcrt_create_popup_menu(struct lcrt_terminal *parent);

#endif
