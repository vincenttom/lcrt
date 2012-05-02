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
    LCRT_I_TIP_CONNECT,
    LCRT_I_TIP_EDIT,
    LCRT_I_CANCEL,
    LCRT_I_PREV,
    LCRT_I_NEXT,
    LCRT_I_INFO,
    LCRT_I_ERR_MSG,
    LCRT_I_NUMBER
};
#define LCRT_I_NAME \
    "i_title", \
    "i_tip_connect", \
    "i_tip_edit", \
    "i_cancel", \
    "i_prev", \
    "i_next", \
    "i_info", \
    "i_err_msg"

#define LCRT_I_VALUE \
    "Find", \
    "Enter the name of session you want find:", \
    "Enter the string you want find:", \
    "Cancel", \
    "Previous", \
    "Next", \
    "There is nothing find!", \
    "There is no memory to create dialog"

struct lcrtc_find {
    struct lcrt_config db;
    const char *name[LCRT_I_NUMBER];
    char value[LCRT_I_NUMBER][LCRT_CONFIG_VALUE_LEN];
};
struct lcrt_find {
    void *parent;
    int flag;
#define LCRT_FIND_FCONNECT 0x1
#define LCRT_FIND_FEDIT    0x2
    GtkWidget *dialog;
    GtkWidget *entry_find;
    GtkWidget *prev_button;
    GtkWidget *next_button;
    GtkTreeIter *iter;
    GtkWidget *parent_window;

    struct lcrtc_find config;
    const char *(*get_db)(struct lcrt_find *lfind);
    const char *(*get_tb)(struct lcrt_find *lfind);
};
struct lcrt_find *lcrt_create_dialog_find(void *parent, int flag);
void lcrt_destroy_find(struct lcrt_find *lfind);
#endif
