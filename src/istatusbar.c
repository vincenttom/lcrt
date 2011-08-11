#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "debug.h"
#include "iwindow.h"
#include "istatusbar.h"
#include "mkconfig.h"

struct lcrt_statusbar *lcrt_create_statusbar(struct lcrt_window *parent)
{
    struct lcrt_statusbar *lstatusbar;
    GtkWidget *statusbar;

    lstatusbar = (struct lcrt_statusbar *)calloc(1, sizeof(struct lcrt_statusbar));
    assert(lstatusbar);

    lstatusbar->parent = parent;

    statusbar = gtk_statusbar_new ();
    lstatusbar->statusbar = statusbar;
    gtk_widget_show (statusbar);
    gtk_widget_set_size_request (statusbar, -1, 20);
    return lstatusbar;
}
void lcrt_destroy_statusbar(struct lcrt_statusbar *lstatusbar)
{
    debug_where();
    gtk_widget_destroy(lstatusbar->statusbar);
    if (lstatusbar)
        free(lstatusbar);
}
