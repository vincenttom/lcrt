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
//#define __LCRT_DEBUG__
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include "iterminal.h"
#include "iqconnect.h"
#include "debug.h"
#include "user.h"
#include "shell.h"

struct lcrt_shell_if {
    GtkWidget *shell;
};

static void lcrt_shell_receive(struct lcrt_terminal *lterminal)
{
    lcrt_terminal_set_connected_status(lterminal);
    debug_print("reset = %f\n",
        gtk_adjustment_get_upper(
           gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(lterminal->scrolledwindow))));
}

static int lcrt_shell_connect(struct lcrt_terminal *lterminal)
{
    lcrt_protocol_t protocol;
    struct lcrtc_user *user;
    char *argv[5], *work_dir;
    char hostname[256], port[32];
    char *dep_prog[] = {LCRT_DEP_PROG};
    int dep = -1;
    char tmp[32];

    if (lterminal == NULL)
       return EINVAL;

    user = lterminal->user;
    argv[0] = user->password;
    debug_print("shell = %s\n", argv[0]);
    if (access(argv[0], F_OK | X_OK) != 0)
        argv[0] = "sh";

    argv[1] = NULL;
    work_dir = getenv("HOME");
    if (lcrt_exec_check(protocol) != 0) {
        lcrt_message_info(lterminal->parent->parent->window, 
                          lterminal->parent->config.value[LCRT_TM_CONNECTION_PROG_NOT_FOUND],
                          argv[0]);
        return -LCRTE_NOT_FOUND;
    }
#if VTE_CHECK_VERSION(0, 26, 0)
    vte_terminal_fork_command_full(
                            VTE_TERMINAL(lterminal->terminal), 
                            VTE_PTY_DEFAULT,
                            work_dir,
                            argv, 
                            NULL, 
                            0, 
                            NULL, 
                            NULL,
                            &lterminal->child_pid, 
                            NULL);
#else
    lterminal->child_pid  = vte_terminal_fork_command(VTE_TERMINAL(lterminal->terminal), 
                argv[0], argv, NULL , work_dir, FALSE, FALSE, FALSE);
#endif
    debug_print("child_pid = %d\n", lterminal->child_pid);
    lcrt_statusbar_set_user(lterminal->parent->parent->w_statusbar, lterminal->user);
    debug_where();

}

static void lcrt_shell_disconnect(struct lcrt_terminal *lterminal)
{
}

static void lcrt_shell_show(struct lcrt_qconnect *lqconnect)
{
    GtkWidget *vbox;
    GtkWidget *vbox_spec;
    GtkWidget *hbox2;
    GtkWidget *label_shell;
    GtkWidget *combobox_shell;
    GtkWidget *hbox3;
    struct lcrt_window *parent;
	int i;
    const char *shells[LCRT_SHELL_SUPPORT_NUMBER] = {LCRT_SHELL_SUPPORT};
    static struct lcrt_shell_if slshell, *lshell = &slshell;

    memset(lshell, 0, sizeof(struct lcrt_shell_if));
    lqconnect->private_data = lshell;


    parent = lqconnect->parent;

    vbox = GTK_DIALOG (lqconnect->q_connect)->vbox;

    vbox_spec = gtk_vbox_new (FALSE, 0);
    lqconnect->q_vbox_spec = vbox_spec;
    gtk_widget_show (vbox_spec);
    gtk_box_pack_start (GTK_BOX (vbox), vbox_spec, TRUE, TRUE, 0);
    gtk_box_reorder_child (GTK_BOX (vbox), vbox_spec, 1);
    gtk_widget_set_size_request (vbox_spec, -1, 210);

    hbox2 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox2);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox2, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox2, -1, 25);

    label_shell = gtk_label_new (lqconnect->config.value[LCRT_Q_SHELL]);
    gtk_widget_show (label_shell);
    gtk_box_pack_start (GTK_BOX (hbox2), label_shell, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_shell, 90, 25);
    gtk_misc_set_alignment (GTK_MISC (label_shell), 0, 0.5);

    combobox_shell = gtk_combo_box_entry_new_text ();
    lshell->shell = combobox_shell;
    gtk_widget_show (combobox_shell);
    gtk_box_pack_start (GTK_BOX (hbox2), combobox_shell, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_shell, 220 , 25);
    
    for (i = 0; i < LCRT_SHELL_SUPPORT_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_shell), shells[i]);
    }
    debug_where();
    //gtk_entry_set_editable(GTK_ENTRY(GTK_BIN(combobox_shell)->child), FALSE);
    gtk_entry_set_max_length(GTK_ENTRY(GTK_BIN(combobox_shell)->child), PASSWORD_LEN);


    hbox3 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox3);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox3, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox3, -1, 185);

    if (lqconnect->flag == LCRT_QCONNECT_SESSION_OPTION) {
        struct lcrtc_user *user;
        if ((user = lcrt_user_find_by_name(&parent->u_config, lqconnect->uname)) != NULL) {
			/* NOTE: there is very ugly, we use passwd field to store shell index */
            gtk_combo_box_set_active(GTK_COMBO_BOX(lshell->shell), atoi(user->password));
        }
    } else {
    	gtk_widget_set_sensitive(lqconnect->q_bt_connect, TRUE);
        gtk_combo_box_set_active(GTK_COMBO_BOX(lshell->shell), 0); //default shell is 'bash'
    }
}

