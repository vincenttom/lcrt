/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Mon 17 Jan 2011 11:32:42 PM CST
 * File Name:    inotebook.h
 *
 * Description:  
 */

#ifndef __LCRT_INOTEBOOK_H__
#define __LCRT_INOTEBOOK_H__

#include "iwindow.h"
#include "list.h"
#define LCRT_ITERMINAL_TABLE "iterminal"
typedef enum {
    LCRT_NB_COPY,
    LCRT_NB_PASTE,
    LCRT_NB_NONE, /*useless*/
    LCRT_NB_CLONE,
    LCRT_NB_CLOSE,
    LCRT_NB_MOVE_NEXT,
    LCRT_NB_MOVE_PREV,
    LCRT_NB_SWITCH_NEXT,
    LCRT_NB_SWITCH_PREV,
    LCRT_NB_NUMBER
}lcrt_nb_t;

enum {
    LCRT_TM_CONNECTION_TIMEOUT,
    LCRT_TM_CONNECTION_REFUSED,
    LCRT_TM_CONNECTION_FAILED,
    LCRT_TM_CONNECTION_PROG_NOT_FOUND,
    LCRT_TM_CONNECTION_NOT_SUPPORT,
    LCRT_TM_NUMBER
};
#define LCRT_TM_NAME \
    "tm_connection_timed_out", \
    "tm_connection_refused", \
    "tm_connection_failed", \
    "tm_connection_prog_not_found", \
    "tm_connection_not_support"

#define LCRT_TM_VALUE \
    "Connection timed out!", \
    "Connection refused!", \
    "Username or password error,exit!", \
    "Can't find the programe \"%s\",please install it!", \
    "Current version not support!"

struct lcrtc_notebook {
    struct lcrt_config db;
    const char *name[LCRT_TM_NUMBER];
    char value[LCRT_TM_NUMBER][LCRT_CONFIG_VALUE_LEN];
    int shortcut[LCRT_NB_NUMBER][2];
};
struct lcrt_notebook {
    struct lcrt_window *parent;
    int current_page;

    GtkWidget *notebook;
    struct lcrt_terminal *current_terminal;
    struct list_head child;
    struct lcrtc_notebook config;
    const char *(*get_db)(struct lcrt_notebook *lnotebook);
    const char *(*get_tb)(struct lcrt_notebook *lnotebook);
};
struct lcrt_notebook *lcrt_create_notebook(struct lcrt_window *parent);
void lcrt_destroy_notebook(struct lcrt_notebook *lnotebook);

#endif
