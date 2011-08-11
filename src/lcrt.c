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

