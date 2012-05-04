/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sun 23 Jan 2011 02:36:22 AM CST
 *
 *
 * Description:  
 */

#define __LCRT_DEBUG__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <libgen.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "iwindow.h"
#include "debug.h"
#include "message.h"
#include "iaccels.h"
#include "caccels.h"
#include "isettings.h"
#include "foperate.h"

int lcrt_accels_create_local_config(struct lcrt_accels *laccels);
int lcrt_accels_load_config(struct lcrt_accels *laccels);
int lcrt_accels_init_config(struct lcrt_accels *laccels);
int lcrt_accels_add_entry(GtkTreeStore *tree, char name[][LCRT_CONFIG_VALUE_LEN], int key[][2], int number);

struct lcrt_accels *lcrt_create_accels(struct lcrt_window *parent)
{
    GtkWidget *tree_view;
    GtkTreeViewColumn *column;
    GtkCellRenderer *cell_renderer;
    GtkTreeStore *tree;
    struct lcrt_accels *laccels;
    laccels =(struct lcrt_accels *)calloc(1, sizeof(struct lcrt_accels));
    if (laccels == NULL)
        goto err;

    lcrt_accels_init_config(laccels);
    lcrt_accels_load_config(laccels);

    laccels->parent = parent;
    tree_view = parent->w_settings->k_tv_shortcurts;
    laccels->tree_view = tree_view;
    /* Column 1 */
    cell_renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(laccels->config.value[LCRT_KB_ACTION],
                             cell_renderer,
                             "text", ACTION_COLUMN,
                             NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    /* Column 2 */
    cell_renderer = gtk_cell_renderer_accel_new();
    g_object_set(cell_renderer,
                  "editable", TRUE,
                  "accel-mode", GTK_CELL_RENDERER_ACCEL_MODE_GTK,
                  NULL);

    g_signal_connect(cell_renderer, "accel-edited",
                      G_CALLBACK(lcrt_accels_on_edited), laccels);

    column = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(column, laccels->config.value[LCRT_KB_SHORTCUT_KEY]);
    gtk_tree_view_column_pack_start(column, cell_renderer, TRUE);
    gtk_tree_view_column_set_cell_data_func(column, cell_renderer, lcrt_accels_set_data, NULL, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    tree = gtk_tree_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_POINTER);
    lcrt_accels_add_entry(tree, 
                          &laccels->config.value[LCRT_KB_FILE], 
                          &laccels->config.key[LCRT_KB_FILE], 4);
    lcrt_accels_add_entry(tree, 
                          &laccels->config.value[LCRT_KB_EDIT], 
                          &laccels->config.key[LCRT_KB_EDIT], 3);
    lcrt_accels_add_entry(tree, 
                          &laccels->config.value[LCRT_KB_SESSION], 
                          &laccels->config.key[LCRT_KB_SESSION], 6);

    gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(tree));
    g_object_unref(tree);

    gtk_tree_view_expand_all(GTK_TREE_VIEW(tree_view));

    return laccels;
err:    
    lcrt_message_error(parent->window, laccels->config.value[LCRT_KB_ERR_MSG]);
    return NULL;
}
int lcrt_accels_add_entry(GtkTreeStore *tree, char name[][LCRT_CONFIG_VALUE_LEN], int key[][2], int number)
{
    GtkTreeIter parent_iter;
    GtkTreeIter iter;
    int j;
    
    gtk_tree_store_append(tree, &parent_iter, NULL);
    gtk_tree_store_set(tree, &parent_iter,
                  ACTION_COLUMN, name[0],
                  -1);
    
    for (j = 1; j <= number; j++) {
    
        gtk_tree_store_insert_with_values(tree, &iter, &parent_iter, -1,
            ACTION_COLUMN, name[j],
            KEYVAL_COLUMN, key[j],
            -1);
    }
    return 0;
}


void lcrt_destroy_accels(struct lcrt_accels *laccels)
{
    if (laccels->parent->w_settings->auto_save_config && laccels->dirty)
        lcrt_accels_save_local_config(laccels);

    if (laccels)
        free(laccels);
}
static const char *lcrt_accels_get_ldb_name(struct lcrt_accels *laccels)
{
    return laccels == NULL ? NULL : laccels->config.ldb.db_name;
}
static const char *lcrt_accels_get_ltb_name(struct lcrt_accels *laccels)
{
    return laccels == NULL ? NULL : laccels->config.ldb.db_table;
}

static const char *lcrt_accels_get_db_name(struct lcrt_accels *laccels)
{
    return laccels->config.db.db_name;
}
static const char *lcrt_accels_get_tb_name(struct lcrt_accels *laccels)
{
    return laccels->config.db.db_table;
}

