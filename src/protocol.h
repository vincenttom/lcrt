/*
 * Copyright (c) 2011 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : Niu Tao <niutao0602@gmail.com>
 * Created Time: Wed 10 Aug 2011 01:28:31 PM CST
 * File Name   : protocol.h
 *
 * Description:  
 */

#ifndef __LCRT_PROTOCOL_H__
#define __LCRT_PROTOCOL_H__

typedef enum {
    LCRT_PROTOCOL_SSH2,
    LCRT_PROTOCOL_SSH1,
    LCRT_PROTOCOL_TELNET,
    LCRT_PROTOCOL_TELNET_SSL,
    LCRT_PROTOCOL_RLOGIN,
    LCRT_PROTOCOL_SERIAL,
    LCRT_PROTOCOL_SHELL,
    LCRT_PROTOCOL_NUMBER,
    LCRT_PROTOCOL_UNKNOWN
}lcrt_protocol_t;

/** default port of protocol */
#define LCRT_PROTOCOL_PORT \
    "22", \
    "22", \
    "23", /**<can't change*/ \
    "992", /**<can't change*/ \
    "0", \
    "0", \
    "0"

#define LCRT_PROTOCOL_NAME \
    "SSH2", \
    "SSH1", \
    "Telnet", \
    "Telnet/SSL", \
    "Rlogin", \
    "Serial", \
    "Shell"
#if 0
#define LCRT_PROTOCOL_CONTENTS_CHANGED_CALLBACKS \
    lcrt_terminal_ssh_contents_changed, \
    lcrt_terminal_ssh_contents_changed, \
    lcrt_terminal_telnet_contents_changed, \
    lcrt_terminal_telnet_contents_changed, \
    lcrt_terminal_rlogin_contents_changed, \
    lcrt_terminal_serial_contents_changed, \
    lcrt_terminal_shell_contents_changed
#define LCRT_PROTOCOL_CREATE_SUBBOX_CALLBACKS \
    lcrt_qconnect_create_ssh_subbox, \
    lcrt_qconnect_create_ssh_subbox, \
    lcrt_qconnect_create_telnet_subbox, \
    lcrt_qconnect_create_telnet_subbox, \
    lcrt_qconnect_create_rlogin_subbox, \
    lcrt_qconnect_create_serial_subbox, \
    lcrt_qconnect_create_shell_subbox
#define LCRT_PROTOCOL_ON_BUTTON_CONNECT_CLICKED_CALLBACKS \
    lcrt_qconnect_ssh_on_button_connect_clicked, \
    lcrt_qconnect_ssh_on_button_connect_clicked, \
    lcrt_qconnect_telnet_on_button_connect_clicked, \
    lcrt_qconnect_telnet_on_button_connect_clicked, \
    lcrt_qconnect_rlogin_on_button_connect_clicked, \
    lcrt_qconnect_serial_on_button_connect_clicked, \
    lcrt_qconnect_shell_on_button_connect_clicked
#endif
struct lcrt_qconnect;
struct lcrt_terminal;

/*
 * This structure is used to support more protocol
 */
struct lcrt_protocol_callback {
    lcrt_protocol_t protocol;
    /* 
     * When a terminal receive data from remote, it will call this functions
     * to handle.
     * NOTE: The order of this functions must be same wtih lcrt_protocol_t 
     */
    void (*contents_changed)(struct lcrt_terminal *lterminal);

    /* When we create a terminal, we should fork a child to connect with remote */
    int (*connect_remote)(struct lcrt_terminal *lterminal);

    /*
     * When we open the Quick connect dialog, we should show different
     * options for each protocol, so we use these callbacks to create 
     * sub box in quick connect dialog.
     */
    void (*create_subbox)(struct lcrt_qconnect *lqconnect);

    /*
     * When you click the connect button in quick connect dialog, we
     * should create a terminal to connect with remote,each protocol
     * has different method.
     */
    struct lcrtc_user *(*create_session)(struct lcrt_qconnect *lqconnect);
};

enum {
    LCRT_FIREWALL_NONE,
    LCRT_FIREWALL_NUMBER
};
#define LCRT_FIREWALL_NAME \
    "none"
enum {
    LCRT_DEP_SSH,
    LCRT_DEP_TELNET,
    LCRT_DEP_RLOGIN,
    LCRT_DEP_SHELL,
    LCRT_DEP_NUMBER
};
#define LCRT_DEP_PROG \
    "ssh", \
    "telnet", \
    "rlogin", \
    "bash"

extern struct lcrt_protocol_callback *lcrt_protocol_get_callback(lcrt_protocol_t protocol);

#endif
