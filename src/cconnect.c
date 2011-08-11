/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Thu 27 Jan 2011 12:24:25 AM CST
 *
 * Description:  
 */
//#define __LCRT_DEBUG__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iterminal.h"
#include "iwindow.h"
#include "cwindow.h"
#include "iconnect.h"
#include "cconnect.h"
#include "iqconnect.h"
#include "debug.h"
#include "user.h"
#include "message.h"
#include "cstatusbar.h"
#include "irename.h"
#include "ifind.h"

void lcrt_connect_on_connectbutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_connect *lconnect = (struct lcrt_connect *)user_data;
    debug_where();

    lconnect->connect_button_clicked = TRUE;
    lcrt_connect_on_button_press_event(NULL, NULL, user_data);
}
void lcrt_connect_on_cancelbutton_clicked (GtkButton *button, gpointer user_data)
{
    struct lcrt_connect *lconnect = (struct lcrt_connect *)user_data;

    lcrt_statusbar_set_message(lconnect->parent->w_statusbar, "");
    debug_where();
    gtk_widget_destroy(lconnect->c_connect);
    lcrt_destroy_connect(lconnect);
}
gboolean lcrt_connect_on_window_delete(GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
    debug_where();
    lcrt_connect_on_cancelbutton_clicked(NULL, user_data);
    return FALSE;
}

void lcrt_connect_on_quick_connect_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_connect *lconnect = (struct lcrt_connect *)user_data;
    int rv;

    debug_where();
    rv = lcrt_create_qconnect(lconnect->parent, GTK_WINDOW(lconnect->c_connect), LCRT_QCONNECT_IN_WINDOW, NULL);
    if (rv == GTK_RESPONSE_OK) {
        if (lconnect->tab == TRUE)
            lcrt_create_terminal(lconnect->parent->w_notebook);
   
        lcrt_connect_on_cancelbutton_clicked(NULL, user_data);
    }
}
void lcrt_connect_on_rename_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_connect *lconnect = (struct lcrt_connect *)user_data;
    struct lcrt_window *lwindow = lconnect->parent;
    struct lcrtc_user *user;
    GValue gvalue = {G_TYPE_STRING};
    int rv;
    debug_where();

    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkTreeView *treeview = GTK_TREE_VIEW (lconnect->c_treeview);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);;
    char *value;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        memset(lwindow->current_uname, 0, HOSTNAME_LEN);
        gtk_tree_model_get(model, &iter, 0, &value, -1);
        lcrt_create_dialog_rename(lwindow, GTK_WINDOW(lconnect->c_connect), value, FALSE);
        if (strcmp(lwindow->current_uname, "") != 0) {
            if ((user = lcrt_user_find_by_name(&lwindow->u_config, value)) != NULL) {
                if (lcrt_window_get_auto_save(lwindow)) {
                    rv = lcrt_user_rename(&lwindow->u_config, user, lwindow->current_uname);
                    if (rv == LCRTE_OK) {
                        g_value_set_string(&gvalue, lwindow->current_uname);
                        gtk_tree_store_set_value(
                                GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW (treeview))),
                                &iter, 0, &gvalue);
                    }
                }
            }
            memset(lwindow->current_uname, 0, HOSTNAME_LEN);
        }
        g_free(value);
    } 
}
void lcrt_connect_on_delete_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_connect *lconnect = (struct lcrt_connect *)user_data;
    struct lcrt_window *lwindow = lconnect->parent;
    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkTreeView *treeview = GTK_TREE_VIEW (lconnect->c_treeview);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);;
    char *value;
    struct lcrtc_user *user;
    debug_where();   
    if (gtk_tree_selection_get_selected(selection, &model, &iter) == FALSE)
        return;

    gtk_tree_model_get(model, &iter, 0, &value, -1);
    if (strcmp(value, lconnect->config.value[LCRT_C_SESSION]) == 0)
        goto out;

    if ((user = lcrt_user_find_by_name(&lwindow->u_config, value)) == NULL)
        goto out;

    gboolean ok = lcrt_message_choose(lwindow->window, GTK_MESSAGE_WARNING, 
                                      lconnect->config.value[LCRT_C_WARN_MSG]);
    if (ok == TRUE) {
        /* delete item from treelist */
        lcrt_connect_del_user(lconnect, value);
        if (lcrt_window_get_auto_save(lwindow)) {
            /*delete user from user.db*/
            lcrt_user_del_one(&lwindow->u_config, user);
        }
        /* free memory */
        lcrtc_user_destroy(user);
    }
