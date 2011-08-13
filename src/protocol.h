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

struct lcrt_qconnect;
struct lcrt_terminal;

/*
 * This structure is used to support more protocol
 */
struct lcrt_protocol_callback {
    /** the protocol mark */
    lcrt_protocol_t protocol;
    /** 
     * @brief When a terminal receive data from remote, it will call 
     *        this functions to handle.
     * NOTE: The order of this functions must be same wtih lcrt_protocol_t
     * @param lterminal the terminal which receive data from remote.
     */
    void (*receive)(struct lcrt_terminal *lterminal);

    /** @brief When we create a terminal, we should fork a child to connect 
     *         with remote 
     *  @param lterminal the terminal which will be connected with remote.
     *  @return return val.
     *  - < 0 an error code
     *  - = 0 success
     */
    int (*connect)(struct lcrt_terminal *lterminal);

    /** 
     * @brief disconnect with remote, each protocol use connect method 
     *        to connect with remote, use disconnect to close from 
     *        remote.
     * @param lterminal the terminal descriptor which will be disconnected.
     */
    void (*disconnect)(struct lcrt_terminal *lterminal);
    /**
     * @brief When we open the Quick connect dialog, we should show different
     *        options for each protocol, so we use these callbacks to create 
     *        sub box in quick connect dialog.
     * @param lqconnet the quick connect dialog descriptor
     */
    void (*show)(struct lcrt_qconnect *lqconnect);

    /**
     * @brief When you click the connect button in quick connect dialog, we
     *        should create a terminal to connect with remote,each protocol
     *        has different method.
     * @param lqconnect contain all data which is used to create s session.
     * @return a user information data structure
     */
    struct lcrtc_user *(*create)(struct lcrt_qconnect *lqconnect);
    /**
     * @brief For some protocol,like ssh, when username is changed,we should kill 
     *        the previous connection and fork connect again, we use this callback
     *        to mark whether the username is changed.
     * @param lterminal the terminal which usernam changed.
     * @param change whether the username is changed.
     */
    void (*changed)(struct lcrt_terminal *lterminal, int change);
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
