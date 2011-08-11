#ifndef __LCRT_ISTATUS_H__
#define __LCRT_ISTATUS_H__
#include <gtk/gtk.h>
#include "mkconfig.h"
#include "iwindow.h"
#include "user.h"

#define LCRT_ISTATUS_TABLE "istatus"
enum {
     LCRT_S_TOOLTIP,
     LCRT_S_SHOW_WINDOW,
     LCRT_S_QUIT,
     LCRT_S_WINDOW_EXIT,
     LCRT_S_SESSION_DISCONNECT,
     LCRT_S_NUMBER
};

#define  LCRT_S_NAME \
    "s_tooltip", \
    "s_show_status", \
    "s_quit", \
    "s_window_exit", \
    "s_session_disconnect"

#define  LCRT_S_VALUE \
    "Linux Remote login Tool", \
    "Main Window", \
    "Quit", \
    "There are %d session connected, do you want to quit ?", \
    "Do you wish to disconnect from %s ?"

struct lcrtc_status {
    struct lcrt_config db;
    const char *name[LCRT_S_NUMBER];
    char value[LCRT_S_NUMBER][LCRT_CONFIG_VALUE_LEN];
};

struct lcrt_status {
    struct lcrt_window *parent;

    GtkStatusIcon *status;
    GtkWidget *popup;


    struct lcrtc_status config;

    const char *(*get_db)(struct lcrt_status *lstatus);
    const char *(*get_tb)(struct lcrt_status *lstatus);
};

struct lcrt_status *lcrt_create_status(struct lcrt_window *parent);
void lcrt_destroy_status(struct lcrt_status *lstatus);
int lcrt_status_save_config(struct lcrt_status *lstatus);
GtkWidget* lcrt_status_create_popup_menu(struct lcrt_status *lstatus);
void lcrt_destroy_status(struct lcrt_status *lstatus);
#endif
