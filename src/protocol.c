/**
 * @file protocol.c
 * <h2>License</h2>
 * Copyright (c) 2011-2012 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * @author Niu Tao <niutao0602@gmail.com>
 * @version $Id
 * @date Wed 10 Aug 2011 01:30:13 PM CST
 *
 * @brief Here, list all the protocols we support. When you want to add a new
 *        protocol, you should give a structure name with lcrt_protocol_XX_callbacks,
 *        this structure contain some callback,like receive,connect,show and etc, you
 *        should complete this functions.
 */

#include <stdio.h>
#include "protocol.h"
#include "ssh.h"
#include "telnet.h"
#include "rlogin.h"
#include "serial.h"
#include "shell.h"

#define LCRT_INSTALL_PROTOCOL(support_protocol) &lcrt_protocol_##support_protocol##_callbacks

static struct lcrt_protocol_callback lcrt_protocol_null_callbacks = {
    .protocol   = LCRT_PROTOCOL_UNKNOWN,
    .receive    = NULL,
    .connect    = NULL,
    .disconnect = NULL,
    .show       = NULL,
    .create     = NULL,
    .changed    = NULL,

};

/** 
 * list the protocol we support 
 */
static struct lcrt_protocol_callback *lcrt_protocol_callbacks[LCRT_PROTOCOL_UNKNOWN] = {
    LCRT_INSTALL_PROTOCOL(ssh2),
    LCRT_INSTALL_PROTOCOL(ssh1),
    LCRT_INSTALL_PROTOCOL(telnet),
    LCRT_INSTALL_PROTOCOL(telnet_ssl),
    LCRT_INSTALL_PROTOCOL(rlogin),
    LCRT_INSTALL_PROTOCOL(serial),
    LCRT_INSTALL_PROTOCOL(shell),
    /** Add new protocol after this */
    [LCRT_PROTOCOL_UNKNOWN - 1] = LCRT_INSTALL_PROTOCOL(null)
};
/**
 * @brief  Get callbacks from protocol
 * @param  protocol protocol list in lcrt_protocol_t
 * @return If found, return the callback matched, 
 *         otherwise,return unknown protocol callback.
 */
struct lcrt_protocol_callback *lcrt_protocol_get_callback(lcrt_protocol_t protocol)
{
    int i;
    
    for (i = 0; i < LCRT_PROTOCOL_UNKNOWN; i++) {
        if (lcrt_protocol_callbacks[i]->protocol == protocol)
            return lcrt_protocol_callbacks[i];
    }

    return lcrt_protocol_callbacks[LCRT_PROTOCOL_UNKNOWN - 1];
}

