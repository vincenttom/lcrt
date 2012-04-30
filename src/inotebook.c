//#define __LCRT_DEBUG__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <assert.h>
#include "debug.h"
#include "list.h"
#include "iwindow.h"
#include "inotebook.h"
#include "mkconfig.h"
#include "iterminal.h"
#include "cnotebook.h"
#include "iaccels.h"

int lcrt_notebook_init_config(struct lcrt_notebook *lnotebook);
int lcrt_notebook_load_config(struct lcrt_notebook *lnotebook);

struct lcrt_notebook *lcrt_create_notebook(struct lcrt_window *parent)
{
    struct lcrt_notebook *lnotebook;
    int i;
    GtkWidget *notebook;
    GClosure *closure;
    GtkAccelGroup *accel_group = parent->w_accel;

    lnotebook = (struct lcrt_notebook *)calloc(1, sizeof(struct lcrt_notebook));
    assert(lnotebook);
    lnotebook->parent = parent;

    lcrt_notebook_init_config(lnotebook);
    lcrt_notebook_load_config(lnotebook);
    notebook = gtk_notebook_new ();
    lnotebook->notebook = notebook;
    gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook), TRUE);
    gtk_widget_show (notebook);
    GTK_WIDGET_UNSET_FLAGS (notebook, GTK_CAN_FOCUS);
    gtk_notebook_set_show_border(GTK_NOTEBOOK (notebook), FALSE);
    lcrt_nb_t nb;
    for (nb = 0; nb < LCRT_NB_NUMBER; nb++) {
        if (nb == LCRT_NB_NONE)
            continue;
        closure = g_cclosure_new(G_CALLBACK(lcrt_notebook_control_terminal), lnotebook , NULL);
        debug_print("Attach shortcut %d\n", nb);
        gtk_accel_group_connect(accel_group, lnotebook->config.shortcut[nb][0], 
                            (GdkModifierType)lnotebook->config.shortcut[nb][1],
                            GTK_ACCEL_VISIBLE, closure);
    }
 
    for (i = 0; i < 9; i ++) {
        closure = g_cclosure_new(G_CALLBACK (lcrt_notebook_control_terminal), lnotebook , NULL); 
        gtk_accel_group_connect(accel_group, GDK_1 + i, 
                                (GdkModifierType)GDK_MOD1_MASK,
                                GTK_ACCEL_VISIBLE, closure);
    }
    g_signal_connect((gpointer)notebook, "switch_page",
                    G_CALLBACK(lcrt_notebook_on_switch_page),
                    lnotebook);
    g_signal_connect((gpointer)notebook, "page_removed",
                    G_CALLBACK(lcrt_notebook_on_page_removed),
                    lnotebook);
    INIT_LIST_HEAD(&lnotebook->child);
    return lnotebook;
}
void lcrt_destroy_notebook(struct lcrt_notebook *lnotebook)
{
    debug_where();
    struct lcrt_terminal *lterminal;
    struct list_head *pos, *tmp;

    if (lnotebook == NULL)
        return;
    list_for_each_safe(pos, tmp, &lnotebook->child) {
        lterminal = list_entry(pos, struct lcrt_terminal, brother);
        lcrt_destroy_terminal(lterminal);
    }
    gtk_widget_destroy(lnotebook->notebook);
    free(lnotebook);
}
static const char *lcrt_notebook_get_db_name(struct lcrt_notebook *lnotebook)
{
    return lnotebook->config.db.db_name;
}
static const char *lcrt_notebook_get_tb_name(struct lcrt_notebook *lnotebook)
{
    return lnotebook->config.db.db_table;
}
int lcrt_notebook_init_config(struct lcrt_notebook *lnotebook)
{
    struct lcrt_accels *laccels;
    char db_name[256];
    int i;
    static char *name[LCRT_TM_NUMBER] = {LCRT_TM_NAME};
    static char *value[LCRT_TM_NUMBER] = {LCRT_TM_VALUE};

    assert(lnotebook);
    laccels = lnotebook->parent->w_accels;
    memset(&lnotebook->config, 0, sizeof(struct lcrtc_notebook));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lnotebook->config.db, db_name, LCRT_ITERMINAL_TABLE);
    for (i = 0; i < LCRT_TM_NUMBER; i++) {
        lnotebook->config.name[i] = name[i];
        strncpy(lnotebook->config.value[i], value[i], sizeof(lnotebook->config.value[i]));
    }
    debug_where();

    for (i = 0; i < LCRT_NB_NUMBER; i++) {
        memcpy(lnotebook->config.shortcut[i], 
               laccels->config.key[LCRT_KB_E_COPY + i], 
                sizeof(int) * 2);
    }
    lnotebook->get_db = lcrt_notebook_get_db_name;
    lnotebook->get_tb = lcrt_notebook_get_tb_name;

    return 0;
}
int lcrt_notebook_load_config(struct lcrt_notebook *lnotebook)
{
    int rv, i;

    debug_where();
    assert(lnotebook);
    rv = lnotebook->config.db.select(&lnotebook->config.db, "SELECT * FROM %s", lnotebook->get_tb(lnotebook));
    if (rv == LCRTE_NO_TABLE) {
        return LCRTE_NO_CONFIG;
    }

    debug_where();
    for (i = 0; i < LCRT_TM_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lnotebook->config.value[i], lnotebook->config.db.get_text_col(&lnotebook->config.db, 1), 
                sizeof(lnotebook->config.value[i]));
        rv = lnotebook->config.db.get_row(&lnotebook->config.db);

        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s]\n",
                basename((char *)lnotebook->get_db(lnotebook)),
                lnotebook->get_tb(lnotebook),
                lnotebook->config.name[i],
                lnotebook->config.value[i]);
    }

    return rv;
}

