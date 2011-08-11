/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sat 22 Jan 2011 04:18:02 PM CST
 *
 *
 * Description:  
 */

#ifndef __LCRT_IQCONNECT_H__
#define __LCRT_IQCONNECT_H__

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "imenubar.h"

#define LCRT_IQCONNECT_TABLE "iqconnect"

enum {
    LCRT_Q_CONNECT,
    LCRT_Q_SESSION_OPTION,
    LCRT_Q_LB_PROTOCOL,
    LCRT_Q_LB_HOSTNAME,
    LCRT_Q_LB_USERNAME,
    LCRT_Q_LB_FIREWALL,
    LCRT_Q_LB_PORT,
    LCRT_Q_LB_FRAME,
    LCRT_Q_CB_PASSWD,
    LCRT_Q_CB_PUBLICKEY,
    LCRT_Q_CB_KEYBOARD,
    LCRT_Q_CB_GSSAPI,
    LCRT_Q_CB_RSA,
    LCRT_Q_CB_TIS,
    LCRT_Q_BT_PROPERTIES,
    LCRT_Q_LB_DEFAULT_COMMAND,
    LCRT_Q_CB_SHOW_QCONNECT,
    LCRT_Q_CB_SAVE_SESSION,
    LCRT_Q_CB_OPEN_IN_A_TAB,
    LCRT_Q_BT_CONNECT,
    LCRT_Q_BT_OK,
    LCRT_Q_BT_CANCEL,
    LCRT_Q_ERR_MSG,
    LCRT_Q_SPORT,
    LCRT_Q_SBAUD_RATE,
    LCRT_Q_SDATA_BITS,
    LCRT_Q_SPARITY,
    LCRT_Q_SSTOP_BITS,
    LCRT_Q_SFLOW_CONTROL,
    LCRT_Q_NUMBER
};
#define LCRT_Q_NAME \
    "q_connect", \
    "q_session_option", \
    "q_lb_protocol", \
    "q_lb_hostname", \
    "q_lb_username", \
    "q_lb_firewall", \
    "q_lb_port", \
    "q_lb_frame", \
    "q_cb_passwd", \
    "q_cb_publickey", \
    "q_cb_keyboard", \
    "q_cb_gssapi", \
    "q_cb_rsa", \
    "q_cb_tis", \
    "q_bt_properties", \
    "q_lb_default_command", \
    "q_cb_show_qconnect", \
    "q_cb_save_session", \
    "q_cb_open_in_a_tab", \
    "q_bt_connect", \
    "q_bt_ok", \
    "q_bt_cancel", \
    "q_err_msg", \
    "q_sport", \
    "q_sbaud_rate", \
    "q_sdata_bits", \
    "q_sparity", \
    "q_sstop_bits", \
    "q_sflow_control", \

#define LCRT_Q_VALUE \
    "Quick connect", \
    "Session Option", \
    "  Protocol:", \
    "  Hostname:", \
    "  Username:", \
    "  Firewall:", \
    "  Port: ", \
    "Authentication", \
    "Password", \
    "PublicKey", \
    "KeyBoard Interactive", \
    "GSSAPI", \
    "RSA", \
    "TIS", \
    "Properties...", \
    "  Command:", \
    "Show Quick connect on startup", \
    "Save Session", \
    "Open in a tab", \
    "Connect", \
    "OK", \
    "Cancel", \
    "There is no memory to create window" , \
    "  Part:", \
    "  Baud rate:", \
    "  Data bits:", \
    "  Parity:", \
    "  Stop bits:", \
    "Flow control"

#define LCRT_Q_SHORTCUT \
    {0, 0}

struct lcrtc_qconnect {
    struct lcrt_config db;
    const char *name[LCRT_Q_NUMBER];
    char value[LCRT_Q_NUMBER][LCRT_CONFIG_VALUE_LEN];
    int   shortcut[LCRT_Q_NUMBER][2];
};

typedef enum {
    LCRT_QCONNECT_IN_TAB = 1,
    LCRT_QCONNECT_IN_WINDOW,
    LCRT_QCONNECT_SESSION_OPTION,
}lcrt_qconnect_flag_t;

struct lcrt_qconnect {
    struct lcrt_window *parent;
    lcrt_qconnect_flag_t flag;
    const char *uname;

    GtkWidget *q_connect;
    GtkWidget *q_vbox_spec;
    GtkWidget *q_cb_protocol;

    lcrt_protocol_t nproto;

    union {
        struct {
            GtkWidget *hostname;
            GtkWidget *username;
            GtkWidget *firewall;
            GtkWidget *port;
            GtkWidget *passwd;
            GtkWidget *publickey;
            GtkWidget *keyboard;
            GtkWidget *gssapi;
            GtkWidget *rsa;
            GtkWidget *tis;
            GtkListStore *authentication;
            boolean auth[4];
        } ssh;
#define pssh q_proto.ssh
        struct {
            GtkWidget *hostname;
            GtkWidget *username;
            GtkWidget *firewall;
            GtkWidget *port;
        } telnet;
#define ptelnet q_proto.telnet
        struct {
            GtkWidget *hostname;
            GtkWidget *username;
        } rlogin;
#define prlogin q_proto.rlogin    
        struct {
            GtkWidget *port;
            GtkWidget *baud_rate;
            GtkWidget *data_bits;
            GtkWidget *parity;
            GtkWidget *stop_bits;
            GtkWidget *dir_dsr;
            GtkWidget *rts_cts;
            GtkWidget *xon_xoff;
        } serial;
#define pserial q_proto.serial
    
        struct {
            GtkWidget *hostname;
            GtkWidget *username;
            GtkWidget *firewall;
            GtkWidget *port;
        } raw;
#define praw q_proto.raw        
    } q_proto;

    GtkWidget *q_bt_properties;
    GtkWidget *q_et_default_command;
    GtkWidget *q_cb_show_qconnect;
    GtkWidget *q_cb_save_session;
    GtkWidget *q_cb_open_in_a_tab;
    GtkWidget *q_bt_connect;
    GtkWidget *q_bt_cancel;

    struct lcrtc_qconnect config;
    const char *(*get_db)(struct lcrt_qconnect *lqconnect);
    const char *(*get_tb)(struct lcrt_qconnect *lqconnect);
    struct lcrtc_user *(*on_button_connect_clicked)(struct lcrt_qconnect *lqconnect);
    void (*create_subbox)(struct lcrt_qconnect *lqconnect);
};

int lcrt_create_qconnect(
    struct lcrt_window *parent, 
    GtkWindow *window, 
    lcrt_qconnect_flag_t flag, 
    const char *uname);
void lcrt_destroy_qconnect(struct lcrt_qconnect *lqconnect);
#endif
