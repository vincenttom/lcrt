//#define __LCRT_DEBUG__
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "iwindow.h"
#include "cwindow.h"
#include "imenubar.h"
#include "itoolbar.h"
#include "inotebook.h"
#include "istatusbar.h"
#include "istatus.h"
#include "support.h"
#include "mkconfig.h"
#include "debug.h"
#include "foperate.h"
#include "user.h"
#include "cview.h"
#include "isettings.h"
#include "iaccels.h"

int lcrt_window_init_config(struct lcrt_window *lwindow);
int lcrt_window_load_config(struct lcrt_window *lwindow);
int lcrt_window_create_config(struct lcrt_window *lwindow);
void lcrt_window_set_status(struct lcrt_window *lwindow);

struct lcrt_window *lcrt_create_window(void)
{
    debug_where();
    GtkWidget *window;
    GtkWidget *vbox;
    GtkAccelGroup *accel;
    GdkPixbuf *pixpic;

    struct lcrt_window *lwindow;

    lwindow = (struct lcrt_window *)calloc(1, sizeof(struct lcrt_window));
    assert(lwindow);

    lcrt_window_init_config(lwindow);
    lcrt_window_load_config(lwindow);
    lcrt_user_init_config(&lwindow->u_config);
    lcrt_user_load_config(&lwindow->u_config);

    accel = gtk_accel_group_new ();
    lwindow->w_accel = accel;

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    lwindow->window = window;
    gtk_widget_set_size_request(window, 640, 480);
    gtk_window_set_default_size (GTK_WINDOW(window), 
                                 lwindow->w_config.value[LCRT_W_WIDTH], 
                                 lwindow->w_config.value[LCRT_W_HEIGHT]);
    gtk_window_set_title (GTK_WINDOW (window), "LCRT");
    pixpic = create_pixbuf ("lcrt-logo.xpm", 16, 16); 
    if (pixpic){
        gtk_window_set_icon (GTK_WINDOW (window), pixpic);
        gdk_pixbuf_unref (pixpic);
    }
    lwindow->w_status = lcrt_create_status(lwindow);
    if ((lwindow->w_config.value[LCRT_W_START_X] |
        lwindow->w_config.value[LCRT_W_START_Y]) == 0) {
        gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    } else {
        gtk_window_move(GTK_WINDOW(window),
                        lwindow->w_config.value[LCRT_W_START_X],
                        lwindow->w_config.value[LCRT_W_START_Y]);
    }

    g_signal_connect ((gpointer) window, "delete_event",
                    G_CALLBACK(lcrt_window_on_delete_event),
                    lwindow);
    g_signal_connect ((gpointer) window, "configure_event",
                    G_CALLBACK(lcrt_window_on_configure_event),
                    lwindow);
    g_signal_connect ((gpointer) window, "window_state_event",
                    G_CALLBACK(lcrt_window_on_state_event),
                    lwindow);
    g_signal_connect ((gpointer) window, "key_press_event",
                    G_CALLBACK(lcrt_window_on_key_press_event),
                    lwindow);
    gtk_widget_show(window);
    lwindow->hide = FALSE;
    debug_where();
    lwindow->w_settings = lcrt_create_settings(lwindow);
    debug_where();
    assert(lwindow->w_settings);
    lwindow->w_accels = lcrt_create_accels(lwindow);
    assert(lwindow->w_accels);

    vbox = gtk_vbox_new (FALSE, 0);
    lwindow->w_vbox = vbox;
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    debug_where();   
    lwindow->w_menubar = lcrt_create_menubar(lwindow);
    assert(lwindow->w_menubar);
    gtk_box_pack_start (GTK_BOX (vbox), lwindow->w_menubar->menubar, FALSE, FALSE, 0);

    lwindow->w_toolbar = lcrt_create_toolbar(lwindow);
    assert(lwindow->w_toolbar);
    gtk_box_pack_start (GTK_BOX (vbox), lwindow->w_toolbar->toolbar, FALSE, FALSE, 0);
    debug_where();
    lwindow->w_notebook = lcrt_create_notebook(lwindow);
    assert(lwindow->w_notebook);
    gtk_box_pack_start (GTK_BOX (vbox), lwindow->w_notebook->notebook, TRUE, TRUE, 0);

    lwindow->w_statusbar = lcrt_create_statusbar(lwindow);
    assert(lwindow->w_statusbar);
    gtk_box_pack_start (GTK_BOX (vbox), lwindow->w_statusbar->statusbar, FALSE, FALSE, 0);
    gtk_window_add_accel_group (GTK_WINDOW (window), accel);

    lcrt_window_set_status(lwindow);

    return lwindow;

}
void lcrt_window_set_status(struct lcrt_window *lwindow)
{
    struct lcrt_view *lview = lwindow->w_menubar->m_view;

    lcrt_view_on_toolbar_activate(NULL, lview);
    lcrt_view_on_status_bar_activate(NULL, lview);
    lcrt_view_on_always_on_top_activate(NULL, lview);
}

