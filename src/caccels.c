/*
 * Copyright(c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Sun 06 Mar 2011 04:36:06 PM CST
 * File Name   : caccels.c
 *
 * Description:  
 */
#include <gtk/gtk.h>
#include "iaccels.h"
#include "iwindow.h"
#include "message.h"
#include "isettings.h"

void lcrt_accels_set_data(GtkTreeViewColumn *tree_column,
                GtkCellRenderer   *cell,
                GtkTreeModel      *model,
                GtkTreeIter       *iter,
                gpointer           data)
{
    int *key;
    
    gtk_tree_model_get(model, iter,
                        KEYVAL_COLUMN, &key,
                        -1);

    if(key == NULL)
      g_object_set(cell,"visible", FALSE, NULL);
    else
      g_object_set(cell,
                   "visible", TRUE,
                   "accel-key", key[0],
                   "accel-mods", key[1],
                   NULL);
}
void lcrt_accels_on_edited(GtkCellRendererAccel *cell,
                       gchar                *path_string,
                       guint                 keyval,
                       GdkModifierType       mask,
                       guint                 hardware_keycode,
                       gpointer              user_data)
{
    GtkTreeModel *model;
    GtkTreePath *path;
    GtkTreeIter iter;
    char *str;
    int *key;
    int i;
    struct lcrt_accels *laccels = (struct lcrt_accels *)user_data;

    model = gtk_tree_view_get_model(GTK_TREE_VIEW(laccels->tree_view));
    path = gtk_tree_path_new_from_string(path_string);
    if(!path)
        return;

    if(!gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_path_free(path);
        return;
    }

    gtk_tree_model_get(model, &iter, KEYVAL_COLUMN, &key, -1);

    if(key == NULL)
        return;
    for (i = LCRT_KB_F_CONNECT; i <= LCRT_KB_S_SWITCH_PREV; i++) {
        if (laccels->config.key[i][0] == keyval && 
            laccels->config.key[i][1] == mask &&
            laccels->config.key[i] != key) {
            str = gtk_accelerator_get_label(keyval, mask);
            lcrt_message_warning(laccels->parent->w_settings->dialog_settings,
                    laccels->config.value[LCRT_KB_WARN_MSG],
                    str,
                    laccels->config.value[i]);
            return;
        }
    }
    key[0] = keyval;
    key[1] = mask;
    gtk_tree_model_row_changed (model, path, &iter);
    gtk_tree_path_free(path);
    laccels->dirty = TRUE;
}