int lcrt_accels_init_language_config(struct lcrt_accels *laccels)
{
    char db_name[256];
    int i;
    static char *name[LCRT_KB_NUMBER] = {LCRT_KB_NAME};
    static char *value[LCRT_KB_NUMBER] = {LCRT_KB_VALUE};

    assert(laccels);

    memset(&laccels->config, 0, sizeof(struct lcrtc_accels));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&laccels->config.db, db_name, LCRT_IACCELS_TABLE);

    for (i = 0; i < LCRT_KB_NUMBER; i++) {
        laccels->config.name[i] = name[i];
        strncpy(laccels->config.value[i], value[i], sizeof(laccels->config.value[i]));
    }
    laccels->get_db = lcrt_accels_get_db_name;
    laccels->get_tb = lcrt_accels_get_tb_name;

    return 0;
}
int lcrt_accels_init_local_config(struct lcrt_accels *laccels)
{
    char db_name[256];
    int i;
    int key[LCRT_KB_NUMBER][2] = {LCRT_KB_KEY};

    snprintf(db_name, sizeof(db_name), "%s/%s/", lcrt_config_get_local_dir(), LCRT_ACCELS_CONFIG_DIR);
    lcrt_fmkdir(db_name);
    strcat(db_name, LCRT_ACCELS_DB);
    lcrt_config_init(&laccels->config.ldb, db_name, LCRT_IACCELS_TABLE);

    for (i = 0; i < LCRT_KB_NUMBER; i++) {
        laccels->config.key[i][0] = key[i][0];
        laccels->config.key[i][1] = key[i][1];
        debug_print("name = [%-20s] value = [%-20s] shortcut = {%-5d, %d}\n",
                laccels->config.name[i],
                laccels->config.value[i],
                laccels->config.key[i][0],
                laccels->config.key[i][1]);
    }
    laccels->get_ldb = lcrt_accels_get_ldb_name;
    laccels->get_ltb = lcrt_accels_get_ltb_name;

    return 0;
}
int lcrt_accels_init_config(struct lcrt_accels *laccels)
{
    assert(laccels);

    memset(&laccels->config, 0, sizeof(struct lcrtc_accels));
    lcrt_accels_init_language_config(laccels);
    lcrt_accels_init_local_config(laccels);
    return LCRTE_OK;
}
int lcrt_accels_load_language_config(struct lcrt_accels *laccels)
{
    int rv, i;

    assert(laccels);

    rv = laccels->config.db.select(&laccels->config.db, "SELECT * FROM %s", laccels->get_tb(laccels));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_KB_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(laccels->config.value[i], laccels->config.db.get_text_col(&laccels->config.db, 1), 
                sizeof(laccels->config.value[i]));
        rv = laccels->config.db.get_row(&laccels->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s]\n",
                basename((char *)laccels->get_db(laccels)),
                laccels->get_tb(laccels),
                laccels->config.name[i],
                laccels->config.value[i]);
    }

    return rv;
}
int lcrt_accels_load_local_config(struct lcrt_accels *laccels)
{
    int rv, i;
    char *name[LCRT_KB_NUMBER] = {LCRT_KB_NAME};

    assert(laccels);

    rv = laccels->config.ldb.select(&laccels->config.ldb, "SELECT * FROM %s", laccels->get_ltb(laccels));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        lcrt_accels_create_local_config(laccels);
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_KB_NUMBER && rv == LCRTE_OK; i++) {
        laccels->config.key[i][0] = laccels->config.ldb.get_int_col(&laccels->config.ldb, 1);
        laccels->config.key[i][1] = laccels->config.ldb.get_int_col(&laccels->config.ldb, 2);
        rv = laccels->config.ldb.get_row(&laccels->config.ldb);
        debug_print("[%-10s|%-10s]: name = [%-20s] shortcut = {%-5d, %d}\n",
                basename((char *)laccels->get_ldb(laccels)),
                laccels->get_ltb(laccels),
                name[i],
                laccels->config.key[i][0],
                laccels->config.key[i][1]);
    }

    return rv;
}
int lcrt_accels_load_config(struct lcrt_accels *laccels)
{
    lcrt_accels_load_language_config(laccels);
    lcrt_accels_load_local_config(laccels);
    return LCRTE_OK;
}


int lcrt_accels_create_local_config(struct lcrt_accels *laccels)
{
    int i;

    char *name[LCRT_KB_NUMBER] = {LCRT_KB_NAME};
    assert(laccels);
    laccels->config.ldb.exec(&laccels->config.ldb, 
                         "CREATE TABLE %s( \
                          name VARCHAR(%d) PRIMARY KEY, \
                          key INTEGER, \
                          mask INTEGER)",
                         laccels->get_ltb(laccels),
                         LCRT_CONFIG_NAME_LEN);
    for (i = 0; i < LCRT_KB_NUMBER; i++) {
        laccels->config.ldb.exec(&laccels->config.ldb,
                             "INSERT INTO %s VALUES('%s', %d, %d)",
                             laccels->get_ltb(laccels),
                             name[i],
                             laccels->config.key[i][0],
                             laccels->config.key[i][1]);
    }
    laccels->config.ldb.close(&laccels->config.ldb);
    return 0;
}
int lcrt_accels_save_local_config(struct lcrt_accels *laccels)
{
    int i;
    char *name[LCRT_KB_NUMBER] = {LCRT_KB_NAME};

    if(laccels == NULL)
        return EINVAL;

    for (i = 0; i < LCRT_KB_NUMBER; i++) {
        laccels->config.ldb.exec(&laccels->config.ldb, 
            "UPDATE %s SET key=%d, mask=%d WHERE name='%s'",
            laccels->get_ltb(laccels),
            laccels->config.key[i][0],
            laccels->config.key[i][1],
            name[i]);
    
        if (laccels->config.ldb.changes(&laccels->config.ldb) == 0) {
            laccels->config.ldb.exec(&laccels->config.ldb,
                           "INSERT INTO %s VALUES('%s', %d, %d)",
                           laccels->get_ltb(laccels),
                           name[i],
                           laccels->config.key[i][0],
                           laccels->config.key[i][1]);
        }
    }
    laccels->config.ldb.close(&laccels->config.ldb);
    return LCRTE_OK;
}

