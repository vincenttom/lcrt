/**
 * @mainpage Introduction
 *
 * <h2>What is lcrt?</h2>
 *
 * lcrt is a Linux remote login tool. Just like ssh or telnet, but it has a 
 * GUI and can manage many connections. User can use it to save connection 
 * information such as hostname, username and password(optionally), and 
 * auto login next time. lcrt also can clone a connected session, so user 
 * can open many connections quickly.
 *
 * <h2>Why choose lcrt?</h2>
 *
 * As we known, ssh is a popular login tool to connect to remote server, but
 * in Linux system, it only have a command line programe.Every time when you
 * connect to remote server, you should enter user name and password. If you
 * are a system administrator and manage hundreds of server, or you are a 
 * software engineer in embedded field, you may need open several connection
 * to watch different state of your programe, this time you should enter
 * user name and password again and again,this will be very troublesome.\n
 * So lcrt can help you to manage user information(such as user name and 
 * password), auto login at next time. And, lcrt can clone a connected 
 * session so you can open many connections as you can.
 *
 * <h2>Where does lcrt come from?</h2>
 *
 * About December 2010, i visit to SLUG(shlug@googlegroups.com), In one 
 * thread, someone want to find a useful graphic ssh login tool in Linux,
 * like <em>SecureCRT</em>(http://www.vandyke.com/products/securecrt/), the other
 * one recommend he to use <em>PAC</em>(http://sourceforge.net/projects/pacmanager/),
 * so i try to use PAC beacuse i should login to several board to debug my
 * programe in job, and i should reboot frequently. But i also heop it can 
 * provide a interface like gnome-terminal and it's shortkey. So i decided 
 * to develop a graphic login tool like SecureCRT, i named <em>lcrt</em>.
 *
 * <h2>What's in a name?</h2>
 *
 * The name <em>lcrt</em> means Linux SecureCRT, i want it can be help the 
 * people want to use SecureCRT in linux.
 *
 * <h2>Features</h2>
 * - auto save connection information so that can auto login remote server 
 *   next time.
 * - easy to clone a connected session.
 * - support ssh, telnet, serial, the others(rlogin, raw and etc) will be 
 *   support in future.
 * - open many connections in tables and use useful shortkey to switch
 *   from one to another.
 */
/**
 * @file lcrt.c
 * <h2>License</h2>
 * Copyright (c) 2011 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * @author Niu Tao <niutao0602@gmail.com>
 * @version $Id$
 * @date Sat 15 Jan 2011 04:24:46 PM CST
 *
 * @brief The entry of lcrt
 */

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <signal.h>
#include <gtk/gtk.h>
#include "iwindow.h"
#include "iterminal.h"
#include "support.h"
#include "mkconfig.h"
#include "debug.h"
#include "user.h"
#include "cabout.h"

void usage()
{
    fprintf(stderr,
"usage: lcrt [-q connect_name] [-c connect_name]\n"
"   -q quick connect in commandline(don't start GUI)\n"
"   -c connect in tab\n"
"example:\n"
"   lcrt -q \"localhost (1)\"\n"
"   lcrt -c \"localhost (1)\"\n");
    exit(1);
}
int main(int argc, char *argv[])
{
    debug_where();
    struct lcrt_window *lwindow;
    int opt;
    char qconnect_name[256], connect_name[256];
    int has_qconnect, has_connect;
    struct lcrtc_user *user;
    struct lcrt_user user_config;

    has_qconnect = has_connect = 0;
#ifdef ENABLE_NLS
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
#endif

    gtk_set_locale ();
    gtk_init (&argc, &argv);
    lcrt_config_load();

    signal(SIGCHLD, SIG_IGN);
    add_pixmap_directory (LCRT_INSTALL_DIR"/pixmaps");
    add_pixmap_directory ("./pixmaps");
    add_pixmap_directory ("../pixmaps");

    while ((opt = getopt(argc, argv, "cqh:")) > 0) {
        switch (opt) {
        case 'q':
            has_qconnect = 1;
            if (argv[optind] == NULL)
                usage();
            strcpy(qconnect_name, argv[optind]);
            break;
        case 'c':
            has_connect = 1;
            if (argv[optind] == NULL)
                usage();
            strcpy(connect_name, argv[optind]);
            break;
        case 'h':
            usage();
            break;
        }
    }
    if ((has_qconnect)) {
        lcrt_user_init_config(&user_config);
        lcrt_user_load_config(&user_config);
        user = lcrt_user_find_by_name(&user_config, qconnect_name);
        if (user == NULL) {
            fprintf(stdout, "Not found user '%s' in database\n", qconnect_name);
            lcrt_user_destroy_config(&user_config);
            exit(1);
        }
        fprintf(stderr, "current version not support\n");
        lcrt_user_destroy_config(&user_config);
        return 0;
    } 
    lwindow = lcrt_create_window();
    strcpy(lwindow->programe_name, argv[0]);
    if (has_connect) {
        if ((user = lcrt_user_find_by_name(&lwindow->u_config, connect_name)) != NULL) {
            lcrt_window_set_current_user(lwindow, user);
            lcrt_create_terminal(lwindow->w_notebook);
        } else {
            fprintf(stdout, "Not found user '%s' in database\n", connect_name);
        }
    }

    gtk_about_dialog_set_url_hook(lcrt_about_url_hook, NULL, NULL);
    gtk_about_dialog_set_email_hook(lcrt_about_email_hook, NULL, NULL);
    gtk_main ();
    return 0;
}

