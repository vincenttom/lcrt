#ifndef __LCRT_IMKDIR_H__
#define __LCRT_IMKDIR_H__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "iconnect.h"
#include "user.h"

#define LCRT_IMKDIR_TABLE "imkdir"

enum {
    LCRT_M_TITLE,
    LCRT_M_TIP,
    LCRT_M_CANCEL,
    LCRT_M_OK,
    LCRT_M_ERROR,
    LCRT_M_ERR_MSG,
    LCRT_M_NUMBER
};
#define LCRT_M_NAME \
    "m_title", \
    "m_tip", \
    "m_cancel", \
    "m_ok", \
    "m_error", \
    "m_err_msg"

#define LCRT_M_VALUE \
    "New folder", \
    "Enter new folder name:", \
    "Cancel", \
    "OK", \
    "There has a same one, try an another!", \
    "There is no memory to create dialog"

struct lcrtc_mkdir {
    struct lcrt_config db;
    const char *name[LCRT_M_NUMBER];
    char value[LCRT_M_NUMBER][LCRT_CONFIG_VALUE_LEN];
};
struct lcrt_mkdir {
    struct lcrt_connect *parent;
    char mkdir[DIRNAME_LEN + 1];
    GtkWidget *dialog;
    GtkWidget *entry_mkdir;
    GtkWidget *okbutton;
    GtkTreeIter *iter;

    struct lcrtc_mkdir config;
    const char *(*get_db)(struct lcrt_mkdir *lmkdir);
    const char *(*get_tb)(struct lcrt_mkdir *lmkdir);
};
struct lcrt_mkdir *lcrt_create_dialog_mkdir(struct lcrt_connect *parent, GtkWindow *window);
void lcrt_destroy_mkdir(struct lcrt_mkdir *lmkdir);
#endif
