/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Wed 26 Jan 2011 12:22:55 AM CST
 *
 * Description:  
 */
//#undef __LCRT_DEBUG__
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
#include "iconnect.h"
#include "cconnect.h"
#include "support.h"
#include "debug.h"
#include "message.h"
#include "mkconfig.h"

int lcrt_connect_init_config(struct lcrt_connect *lconnect);
int lcrt_connect_load_config(struct lcrt_connect *lconnect);
int lcrt_connect_create_config(struct lcrt_connect *lconnect);
int lcrt_connect_create_toolitem(struct lcrt_connect *lconnect);
static GtkWidget *lcrt_connect_create_view(struct lcrt_connect *lconnect);

int lcrt_create_connect(struct lcrt_window *parent, gboolean tab)
{
    GtkWidget *dialog_connect;
    GtkWidget *dialog_vbox;
    GtkWidget *vbox;
    GtkWidget *toolbar;
    GtkWidget *scrolledwindow;
    GtkWidget *treeview;
    GtkWidget *checkbutton;
    GtkWidget *dialog_action_area;
    GtkWidget *connectbutton;
    GtkWidget *cancelbutton;
    GtkTreeSelection *selection;
    struct lcrt_connect *lconnect;
    int rv = 0;

    lconnect = (struct lcrt_connect *)calloc(1, sizeof(struct lcrt_connect));
    
    if (lconnect == NULL)
        goto err;

    lconnect->parent = parent;
    lconnect->tab = tab;
    debug_where();

    lcrt_connect_init_config(lconnect);
    lcrt_connect_load_config(lconnect);

    dialog_connect = gtk_dialog_new ();
    lconnect->c_connect = dialog_connect;
    gtk_widget_set_size_request (dialog_connect, 320, 320);
    gtk_window_set_transient_for(GTK_WINDOW(dialog_connect), 
            GTK_WINDOW(lconnect->parent->window));
    gtk_window_set_modal(GTK_WINDOW(dialog_connect), TRUE);
    gtk_window_set_title (GTK_WINDOW (dialog_connect), lconnect->config.value[LCRT_C_CONNECT_IN_TAB + !tab]);
    gtk_window_set_position (GTK_WINDOW (dialog_connect), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_window_set_resizable (GTK_WINDOW (dialog_connect), FALSE);
    gtk_window_set_type_hint (GTK_WINDOW (dialog_connect), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_dialog_set_has_separator (GTK_DIALOG (dialog_connect), FALSE);

    g_signal_connect ((gpointer) dialog_connect, "delete_event",
                    G_CALLBACK (lcrt_connect_on_window_delete),
                    lconnect);
    dialog_vbox = GTK_DIALOG (dialog_connect)->vbox;
    gtk_widget_show (dialog_vbox);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox);
    gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

    toolbar = gtk_toolbar_new ();
    gtk_widget_show (toolbar);
    gtk_box_pack_start (GTK_BOX (vbox), toolbar, FALSE, FALSE, 0);
    gtk_widget_set_size_request (toolbar, -1, 35);
    gtk_toolbar_set_orientation (GTK_TOOLBAR (toolbar), GTK_ORIENTATION_HORIZONTAL);
    gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_ICONS);

    lconnect->c_toolbar = toolbar;
    lcrt_connect_create_toolitem(lconnect);

    scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_show (scrolledwindow);
    gtk_box_pack_start (GTK_BOX (vbox), scrolledwindow, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), 
            GTK_POLICY_NEVER, 
            GTK_POLICY_ALWAYS);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    treeview = lcrt_connect_create_view(lconnect);
    gtk_widget_show (treeview);
    gtk_container_add (GTK_CONTAINER (scrolledwindow), treeview);

    checkbutton = gtk_check_button_new_with_mnemonic (lconnect->config.value[LCRT_C_SHOW_DIALOG_ON_STARTUP]);
    lconnect->c_checkbutton = checkbutton;
    gtk_widget_show (checkbutton);
    gtk_box_pack_start (GTK_BOX (vbox), checkbutton, FALSE, FALSE, 0);

    dialog_action_area = GTK_DIALOG (dialog_connect)->action_area;
    gtk_widget_show (dialog_action_area);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);

    cancelbutton = gtk_button_new_with_mnemonic(lconnect->config.value[LCRT_C_CLOSE_BUTTON]);
    gtk_widget_show (cancelbutton);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog_connect), cancelbutton, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
    g_signal_connect ((gpointer) cancelbutton, "clicked",
                      G_CALLBACK (lcrt_connect_on_cancelbutton_clicked),
                      lconnect);
    GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
    gtk_widget_grab_focus(cancelbutton);

    connectbutton = gtk_button_new_with_mnemonic(lconnect->config.value[LCRT_C_CONNECT_BUTTON]);
    lconnect->c_connectbutton = connectbutton;
    gtk_widget_show (connectbutton);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog_connect), connectbutton, GTK_RESPONSE_OK);
    gtk_widget_set_sensitive(connectbutton, FALSE);

    g_signal_connect ((gpointer) connectbutton, "clicked",
                      G_CALLBACK (lcrt_connect_on_connectbutton_clicked),
                      lconnect);

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    g_signal_connect((gpointer)selection, "changed", 
                     G_CALLBACK(lcrt_connect_on_selection_changed), 
                     lconnect);

    g_signal_connect ((gpointer) treeview, "button_press_event",
                      G_CALLBACK (lcrt_connect_on_button_press_event),
                      lconnect);
    gtk_widget_show(dialog_connect);

    rv = gtk_dialog_run(GTK_DIALOG(dialog_connect));
    rv += (-100 * (!!lconnect->tab));
    return rv;
