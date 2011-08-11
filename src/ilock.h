#ifndef __LCRT_ILOCK_H__
#define __LCRT_ILOCK_H__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "imenubar.h"

#define LCRT_ILOCK_TABLE "ilock"

enum {
    LCRT_K_LOCK_TITLE,
    LCRT_K_UNLOCK_TITLE,
    LCRT_K_LOCK_TIP,
    LCRT_K_UNLOCK_TIP,
    LCRT_K_PASSWORD,
    LCRT_K_REPASSWORD,
    LCRT_K_CANCEL,
    LCRT_K_OK,
    LCRT_K_ERROR,
    LCRT_K_ERR_MSG,
    LCRT_K_NUMBER
};
#define LCRT_K_NAME \
    "k_lock_title", \
    "k_unlock_title", \
    "k_lock_tip", \
    "k_unlock_tip", \
    "k_password", \
    "k_repassword", \
    "k_cancel", \
    "k_ok", \
    "k_error", \
    "k_err_msg"

#define LCRT_K_VALUE \
    "Lock Session", \
    "Unlock Session", \
    "Enter password for session and will be used \nwhen unlock the session.", \
    "Enter password to unlock the session.", \
    "Password: ", \
    "Retype password:", \
    "Cancel", \
    "OK", \
    "Password not match!", \
    "There is no memory to create dialog"

struct lcrtc_lock {
    struct lcrt_config db;
    const char *name[LCRT_K_NUMBER];
    char value[LCRT_K_NUMBER][LCRT_CONFIG_VALUE_LEN];
};
struct lcrt_lock {
    struct lcrt_terminal *parent;

    GtkWidget *dialog;
    GtkWidget *entry_password;
    GtkWidget *entry_repassword;

    struct lcrtc_lock config;
    const char *(*get_db)(struct lcrt_lock *llock);
    const char *(*get_tb)(struct lcrt_lock *llock);
};
struct lcrt_lock *lcrt_create_dialog_lock(struct lcrt_terminal *parent);
void lcrt_destroy_lock(struct lcrt_lock *llock);
#endif
