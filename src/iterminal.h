/**
 * @file iterminal.h
 * <h2>License</h2>
 * Copyright (c) 2011 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * @author Niu Tao <niutao0602@gmail.com>
 * @version $Id
 * @date Sat 15 Jan 2011 04:24:46 PM CST
 *
 * @brief define same structure for our terminal component
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
/**
 * The status of session
 */
typedef enum {
    LCRT_TERMINAL_DISCONNECT = 0x0, /**< disconnected from remote  */
    LCRT_TERMINAL_CONNECTED = 0x1, /**< connected with remote */
    LCRT_TERMINAL_CONNECTING = 0x2, /**< connecting with remote */
    LCRT_TERMINAL_WAIT_RETURN = 0x4, /**< send login information and wait responses */
    LCRT_TERMINAL_CHILD_EXIT = 0x8, /**< the child process of terminal is exit, it occur when disconnect from remote */
    LCRT_TERMINAL_SEND_USERNAME = 0x10, /**< had send the username */
    LCRT_TERMINAL_SEND_PASSWORD = 0x20, /*< had send the password */
}lcrt_terminal_status_t;

/**
 * terminal structure
 */
struct lcrt_terminal {
    struct lcrt_notebook *parent; /**< parent component */
    pid_t child_pid; /**< child process id */
    gboolean signal_connected; /**< the flag to mark whether the key press 
                                 singal 'Return' is connected with current terminal */
    struct lcrtc_user *user; /**< user information of current terminal*/

    char lock_password[LCRT_PASSWORD_LEN + 1]; /**< the buffer to store the password of 'lock session'*/
    gboolean locked; /**< the flag to mark whether we are locked */

    GtkWidget *scrolledwindow; /**< the pointer of scroll bar */
    GtkWidget *label; /**< the pointer of label which is used to show session name */
    VteTerminal *terminal; /**< terminal component*/
    struct list_head brother; /**< list the brother terminal */
    lcrt_terminal_status_t connected; /**< the status of current session */
    gboolean save_passwd; /**< the flag to mark whether user choose save password in login dialog */

    void *private_data; /**< the pointer to point to private data of each protocol */
    const struct lcrt_protocol_callback *ops; /**< callbacks for each protocl  */
};

int lcrt_create_terminal(struct lcrt_notebook *parent);
void lcrt_destroy_terminal(struct lcrt_terminal *lterminal);
int lcrt_terminal_set_status(struct lcrt_terminal *lterminal, char *label_name, lcrt_terminal_status_t status);
#endif
