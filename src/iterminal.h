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

#ifndef __LCRT_ITERMINAL_H__
#define __LCRT_ITERMINAL_H__
#include <unistd.h>
#include <gtk/gtk.h>
#include <vte/vte.h>
#include "inotebook.h"
#include "list.h"
#include "mkconfig.h"
#include "user.h"
#define lcrt_terminal_get_config(lterminal, config_id) lterminal->parent->config.value[config_id]

#define LCRT_TERMINAL_EXIT_CMD "exit\n"
#define LCRT_TERMINAL_YES_CMD "yes\n"
#define LCRT_TERMINAL_SEND_CMD "\n"
#define LCRT_TERMINAL_TEST_CMD "LCRT_TERMINAL_TEST_CMD"
#define LCRT_TERMINAL_ECHO_CMD "echo "LCRT_TERMINAL_TEST_CMD"\n"
#define LCRT_TERMINAL_TIMEOUT 5000
#define LCRT_PASSWORD_LEN 32
typedef enum {
    LCRT_TERMINAL_DISCONNECT = 0x0,
    LCRT_TERMINAL_CONNECTED = 0x1,
    LCRT_TERMINAL_CONNECTING = 0x2,
    LCRT_TERMINAL_WAIT_RETURN = 0x4,
    LCRT_TERMINAL_CHILD_EXIT = 0x8,
    LCRT_TERMINAL_SEND_USERNAME = 0x10,
    LCRT_TERMINAL_SEND_PASSWORD = 0x20,
}lcrt_terminal_status_t;

struct lcrt_terminal {
    struct lcrt_notebook *parent;
    pid_t child_pid;
    gboolean signal_connected;
    struct lcrtc_user *user;
    char lock_password[LCRT_PASSWORD_LEN + 1];

    GtkWidget *scrolledwindow;
    GtkWidget *label;
    VteTerminal *terminal;
    struct list_head brother;
    lcrt_terminal_status_t connected;
    gboolean save_passwd;
    gboolean need_reset;
    gboolean locked;
    void *private_data; /**< the pointer to point to private data of each protocol */
    const struct lcrt_protocol_callback *ops; /**< callbacks for each protocl  */
};

int lcrt_create_terminal(struct lcrt_notebook *parent);
void lcrt_destroy_terminal(struct lcrt_terminal *lterminal);
int lcrt_terminal_set_status(struct lcrt_terminal *lterminal, char *label_name, lcrt_terminal_status_t status);
#endif
