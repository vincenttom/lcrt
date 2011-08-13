/*
 * Copyright (c) 2011 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : Niu Tao <niutao0602@gmail.com>
 * Created Time: Wed 10 Aug 2011 01:30:13 PM CST
 * File Name   : protocol.c
 *
 * Description:  
 */

#include <stdio.h>
#include "protocol.h"
#include "ssh.h"
#include "telnet.h"
#include "rlogin.h"
#include "serial.h"
#include "shell.h"

#define LCRT_INSTALL_PROTOCOL(support_protocol) &lcrt_protocol_##support_protocol##_callbacks,

struct lcrt_protocol_callback lcrt_protocol_null_callbacks = {
    .protocol   = LCRT_PROTOCOL_UNKNOWN,
    .receive    = NULL,
    .connect    = NULL,
    .disconnect = NULL,
    .show       = NULL,
    .create     = NULL,
    .changed    = NULL,

};

/* list the protocol we support */
static struct lcrt_protocol_callback *lcrt_protocol_callbacks[LCRT_PROTOCOL_UNKNOWN] = {
    LCRT_INSTALL_PROTOCOL(ssh2)
    LCRT_INSTALL_PROTOCOL(ssh1)
    LCRT_INSTALL_PROTOCOL(telnet)
    LCRT_INSTALL_PROTOCOL(telnet_ssl)
    LCRT_INSTALL_PROTOCOL(rlogin)
    LCRT_INSTALL_PROTOCOL(serial)
    LCRT_INSTALL_PROTOCOL(shell)
    LCRT_INSTALL_PROTOCOL(null)
};

struct lcrt_protocol_callback *lcrt_protocol_get_callback(lcrt_protocol_t protocol)
{
    int i;
    
    for (i = 0; i <= LCRT_PROTOCOL_UNKNOWN; i++) {
        if (lcrt_protocol_callbacks[i]->protocol == protocol)
            return lcrt_protocol_callbacks[i];
    }

    return lcrt_protocol_callbacks[LCRT_PROTOCOL_UNKNOWN];
}