err:
    lcrt_message_error(parent->window, lconnect->config.value[LCRT_C_ERR_MSG]);
    return (GTK_RESPONSE_DELETE_EVENT + (-100 * (!!lconnect->tab)));
}

int lcrt_connect_find_folder(struct lcrt_connect *lconnect, GtkTreeIter *folder, const char *folder_name)
{
    GtkTreeModel *model;
    GtkTreeStore *treestore;
    GtkTreeIter root;
    char *value;
    int valid;

    model = gtk_tree_view_get_model(GTK_TREE_VIEW(lconnect->c_treeview));
    gtk_tree_model_get_iter_first(model, &root);
    valid = gtk_tree_model_iter_children(model, folder, &root);

    while (valid) {
        gtk_tree_model_get(model, folder, 0, &value, -1);
        if (strcmp(value, folder_name) == 0) {
            g_free(value);
            return 1;
        }
        g_free(value);
        valid = gtk_tree_model_iter_next(model, folder);
    }
    return 0;
}
static int lcrt_connect_add_user(struct lcrt_connect *lconnect)
{
    struct lcrtc_user *user;
    struct lcrt_user *luser = &lconnect->parent->u_config;
    GtkTreeIter *root, parent, child, grandson;
    GtkTreeStore *treestore;

    root = &lconnect->c_treeiter;
    treestore = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lconnect->c_treeview)));

    list_for_each_entry(user, &luser->child, brother) {
       if (strlen(user->folder) == 0) {
            gtk_tree_store_append(treestore, &parent, root);
            gtk_tree_store_set(treestore, &parent, 0, user->name, -1);
       } else if (lcrt_connect_find_folder(lconnect, &parent, user->folder)) {
            if (!user->is_folder) {
                gtk_tree_store_append(treestore, &child, &parent);
                gtk_tree_store_set(treestore, &child, 0, user->name, -1);
            }
       } else {
            gtk_tree_store_append(treestore, &parent, root);
            gtk_tree_store_set(treestore, &parent, 0, user->folder, 1, PANGO_WEIGHT_BOLD, -1);
            if (!user->is_folder) {
                gtk_tree_store_append(treestore, &child, &parent);
                gtk_tree_store_set(treestore, &child, 0, user->name, -1);
            }
       }
    }
}
int lcrt_connect_del_user(struct lcrt_connect *lconnect, const char *user_name)
{
    GtkTreeStore *treestore;
    GtkTreeModel *model;
    GtkTreeIter  iter;
    GtkTreeSelection *selection;

    if (lconnect == NULL || user_name == NULL)
        return EINVAL;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(lconnect->c_treeview));
    treestore = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW (lconnect->c_treeview)));
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (lconnect->c_treeview));

    if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) 
        return LCRTE_NOT_FOUND;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, user_name, -1);
        gtk_tree_store_remove(treestore, &iter);
        return LCRTE_OK;
    }
    return LCRTE_NOT_FOUND;
}
static int lcrt_connect_compare_func(GtkTreeModel *model, 
                                     GtkTreeIter *a, GtkTreeIter *b, 
                                     gpointer user_data)
{
    char *value_a, *value_b;
    struct lcrt_connect *lconnect = (struct lcrt_connect *)user_data;
    struct lcrt_window *lwindow = lconnect->parent;
    int ret;
    gboolean is_folder_a, is_folder_b;
    struct lcrtc_user *user_a, *user_b;
    
    gtk_tree_model_get(model, a, 0, &value_a, -1);
    gtk_tree_model_get(model, b, 0, &value_b, -1);
    if ((user_a = lcrt_user_find_by_name(&lwindow->u_config, value_a)) == NULL) {
        ret = -1;
        goto out;
    }
    if ((user_b = lcrt_user_find_by_name(&lwindow->u_config, value_b)) == NULL) {
        ret = 1;
        goto out;
    }
    is_folder_a = user_a->is_folder;
    is_folder_b = user_b->is_folder;

    if (is_folder_a && !is_folder_b) {
        ret = -1;
    } else if (!is_folder_a && is_folder_b) {
        ret = 1;
    } else {
        ret = strcmp(value_a, value_b);
    }
out:
    g_free(value_a);
    g_free(value_b);
    return ret;
}
static GtkWidget *lcrt_connect_create_view(struct lcrt_connect *lconnect)
{
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    GtkWidget *view;
    GtkTreeModel *model;
    GtkTreeStore *treestore;
    GtkTreePath *path;

    view = gtk_tree_view_new();
    lconnect->c_treeview = view;
    col = gtk_tree_view_column_new();

    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), FALSE);
    renderer = gtk_cell_renderer_text_new();
    g_object_set (renderer, "ellipsize", PANGO_ELLIPSIZE_END, NULL);
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_set_attributes(col, renderer, "text", 0, "weight", 1, NULL);

    treestore = gtk_tree_store_new(2, G_TYPE_STRING, PANGO_TYPE_WEIGHT);
    gtk_tree_store_append(treestore, &lconnect->c_treeiter, NULL);
    gtk_tree_store_set(treestore, &lconnect->c_treeiter, 0, lconnect->config.value[LCRT_C_SESSION],-1);
    model = GTK_TREE_MODEL(treestore);
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

    lcrt_connect_add_user(lconnect);

    gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(treestore),
                                     0, lcrt_connect_compare_func,
                                     lconnect, NULL);
    gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE (treestore), 0,
                                         GTK_SORT_ASCENDING);
    path = gtk_tree_model_get_path(GTK_TREE_MODEL(treestore), &lconnect->c_treeiter);
    gtk_tree_view_expand_to_path(GTK_TREE_VIEW(view), path);
    //path = gtk_tree_model_get_path(GTK_TREE_MODEL(treestore), &child);
    //gtk_tree_view_expand_row(GTK_TREE_VIEW(view), path, FALSE);


    g_object_unref(model);

    return view;
}


