/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Mon 17 Jan 2011 11:34:47 PM CST
 *
 * Description:  
 */

#ifndef __LCRT_CNOTEBOOK_H__
#define __LCRT_CNOTEBOOK_H__
#include <gtk/gtk.h>
void lcrt_notebook_on_switch_page(GtkNotebook *notebook, GtkNotebookPage *page,
                                  guint page_num, gpointer user_data);
int  lcrt_notebook_control_terminal(GtkAccelGroup *accelgroup, 
                                    GObject *arg1, 
                                    guint key, 
                                    GdkModifierType key_mask, 
                                    gpointer user_data);

void lcrt_notebook_on_page_removed(GtkNotebook *notebook, GtkWidget *child,
                                   guint page_num, gpointer user_data);
#endif
