/*
 * Copyright (c) 2011 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Sun 07 Aug 2011 05:54:28 PM CST
 * File Name   : rlogin.c
 *
 * Description:  
 */

#include <stdio.h>
#include "iterminal.h"
#include "iqconnect.h"
#include "debug.h"
#include "user.h"

void lcrt_qconnect_create_shell_subbox(struct lcrt_qconnect *lqconnect)
{
}

void *lcrt_terminal_shell_contents_changed(struct lcrt_terminal *lterminal)
{
    lcrt_terminal_set_connected_status(lterminal);
    debug_print("reset = %f\n",
        gtk_adjustment_get_upper(
           gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(lterminal->scrolledwindow))));
}
struct lcrtc_user *lcrt_qconnect_shell_on_button_connect_clicked(struct lcrt_qconnect *lqconnect)
{
}
