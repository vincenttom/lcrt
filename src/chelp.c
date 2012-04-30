#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "message.h"
#include "debug.h"
#include "ihelp.h"
#include "iabout.h"
#include "mkconfig.h"
#include "foperate.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#error "missing config.h"
#endif

#define DOWNLOAD_PAGE "http://lcrt.googlecode.com/files"

static gboolean open_url(GtkWindow * parent, const char *uri, guint32 user_time, GError ** error)
{
    GdkScreen *screen;

    if (parent)
        screen = gtk_widget_get_screen(GTK_WIDGET(parent));
    else
        screen = gdk_screen_get_default();

    return gtk_show_uri(screen, uri, user_time, error);
}

void lcrt_help_show_help(const char *topic, struct lcrt_help *lhelp)
{
    char *uri = NULL, *url;
    int i;
    char help_path[3][256];
    char language[32], *p;

    strcpy(language, basename((char *)lcrt_config_get_language()));
    p = strchr(language, '.');
    if (p)
        *p = '\0';

    snprintf(help_path[0], 256, LCRT_HELP_DIR"/%s/lcrt.xml", language);
    snprintf(help_path[1], 256, "%s/help/%s/lcrt.xml", getenv("PWD"), language);
    snprintf(help_path[2], 256, "%s/../help/%s/lcrt.xml", getenv("PWD"), language);
    for (i = 0; i < 3; i++) {
        if (access(help_path[i], F_OK | R_OK) == 0) {
            uri = help_path[i];
            break;
        }
    }
    if (uri == NULL) {
        lcrt_message_error(lhelp->parent->parent->window,
                           lhelp->config.value[LCRT_H_MISSING_DOC]);
        return;
    }
    if (topic) {
        url = g_strdup_printf("ghelp://%s?%s", uri, topic);
    } else {
        url = g_strdup_printf("ghelp://%s", uri);
    }

    open_url(GTK_WINDOW(lhelp->parent->parent->window), url, gtk_get_current_event_time(), NULL);

    g_free(url);
}

void lcrt_help_on_help_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    struct lcrt_help *lhelp = (struct lcrt_help *)user_data;
    lcrt_help_show_help(NULL, lhelp);
}

void lcrt_help_on_home_page_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    struct lcrt_help *lhelp = (struct lcrt_help *)user_data;
    open_url(GTK_WINDOW(lhelp->parent->parent->window), 
             "http://code.google.com/p/lcrt", 
             gtk_get_current_event_time(), NULL);
}
gboolean lcrt_help_check_update(gpointer user_data)
{
    struct lcrt_help *lhelp = (struct lcrt_help *)user_data;
    int m,n,t;
    char command[256] = "wget "DOWNLOAD_PAGE"/version.h -O /tmp/version.h >/dev/null 2>&1";
    int fd;

    system(command);

    if ((fd = open("/tmp/version.h", O_RDONLY)) == -1) {
        return FALSE;
    }
    memset(command, '\0', 256);
    read(fd, command, 32);
    close(fd);
    lcrt_fremove("/tmp/version.h");
    if (sscanf(command, "%d.%d.%d", &m, &n, &t) != 3) {
        lcrt_message_error(lhelp->parent->parent->window,
                           lhelp->config.value[LCRT_H_CHECK_UPDATE_ERR]);
        return FALSE;
    }
    sprintf(command, "%d.%d.%d", m, n, t);
    if (strcmp(command, PACKAGE_VERSION) <= 0) {
        lcrt_message_info(lhelp->parent->parent->window,
                lhelp->config.value[LCRT_H_NO_NEW_VERSION]);
    } else {
        gtk_show_about_dialog(GTK_WINDOW(lhelp->parent->parent->window),
                          "version", command,
                          "comments", lhelp->config.value[LCRT_H_NEW_VERSION],
                          "website", DOWNLOAD_PAGE,
                          NULL);
#if 0
        lcrt_message_info(lhelp->parent->parent->window,
                lhelp->config.value[LCRT_H_NEW_VERSION],
                command, DOWNLOAD_PAGE);
#endif
    }
    return FALSE;
}
void lcrt_help_on_download_page_activate(GtkMenuItem * menuitem, gpointer user_data)
{
#if 0
    struct lcrt_help *lhelp = (struct lcrt_help *)user_data;
    open_url(GTK_WINDOW(lhelp->parent->parent->window), 
             "http://code.google.com/p/lcrt/downloads/list", 
             gtk_get_current_event_time(), NULL);
#else
    gtk_timeout_add(1, lcrt_help_check_update, user_data);
#endif
}

void h_on_about_activate(GtkMenuItem * menuitem, gpointer user_data)
{
    lcrt_abount_dialog(user_data);
}