static struct lcrtc_user *lcrt_shell_create(struct lcrt_qconnect *lqconnect)
{
    struct lcrt_window *lwindow = lqconnect->parent;
    struct lcrt_shell_if *lshell = (struct lcrt_shell_if *)lqconnect->private_data;
    const char *shell = gtk_combo_box_get_active_text(GTK_COMBO_BOX(lshell->shell));
    struct lcrtc_user *user;
    lcrt_protocol_t protocol = lqconnect->nproto;
    char hostname[HOSTNAME_LEN];
    char name[USERNAME_LEN];
    char password[PASSWORD_LEN];

    if (access(shell, F_OK | X_OK) != 0) {
        lcrt_message_error(lqconnect->parent->window, 
                lqconnect->config.value[LCRT_Q_SNOT_FOUND]);
        return NULL;
    }
    debug_where();
    strcpy(hostname, basename((char *)shell));
    if (lqconnect->flag != LCRT_QCONNECT_SESSION_OPTION) {
        if ((user = lcrtc_user_create()) == NULL) {
            /* 
             * FIXME: There is no more memory, how can 
             * we handle this exception ?
             */
            return NULL;
        }

        lcrt_user_find_unused_label(lwindow, hostname, name);

        lcrtc_user_set_data(
           user,
           name,
           hostname,
           protocol,
           NULL,
           shell,
           gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
           0,
           TRUE,
           "",
           0
        );
        lcrtc_user_ref(user);
        lcrt_user_add(&lwindow->u_config, user);
        lcrt_window_set_current_user(lwindow, user);
        if (lqconnect->flag == LCRT_QCONNECT_IN_TAB) {
            lcrt_create_terminal(lwindow->w_notebook);
        }
    } else {
        if ((user = lcrt_user_find_by_name(&lwindow->u_config, lqconnect->uname)) != NULL) {
            lcrtc_user_set_data(
               user,
               lqconnect->uname,
               hostname,
               protocol,
               NULL,
               shell,
               gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
               0,
               TRUE,
               NULL,
               -1
            );
        }
    }
    return user;

}

struct lcrt_protocol_callback lcrt_protocol_shell_callbacks = {
    .protocol   = LCRT_PROTOCOL_SHELL,
    .receive    = lcrt_shell_receive,
    .connect    = lcrt_shell_connect,
    .disconnect = lcrt_shell_disconnect,
    .show       = lcrt_shell_show,
    .create     = lcrt_shell_create,
    .changed    = NULL,
};

