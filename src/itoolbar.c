#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <libgen.h>
#include "support.h"
#include "iwindow.h"
#include "imenubar.h"
#include "debug.h"
#include "mkconfig.h"
#include "itoolbar.h"
#include "ctoolbar.h"
#include "cfile.h"

int lcrt_toolbar_create_config(struct lcrt_toolbar *ltoolbar);
int lcrt_toolbar_load_config(struct lcrt_toolbar *ltoolbar);
int lcrt_toolbar_init_config(struct lcrt_toolbar *ltoolbar);
int lcrt_toolbar_create_toolitem(struct lcrt_toolbar *ltoolbar);

struct lcrt_toolbar *lcrt_create_toolbar(struct lcrt_window *parent)
{
    struct lcrt_toolbar *ltoolbar;

    GtkWidget *toolbar;
    //GtkAccelGroup *accel_group = parent->w_accel;

    ltoolbar =(struct lcrt_toolbar *)malloc(sizeof(struct lcrt_toolbar));
    
    assert(ltoolbar);

    lcrt_toolbar_init_config(ltoolbar);
    lcrt_toolbar_load_config(ltoolbar);
    ltoolbar->parent = parent;

    toolbar = gtk_toolbar_new ();
    ltoolbar->toolbar = toolbar;

    gtk_widget_set_size_request (toolbar, -1, 30);
    gtk_container_set_border_width (GTK_CONTAINER (toolbar), 0);
    //gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_ICONS);
    gtk_toolbar_set_orientation (GTK_TOOLBAR (toolbar), GTK_ORIENTATION_HORIZONTAL);
    gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_BOTH);
    gtk_widget_show (toolbar);


    lcrt_toolbar_create_toolitem(ltoolbar);

    return ltoolbar;
}
void lcrt_destroy_toolbar(struct lcrt_toolbar *ltoolbar)
{
    debug_where();
    gtk_widget_destroy(ltoolbar->toolbar);
    if (ltoolbar)
        free(ltoolbar);
}
int lcrt_toolbar_create_toolitem(struct lcrt_toolbar *ltoolbar)
{
    GtkWidget *icon;
    int i;
    static const char *toolitem_icon[LCRT_B_NUMBER] = {LCRT_B_ICONS};
    typedef void  (*callbacks)(GtkWidget *widget, gpointer data);
    callbacks callback[LCRT_B_NUMBER] = {LCRT_B_CALLBACK};

    assert(ltoolbar);

    for (i = 0; i < LCRT_B_NUMBER; i++) {
        icon = create_pixmap (NULL, toolitem_icon[i]);
        ltoolbar->toolitem[i] = gtk_toolbar_append_element (
                    GTK_TOOLBAR (ltoolbar->toolbar),
                    GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                    NULL, NULL,
                    ltoolbar->config.value[i],
                    NULL,
                    icon,
                    GTK_SIGNAL_FUNC (callback[i]),
                    ltoolbar);
        gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (ltoolbar->toolitem[i]), TRUE);
        GTK_WIDGET_UNSET_FLAGS (ltoolbar->toolitem[i], GTK_CAN_FOCUS);
        if (i == 2 || i == 4 || i == 6)
            gtk_toolbar_append_space (GTK_TOOLBAR (ltoolbar->toolbar));
    }
    gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_RECONNECT], FALSE);
    gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_DISCONNECT], FALSE);
    gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_COPY], FALSE);
    gtk_widget_set_sensitive(ltoolbar->toolitem[LCRT_B_PASTE], FALSE);
    return LCRTE_OK;
}
static const char *lcrt_toolbar_get_db_name(struct lcrt_toolbar *ltoolbar)
{
    return ltoolbar == NULL ? NULL : ltoolbar->config.db.db_name;
}
static const char *lcrt_toolbar_get_tb_name(struct lcrt_toolbar *ltoolbar)
{
    return ltoolbar == NULL ? NULL : ltoolbar->config.db.db_table;
}

int lcrt_toolbar_init_config(struct lcrt_toolbar *ltoolbar)
{
    char db_name[256];
    int i;
    static char *name[LCRT_B_NUMBER] = {LCRT_B_NAME};
    static char *value[LCRT_B_NUMBER] = {LCRT_B_VALUE};

    assert(ltoolbar);

    memset(&ltoolbar->config, 0, sizeof(struct lcrtc_toolbar));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&ltoolbar->config.db, db_name, LCRT_ITOOLBAR_TABLE);
    for (i = 0; i < LCRT_B_NUMBER; i++) {
        ltoolbar->config.name[i] = name[i];
        strncpy(ltoolbar->config.value[i], value[i], sizeof(ltoolbar->config.value[i]));
    }
    ltoolbar->get_db = lcrt_toolbar_get_db_name;
    ltoolbar->get_tb = lcrt_toolbar_get_tb_name;

    return 0;
}
int lcrt_toolbar_load_config(struct lcrt_toolbar *ltoolbar)
{
    int rv, i;

    assert(ltoolbar); 

    rv = ltoolbar->config.db.select(&ltoolbar->config.db, "SELECT * FROM %s", ltoolbar->get_tb(ltoolbar));
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_toolbar_create_config(ltoolbar);
        return LCRTE_NO_CONFIG;
    }

    for (i = 0; i < LCRT_B_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(ltoolbar->config.value[i], ltoolbar->config.db.get_text_col(&ltoolbar->config.db, 1),
                sizeof(ltoolbar->config.value[i]));
        rv = ltoolbar->config.db.get_row(&ltoolbar->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s]\n",
                basename((char *)ltoolbar->get_db(ltoolbar)),
                ltoolbar->get_tb(ltoolbar),
                ltoolbar->config.name[i],
                ltoolbar->config.value[i]);
    }

    return rv;
}
int lcrt_toolbar_create_config(struct lcrt_toolbar *ltoolbar)
{
    int i;
    if (ltoolbar == NULL)
        return EINVAL;
    ltoolbar->config.db.exec(&ltoolbar->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255))",
                         ltoolbar->get_tb(ltoolbar));
    for (i = 0; i < LCRT_B_NUMBER; i++) {
        ltoolbar->config.db.exec(&ltoolbar->config.db,
                             "INSERT INTO %s VALUES('%s', '%s')",
                             ltoolbar->get_tb(ltoolbar),
                             ltoolbar->config.name[i], 
                             ltoolbar->config.value[i]);
    }
    ltoolbar->config.db.close(&ltoolbar->config.db);
    return 0;
}

