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
/**
 * @brief main window structure. contain all it's children 
 *        elements and structures.
 */
struct lcrt_window {
    GtkWidget *window; /**< main window component */
    int full_screen; /**<  the flag to mark wethere is run in full screen */
    gboolean hide; /**<  the flag to mark wethere is hidden from desktop status bar */
    int q_status; /**< the flag to mark which button user has clicked in quick connect dialog.
                        GTK_RESPONSE_OK mean connect button, 
                        GTK_RESPONSE_CANCEL mean cancel button */

    GtkWidget *w_vbox;
    GtkAccelGroup *w_accel; /**< keyboard shortkey group */

    struct lcrt_menubar *w_menubar; /**< menubar component */
    struct lcrt_toolbar *w_toolbar; /**< toolbar component */
    struct lcrt_statusbar *w_statusbar; /**< statusbar component */
    struct lcrt_notebook *w_notebook; /**< notebook component */
    struct lcrt_status *w_status; /**< the status of main window */
    struct lcrt_accels *w_accels; /**< shortkey configuration */
    struct lcrt_settings *w_settings; /**< global settings component */

    struct lcrtc_window w_config; /**< databse of window*/
    struct lcrt_user u_config; /**< user list, contain informations of saved session*/
    struct lcrtc_user *current_user; /**< current session information,used to connect with remote*/
    char current_uname[HOSTNAME_LEN + 1]; /**< username of current session */
    char programe_name[512]; /**< ourself name */

    const char *(*get_db)(struct lcrt_window *lwindow);
    const char *(*get_tb)(struct lcrt_window *lwindow);
};

struct lcrt_window *lcrt_create_window(void);
int lcrt_window_save_config(struct lcrt_window *lwindow);
void lcrt_destroy_window(struct lcrt_window *lwindow);
int lcrt_window_set_current_user(struct lcrt_window *lwindow, struct lcrtc_user *user);
#endif
