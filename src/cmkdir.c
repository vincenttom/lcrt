/**
 * @file cmkdir.c
 * <h2>License</h2>
 * Copyright (c) 2012 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * @author Niu Tao <niutao0602@gmail.com>
 * @version $Id
 * @date Mon 30 Apr 2012 05:23:43 PM CST
 *
 * @brief  
 */

//#define __LCRT_DEBUG__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "iconnect.h"
#include "iwindow.h"
#include "debug.h"
#include "message.h"
#include "iterminal.h"
#include "imkdir.h"
#include "cmkdir.h"

void lcrt_mkdir_on_okbutton_clicked(GtkButton *button,  gpointer user_data)
{
    debug_where();
    struct lcrt_mkdir *lmkdir = (struct lcrt_mkdir *)user_data;
    struct lcrt_connect *lconnect = lmkdir->parent;
    struct lcrt_window *lwindow = lconnect->parent;
    const char *name = gtk_entry_get_text(GTK_ENTRY(lmkdir->entry_mkdir));
    GtkTreeIter same;
    struct lcrtc_user *user;
    struct lcrt_user *luser = &lconnect->parent->u_config;
    GtkTreeIter *root, parent, child, grandson;
    GtkTreeStore *treestore;
    debug_where();

    if (lcrt_connect_find_folder(lconnect, &same, name)) {
        lcrt_message_info(lconnect->c_connect, lmkdir->config.value[LCRT_M_ERROR]);
        return;
    }
    debug_where();

    root = &lconnect->c_treeiter;
    treestore = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lconnect->c_treeview)));
    if ((user = lcrtc_user_create()) == NULL) {
        /*  
         * FIXME: There is no more memory, how can 
         * we handle this exception ?
         */
        return;
    }
    debug_where();
    lcrtc_user_set_data(
       user,
       name,
       NULL,
       LCRT_PROTOCOL_UNKNOWN,
       NULL,
       NULL,
       NULL,
       -1,
       TRUE,
       name,
       1
    );
    debug_where();
    lcrtc_user_ref(user);
    lcrt_user_add(&lwindow->u_config, user);

    if (lcrt_window_get_auto_save(lwindow)) {
        lcrt_user_save_one(&lwindow->u_config, user);
    }

    debug_where();
    gtk_tree_store_append(treestore, &parent, root);
    gtk_tree_store_set(treestore, &parent, 0, user->folder, 1, PANGO_WEIGHT_BOLD, -1);
    debug_where();

    lcrt_mkdir_on_cancelbutton_clicked(NULL, user_data);
    debug_where();
}

void lcrt_mkdir_on_cancelbutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_mkdir *lmkdir = (struct lcrt_mkdir *)user_data;
    gtk_widget_destroy(lmkdir->dialog);
    lcrt_destroy_mkdir(lmkdir);

}

void lcrt_mkdir_on_name_changed(GtkEditable *editable, gpointer user_data)
{
    struct lcrt_mkdir *lmkdir = (struct lcrt_mkdir *)user_data;
    if (gtk_entry_get_text_length(GTK_ENTRY(editable)) > 0)
        gtk_widget_set_sensitive(lmkdir->okbutton, TRUE);
    else
        gtk_widget_set_sensitive(lmkdir->okbutton, FALSE);

}
gboolean lcrt_mkdir_on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer         user_data)
{
    lcrt_mkdir_on_cancelbutton_clicked(NULL, user_data);

    return FALSE;
}