void lcrt_destroy_connect(struct lcrt_connect *lconnect)
{
    if (lconnect)
        free(lconnect);
}
int lcrt_connect_create_toolitem(struct lcrt_connect *lconnect)
{
    GtkWidget *icon;
    int i;
    const char *toolitem_icon[LCRT_C_NUMBER] = {LCRT_C_ICONS};
    typedef void  (*callbacks)(GtkWidget *widget, gpointer data);
    callbacks callback[LCRT_C_NUMBER] = {LCRT_C_CALLBACK};

    assert(lconnect);
    for (i = LCRT_C_QUICK_CONNECT; i < LCRT_C_SESSION; i++) {
        icon = create_pixmap (NULL, toolitem_icon[i]);
        lconnect->toolitem[i] = gtk_toolbar_append_element (
                    GTK_TOOLBAR (lconnect->c_toolbar),
                    GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                    NULL, NULL,
                    lconnect->config.value[i],
                    NULL,
                    icon,
                    GTK_SIGNAL_FUNC (callback[i]),
                    lconnect);
        gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (lconnect->toolitem[i]), TRUE);
        GTK_WIDGET_UNSET_FLAGS (lconnect->toolitem[i], GTK_CAN_FOCUS);
        if (i == LCRT_C_QUICK_CONNECT)
            gtk_toolbar_append_space (GTK_TOOLBAR (lconnect->c_toolbar));
    }
    return LCRTE_OK;
}

