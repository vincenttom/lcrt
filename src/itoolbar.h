#ifndef __LCRT_ITOOLBAR_H__
#define __LCRT_ITOOLBAR_H__

#include <gtk/gtk.h>
#include "mkconfig.h"
#include "iwindow.h"
#include "list.h"

#define LCRT_ITOOLBAR_TABLE "itoolbar"

enum {
    LCRT_B_CONNECT,
    LCRT_B_QUICK_CONNECT,
    LCRT_B_CONNECT_IN_TAB,
    LCRT_B_RECONNECT,
    LCRT_B_DISCONNECT,
    LCRT_B_COPY,
    LCRT_B_PASTE,
    LCRT_B_TERMINAL,
    LCRT_B_NUMBER
};
#define LCRT_B_NAME \
    "b_connect", \
    "b_quick_connect", \
    "b_connect_on_tab", \
    "b_reconnect",\
    "b_disconnect", \
    "b_copy", \
    "b_paste", \
    "b_terminal"

#define LCRT_B_VALUE \
    "Connect", \
    "Quick connect", \
    "Conect in tab", \
    "Reconnect current session", \
    "Disconnect current session", \
    "Copy", \
    "Paste", \
    "Open a terminal"

#define LCRT_B_ICONS \
    "lcrt-connect.png", \
    "lcrt-quick-connect.png", \
    "lcrt-connect-in-tab.png", \
    "lcrt-reconnect.png", \
    "lcrt-disconnect.png", \
    "lcrt-copy.png", \
    "lcrt-paste.png", \
    "lcrt-terminal.png"

#define LCRT_B_CALLBACK \
    lcrt_toolbar_on_connect_activate, \
    lcrt_toolbar_on_quick_connect_activate, \
    lcrt_toolbar_on_connect_in_tab_activate, \
    lcrt_toolbar_on_reconnect_activate,\
    lcrt_toolbar_on_disconnect_activate,\
    lcrt_toolbar_on_copy_activate,\
    lcrt_toolbar_on_paste_activate,\
    lcrt_toolbar_on_terminal_activate

struct lcrt_toolitem {
    struct lcrt_toolbar *parent;
};
struct lcrtc_toolbar {
    struct lcrt_config db;
    const char *name[LCRT_B_NUMBER];
    char value[LCRT_B_NUMBER][LCRT_CONFIG_VALUE_LEN];
};
struct lcrt_toolbar {
    struct lcrt_window *parent;
    GtkWidget *toolbar;
    GtkWidget *toolitem[LCRT_B_NUMBER];

    struct lcrtc_toolbar config;
    const char *(*get_db)(struct lcrt_toolbar *ltoolbar);
    const char *(*get_tb)(struct lcrt_toolbar *ltoolbar);
};

struct lcrt_toolbar *lcrt_create_toolbar(struct lcrt_window *parent);
void lcrt_destroy_toolbar(struct lcrt_toolbar *ltoolbar);

#endif
