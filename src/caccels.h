#ifndef __LCRT_CACCELS_H__
#define __LCRT_CACCELS_H__
void lcrt_accels_set_data(GtkTreeViewColumn *tree_column,
                GtkCellRenderer   *cell,
                GtkTreeModel      *model,
                GtkTreeIter       *iter,
                gpointer           data);
void lcrt_accels_on_edited(GtkCellRendererAccel *cell,
                       gchar                *path_string,
                       guint                 keyval,
                       GdkModifierType       mask,
                       guint                 hardware_keycode,
                       gpointer              user_data);

#endif