void lcrt_destroy_window(struct lcrt_window *lwindow)
{
    debug_where();
    if (lwindow == NULL)
        return;

    lcrt_destroy_notebook(lwindow->w_notebook);
    lwindow->w_notebook = NULL;

    lcrt_destroy_menubar(lwindow->w_menubar);
    lwindow->w_menubar = NULL;

    lcrt_destroy_toolbar(lwindow->w_toolbar);
    lwindow->w_toolbar = NULL;

    lcrt_destroy_statusbar(lwindow->w_statusbar);
    lwindow->w_statusbar = NULL;

    lcrt_destroy_status(lwindow->w_status);
    lwindow->w_status = NULL;

    lcrt_user_destroy_config(&lwindow->u_config);
    lcrt_destroy_accels(lwindow->w_accels);
    lcrt_destroy_settings(lwindow->w_settings);

    free(lwindow);
}
static const char *lcrt_window_get_db_name(struct lcrt_window *lwindow)
{
    return lwindow->w_config.db.db_name;
}
static const char *lcrt_window_get_tb_name(struct lcrt_window *lwindow)
{
    return lwindow->w_config.db.db_table;
}

int lcrt_window_init_config(struct lcrt_window *lwindow)
{
    char db_name[256];
    int i;
    static const char *name[LCRT_W_NUMBER] = {LCRT_W_NAME};
    static int value[LCRT_W_NUMBER] = {LCRT_W_VALUE};

    assert(lwindow);

    memset(&lwindow->w_config, 0, sizeof(struct lcrtc_window));
    lwindow->full_screen = FALSE;
    snprintf(db_name, sizeof(db_name), "%s/%s/", lcrt_config_get_local_dir(), LCRT_WINDOW_CONFIG_DIR);
    lcrt_mkdir(db_name);
    strcat(db_name, LCRT_WINDOW_DB);
    lcrt_config_init(&lwindow->w_config.db, db_name, LCRT_IWINDOW_TABLE);
    for (i = 0; i < LCRT_W_NUMBER; i++) {
        lwindow->w_config.name[i] = name[i];
        lwindow->w_config.value[i] = value[i];
    }
    lwindow->get_db = lcrt_window_get_db_name;
    lwindow->get_tb = lcrt_window_get_tb_name;

    return 0;
}
int lcrt_window_load_config(struct lcrt_window *lwindow)
{
    int rv, i;

    assert(lwindow);
    debug_print("SQL:SELECT * FROM %s\n", lwindow->get_tb(lwindow));
    rv = lwindow->w_config.db.select(&lwindow->w_config.db, "SELECT * FROM %s", lwindow->get_tb(lwindow));
    if (rv == LCRTE_NO_TABLE) {
        lcrt_window_create_config(lwindow);
        return LCRTE_NO_CONFIG;
    }

    for (i = 0; i < LCRT_W_NUMBER && rv == LCRTE_OK; i++) {
        lwindow->w_config.value[i] = lwindow->w_config.db.get_int_col(&lwindow->w_config.db, 1);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%d]\n", 
                lwindow->get_db(lwindow),
                lwindow->get_tb(lwindow),
                lwindow->w_config.name[i], 
                lwindow->w_config.value[i]);
        rv = lwindow->w_config.db.get_row(&lwindow->w_config.db);
    }

    return rv;
}
int lcrt_window_save_config(struct lcrt_window *lwindow)
{
    int i;

    lwindow->w_config.value[LCRT_W_WIDTH]  = lwindow->window->allocation.width;
    lwindow->w_config.value[LCRT_W_HEIGHT] = lwindow->window->allocation.height;

    for (i = 0; i < LCRT_W_NUMBER; i++) {
        lwindow->w_config.db.exec(&lwindow->w_config.db, 
                "UPDATE %s SET value=%d WHERE name='%s'",
                lwindow->get_tb(lwindow),
                lwindow->w_config.value[i],
                lwindow->w_config.name[i]);
    }
    lwindow->w_config.db.close(&lwindow->w_config.db);
    return 0;
}
int lcrt_window_create_config(struct lcrt_window *lwindow)
{
    int i;

    assert(lwindow);

    lwindow->w_config.db.exec(&lwindow->w_config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value INTEGER)",
                         lwindow->get_tb(lwindow));
    debug_where();
    for (i = 0; i < LCRT_W_NUMBER; i++) {
        lwindow->w_config.db.exec(&lwindow->w_config.db,
                             "INSERT INTO %s VALUES('%s', %d)",
                             lwindow->get_tb(lwindow),
                             lwindow->w_config.name[i], 
                             lwindow->w_config.value[i]);

    }
    debug_where();
    lwindow->w_config.db.close(&lwindow->w_config.db);
    return 0;
}
int lcrt_window_set_current_user(struct lcrt_window *lwindow, struct lcrtc_user *user)
{
    if (lwindow && user) {
        lwindow->current_user = user;
        return LCRTE_OK;
    }
    return EINVAL;

}