static const char *lcrt_connect_get_db_name(struct lcrt_connect *lconnect)
{
    return lconnect->config.db.db_name;
}
static const char *lcrt_connect_get_tb_name(struct lcrt_connect *lconnect)
{
    return lconnect->config.db.db_table;
}

int lcrt_connect_init_config(struct lcrt_connect *lconnect)
{
    char db_name[256];
    int i;
    static char *name[LCRT_C_NUMBER] = {LCRT_C_NAME};
    static char *value[LCRT_C_NUMBER] = {LCRT_C_VALUE};
    static int shortcut[LCRT_C_NUMBER][2] = {LCRT_C_SHORTCUT};

    assert(lconnect);

    memset(&lconnect->config, 0, sizeof(struct lcrtc_connect));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lconnect->config.db, db_name, LCRT_ICONNECT_TABLE);

    for (i = 0; i < LCRT_C_NUMBER; i++) {
        lconnect->config.name[i] = name[i];
        strncpy(lconnect->config.value[i], value[i], sizeof(lconnect->config.value[i]));
        lconnect->config.shortcut[i][0] = shortcut[i][0];
        lconnect->config.shortcut[i][1] = shortcut[i][1];
    }
    lconnect->get_db = lcrt_connect_get_db_name;
    lconnect->get_tb = lcrt_connect_get_tb_name;

    return 0;
}
int lcrt_connect_load_config(struct lcrt_connect *lconnect)
{
    int rv, i;

    assert(lconnect);

    rv = lconnect->config.db.select(&lconnect->config.db, "SELECT * FROM %s", lconnect->get_tb(lconnect));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        //lcrt_connect_create_config(lconnect);
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_C_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lconnect->config.value[i], lconnect->config.db.get_text_col(&lconnect->config.db, 1), 
                sizeof(lconnect->config.value[i]));
        rv = lconnect->config.db.get_row(&lconnect->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s] shortcut = {%-5d, %d}\n",
                basename((char *)lconnect->get_db(lconnect)),
                lconnect->get_tb(lconnect),
                lconnect->config.name[i],
                lconnect->config.value[i],
                lconnect->config.shortcut[i][0],
                lconnect->config.shortcut[i][1]);
    }

    return rv;
}
int lcrt_connect_create_config(struct lcrt_connect *lconnect)
{
    int i;

    assert(lconnect);
    lconnect->config.db.exec(&lconnect->config.db, 
                         "CREATE TABLE %s( \
                          name VARCHAR(64) PRIMARY KEY, \
                          value VARCHAR(255),  \
                          shortcut INTEGER, \
                          shortcut_mask INTEGER)",
                         lconnect->get_tb(lconnect));
    for (i = 0; i < LCRT_C_NUMBER; i++) {
        lconnect->config.db.exec(&lconnect->config.db,
                             "INSERT INTO %s VALUES('%s', '%s', %d, %d)",
                             lconnect->get_tb(lconnect),
                             lconnect->config.name[i], 
                             lconnect->config.value[i],
                             lconnect->config.shortcut[i][0],
                             lconnect->config.shortcut[i][1]);
    }
    lconnect->config.db.close(&lconnect->config.db);
    return 0;
}
