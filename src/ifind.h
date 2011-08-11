#ifndef __LCRT_IFIND_H__
#define __LCRT_IFIND_H__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "iwindow.h"
#include "iconnect.h"

#define LCRT_IFIND_TABLE "ifind"

enum {
    LCRT_I_TITLE,
    LCRT_I_TIP,
    LCRT_I_CANCEL,
    LCRT_I_OK,
    LCRT_I_INFO,
    LCRT_I_ERR_MSG,
    LCRT_I_NUMBER
};
#define LCRT_I_NAME \
    "f_title", \
    "f_tip", \
    "f_cancel", \
    "f_ok", \
    "f_info", \
    "f_erf_msg"

#define LCRT_I_VALUE \
    "Find", \
    "Enter the name of session you want find:", \
    "Cancel", \
    "OK", \
    "There is nothing find!", \
    "There is no memory to create dialog"

struct lcrtc_find {
    struct lcrt_config db;
    const char *name[LCRT_I_NUMBER];
    char value[LCRT_I_NUMBER][LCRT_CONFIG_VALUE_LEN];
};
struct lcrt_find {
    struct lcrt_connect *parent;
    GtkWidget *dialog;
    GtkWidget *entry_find;
    GtkWidget *okbutton;
    GtkTreeIter *iter;

    struct lcrtc_find config;
    const char *(*get_db)(struct lcrt_find *lfind);
    const char *(*get_tb)(struct lcrt_find *lfind);
};
struct lcrt_find *lcrt_create_dialog_find(struct lcrt_connect *parent);
void lcrt_destroy_find(struct lcrt_find *lfind);
#endif