out:
    g_free(value);

}
void lcrt_connect_on_find_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_connect *lconnect = (struct lcrt_connect *)user_data;
    GtkTreeIter root, child;
    GtkTreeView *treeview = GTK_TREE_VIEW (lconnect->c_treeview);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    char *value;
    int i, n;

    debug_where();   
    memset(lconnect->parent->current_uname, 0, HOSTNAME_LEN);
    lcrt_create_dialog_find(lconnect);

    if (strlen(lconnect->parent->current_uname) == 0)
        return;

    gtk_tree_model_get_iter_root(model, &root);
    n = gtk_tree_model_iter_n_children(model, &root);
    for (i = 0; i < n; i++) {
        if (gtk_tree_model_iter_nth_child(model, &child, &root, i) == TRUE) {
            gtk_tree_model_get(model, &child, 0, &value, -1);
            if (strcmp(lconnect->parent->current_uname, value) == 0) {
                gtk_tree_selection_select_iter(selection, &child);
                g_free(value);
                break;
            }
            g_free(value);
        }
    }
}

gboolean lcrt_connect_on_button_press_event(GtkWidget *widget,
            GdkEventButton  *event, gpointer user_data)
{
    struct lcrt_connect *lconnect = (struct lcrt_connect *)user_data;
    struct lcrt_window *lwindow = lconnect->parent;
    struct lcrtc_user *user;

    debug_where();
    if (!(lconnect->connect_button_clicked == TRUE || 
        (event->type == GDK_2BUTTON_PRESS && event->button == BUTTON_LEFT)))
        goto out;

    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkTreeView *treeview = GTK_TREE_VIEW (lconnect->c_treeview);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);;
    char *value;

    debug_print("double clicked\n");
    lconnect->connect_button_clicked = FALSE;
        
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &value, -1);
        if (strcmp(value, lconnect->config.value[LCRT_C_SESSION]) == 0) {
            debug_print("first\n");
        } else {
            debug_print("select: %s\n", value);
            if ((user = lcrt_user_find_by_name(&lwindow->u_config, value)) != NULL) {
                lcrt_window_set_current_user(lwindow, user);
                if (lconnect->tab == TRUE)
                    lcrt_create_terminal(lwindow->w_notebook);
                lcrt_connect_on_cancelbutton_clicked(NULL, user_data);
            }
        }
        g_free(value);
    } else {
        debug_print("unselect\n");
    }
out:
    return FALSE;

}
void lcrt_connect_on_selection_changed(GtkWidget *widget, gpointer user_data)
{
    struct lcrt_connect *lconnect = (struct lcrt_connect *)user_data;
    struct lcrt_window *lwindow = lconnect->parent;
    struct lcrtc_user *user;

    debug_where();
    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkTreeView *treeview = GTK_TREE_VIEW (lconnect->c_treeview);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);;
    char *value;

    lconnect->connect_button_clicked = FALSE;
        
    if (!gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_widget_set_sensitive(lconnect->c_connectbutton, FALSE);
        return;
    }

    gtk_tree_model_get(model, &iter, 0, &value, -1);
    if (strcmp(value, lconnect->config.value[LCRT_C_SESSION]) != 0) {
        debug_print("select on change: %s\n", value);
        if ((user = lcrt_user_find_by_name(&lwindow->u_config, value)) != NULL) {
            lcrt_statusbar_set_user(lwindow->w_statusbar, user);
        }
        gtk_widget_set_sensitive(lconnect->c_connectbutton, TRUE);
    } else {
        gtk_widget_set_sensitive(lconnect->c_connectbutton, FALSE);
    }
    g_free(value);
}
void lcrt_connect_on_session_option_activate(GtkWidget *toolitem, gpointer user_data)
{
    struct lcrt_connect *lconnect = (struct lcrt_connect *)user_data;
    struct lcrt_window *lwindow = lconnect->parent;
    struct lcrtc_user *user;
    GValue gvalue = {G_TYPE_STRING};
    int rv;
    debug_where();

    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkTreeView *treeview = GTK_TREE_VIEW (lconnect->c_treeview);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);;
    char *value;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &value, -1);
        lcrt_create_qconnect(lwindow, 
                             NULL, 
                             LCRT_QCONNECT_SESSION_OPTION, 
                             value);
        g_free(value);
    } 

}
