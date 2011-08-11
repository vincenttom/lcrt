#ifndef __LCRT_IRENAME_H__
#define __LCRT_IRENAME_H__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "iwindow.h"

#define LCRT_IRENAME_TABLE "irename"

enum {
    LCRT_R_TITLE,
    LCRT_R_TIP,
    LCRT_R_CANCEL,
    LCRT_R_OK,
    LCRT_R_ERROR,
    LCRT_R_ERR_MSG,
    LCRT_R_NUMBER
};
#define LCRT_R_NAME \
    "r_title", \
    "r_tip", \
    "r_cancel", \
    "r_ok", \
    "r_error", \
    "r_err_msg"

#define LCRT_R_VALUE \
    "Rename", \
    "Enter new name:", \
    "Cancel", \
    "OK", \
    "There has a same one, try an another!", \
    "There is no memory to create dialog"

struct lcrtc_rename {
    struct lcrt_config db;
    const char *name[LCRT_R_NUMBER];
    char value[LCRT_R_NUMBER][LCRT_CONFIG_VALUE_LEN];
};
struct lcrt_rename {
    struct lcrt_window *parent;
    gboolean rename_terminal;
    char rename[256];
    GtkWidget *dialog;
    GtkWidget *entry_rename;
    GtkWidget *okbutton;
    GtkTreeIter *iter;

    struct lcrtc_rename config;
    const char *(*get_db)(struct lcrt_rename *lrename);
    const char *(*get_tb)(struct lcrt_rename *lrename);
};
struct lcrt_rename *lcrt_create_dialog_rename(struct lcrt_window *parent, GtkWindow *window, const char *name, gboolean popup);
void lcrt_destroy_rename(struct lcrt_rename *lrename);
#endif
