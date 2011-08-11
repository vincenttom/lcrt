/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sat 15 Jan 2011 04:24:46 PM CST
 * File Name:    iterminal.h
 *
 * Description:  
 */
#ifndef __LCRT_IACCELS_H__
#define __LCRT_IACCELS_H__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "imenubar.h"

#define LCRT_IACCELS_TABLE "iaccels"
#define LCRT_ACCELS_CONFIG_DIR "etc/settings"
#define LCRT_ACCELS_DB "accels.db"

enum {
    LCRT_KB_ACTION,
    LCRT_KB_SHORTCUT_KEY,
    LCRT_KB_FILE,
    LCRT_KB_F_CONNECT,
    LCRT_KB_F_QUICK_CONNECT,
    LCRT_KB_F_CONNECT_IN_TAB,
    LCRT_KB_F_RECONNECT,
    LCRT_KB_EDIT,
    LCRT_KB_E_COPY,
    LCRT_KB_E_PASTE,
    LCRT_KB_SESSION,
    LCRT_KB_S_CLONE,
    LCRT_KB_S_CLOSE,
    LCRT_KB_S_MOVE_NEXT,
    LCRT_KB_S_MOVE_PREV,
    LCRT_KB_S_SWITCH_NEXT,
    LCRT_KB_S_SWITCH_PREV,
    LCRT_KB_WARN_MSG,
    LCRT_KB_ERR_MSG,
    LCRT_KB_NUMBER
};
enum
{
  ACTION_COLUMN,
  KEYVAL_COLUMN,
  N_COLUMNS
};

#define LCRT_KB_NAME \
    "kb_action", \
    "kb_shortcut_key", \
    "kb_file", \
    "kb_f_connect", \
    "kb_f_quick_connect", \
    "kb_f_connect_in_tab", \
    "kb_f_reconnect", \
    "kb_edit", \
    "kb_e_copy", \
    "kb_e_paste", \
    "kb_session", \
    "kb_s_clone", \
    "kb_s_close", \
    "kb_s_move_next", \
    "kb_s_move_prev", \
    "kb_s_switch_next", \
    "kb_s_switch_prev", \
    "kb_warn_msg", \
    "kb_err_msg"

#define LCRT_KB_VALUE \
    "Action", \
    "Shortcut key", \
    "File", \
    "Connect", \
    "Quick Connect", \
    "Connect In Tab", \
    "Reconnect", \
    "Edit", \
    "Copy", \
    "Paste", \
    "Session", \
    "Clone", \
    "Close", \
    "Move To Next" ,\
    "Move To Prev", \
    "Switch To Next", \
    "Switch To Prev", \
    "The shortcut key \"%s\" is already bound to the \"%s\" action",\
    "There is no memory to create dialog"

#define LCRT_KB_KEY \
    {0, 0}, \
    {0, 0}, \
    {0, 0}, \
    {GDK_c,  GDK_MOD1_MASK}, \
    {GDK_q,  GDK_MOD1_MASK}, \
    {GDK_b,  GDK_MOD1_MASK}, \
    {GDK_F5,  0}, \
    {0, 0}, \
    {GDK_c, GDK_CONTROL_MASK | GDK_SHIFT_MASK}, \
    {GDK_v, GDK_CONTROL_MASK | GDK_SHIFT_MASK}, \
    {0, 0}, \
    {GDK_t, GDK_CONTROL_MASK | GDK_SHIFT_MASK}, \
    {GDK_w, GDK_CONTROL_MASK | GDK_SHIFT_MASK}, \
    {GDK_Page_Down, GDK_CONTROL_MASK | GDK_SHIFT_MASK}, \
    {GDK_Page_Up, GDK_CONTROL_MASK | GDK_SHIFT_MASK}, \
    {GDK_Page_Down, GDK_CONTROL_MASK}, \
    {GDK_Page_Up, GDK_CONTROL_MASK}

struct lcrtc_accels {
    struct lcrt_config db;
    struct lcrt_config ldb;
    const char *name[LCRT_KB_NUMBER];
    char value[LCRT_KB_NUMBER][LCRT_CONFIG_VALUE_LEN];
    int key[LCRT_KB_NUMBER][2];
};
struct lcrt_accels {
    struct lcrt_window *parent;
    GtkWidget *tree_view;

    gboolean dirty;

    struct lcrtc_accels config;
    const char *(*get_db)(struct lcrt_accels *laccels);
    const char *(*get_tb)(struct lcrt_accels *laccels);
    const char *(*get_ldb)(struct lcrt_accels *laccels);
    const char *(*get_ltb)(struct lcrt_accels *laccels);
};
struct lcrt_accels *lcrt_create_accels(struct lcrt_window *parent);
void lcrt_destroy_accels(struct lcrt_accels *laccels);
int lcrt_accels_init_local_config(struct lcrt_accels *laccels);
int lcrt_accels_save_local_config(struct lcrt_accels *laccels);

#endif 
