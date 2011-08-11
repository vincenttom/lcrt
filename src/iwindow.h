#ifndef __LCRT_IMWINDOW_H__
#define __LCRT_IMWINDOW_H__
#include <gtk/gtk.h>
#include "mkconfig.h"
#include "user.h"

#define LCRT_WINDOW_CONFIG_DIR "etc/window"
#define LCRT_WINDOW_DB "window.db"
#define LCRT_IWINDOW_TABLE "iwindow"
enum {
    BUTTON_LEFT = 0x1,
    BUTTON_MID =0x2,
    BUTTON_RIGHT =0x3
};

enum {
    LCRT_W_START_X,
    LCRT_W_START_Y,
    LCRT_W_WIDTH,
    LCRT_W_HEIGHT,
    LCRT_W_NUMBER
};

#define LCRT_W_NAME \
    "w_start_x", \
    "w_start_y", \
    "w_width",   \
    "w_height"

#define LCRT_W_VALUE \
    0,   \
    0,   \
    800, \
    600

struct lcrtc_window {
    struct lcrt_config db;
    const char *name[LCRT_W_NUMBER];
    int value[LCRT_W_NUMBER];
};

struct lcrt_menubar;
struct lcrt_toolbar;
struct lcrt_statusbar;
struct lcrt_notebook;
struct lcrt_status;
struct lcrt_settings;
struct lcrt_accels;

struct lcrt_window {
    GtkWidget *window;
    int full_screen;
    gboolean hide;

    GtkWidget *w_vbox;
    GtkAccelGroup *w_accel;

    struct lcrt_menubar *w_menubar;
    struct lcrt_toolbar *w_toolbar;
    struct lcrt_statusbar *w_statusbar;
    struct lcrt_notebook *w_notebook;
    struct lcrt_status *w_status;
    struct lcrt_accels *w_accels;
    struct lcrt_settings *w_settings;

    struct lcrtc_window w_config;
    struct lcrt_user u_config;
    struct lcrtc_user *current_user;
    char current_uname[HOSTNAME_LEN + 1];
    char programe_name[512];

    const char *(*get_db)(struct lcrt_window *lwindow);
    const char *(*get_tb)(struct lcrt_window *lwindow);
};

struct lcrt_window *lcrt_create_window(void);
int lcrt_window_save_config(struct lcrt_window *lwindow);
void lcrt_destroy_window(struct lcrt_window *lwindow);
int lcrt_window_set_current_user(struct lcrt_window *lwindow, struct lcrtc_user *user);
#endif
