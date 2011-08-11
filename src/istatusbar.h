/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Mon 17 Jan 2011 11:32:42 PM CST
 * File Name:    istatusbar.h
 *
 * Description:  
 */

#ifndef __LCRT_ISTATUSBAR_H__
#define __LCRT_ISTATUSBAR_H__

#include "iwindow.h"

struct lcrtc_statusbar {
    struct lcrt_config db;
};
struct lcrt_statusbar {
    struct lcrt_window *parent;
    GtkWidget *statusbar;
    struct lcrtc_statusbar config;
};
struct lcrt_statusbar *lcrt_create_statusbar(struct lcrt_window *parent);
void lcrt_destroy_statusbar(struct lcrt_statusbar *lstatusbar);

#endif
