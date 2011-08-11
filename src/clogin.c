/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Niu Tao<niutao0602@gmail.com>
 * Created Time: Sun 23 Jan 2011 02:36:26 AM CST
 *
 *
 * Description:  
 */

#include <stdio.h>
#include <stdlib.h>
#include "iwindow.h"
#include "ilogin.h"
#include "debug.h"
#include "user.h"
#include "message.h"
#include "iterminal.h"
#include "cterminal.h"
#include "clogin.h"

void lcrt_login_on_okbutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_login *llogin = (struct lcrt_login *)user_data;
    struct lcrtc_user *user;

    user = llogin->parent->user;
    lcrtc_user_set_data(
       user,
       NULL,
       NULL,
       LCRT_PROTOCOL_UNKNOWN,
       gtk_entry_get_text(GTK_ENTRY(llogin->l_username)),
       gtk_entry_get_text(GTK_ENTRY(llogin->l_password)),
       NULL,
       -1,
       gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(llogin->l_remember))
       );
    //lcrt_on_terminal_contents_changed(llogin->parent->terminal, llogin->parent);
    lcrt_login_on_cancelbutton_clicked(NULL, llogin);
    debug_where();
    return;

}
void lcrt_login_on_username_changed(GtkEditable *editable, gpointer user_data)
{
    struct lcrt_login *llogin = (struct lcrt_login *)user_data;
    llogin->parent->username_changed = TRUE;
    debug_where();
}


void lcrt_login_on_password_changed(GtkEditable *editable, gpointer user_data)
{

}

void lcrt_login_on_cancelbutton_clicked (GtkButton *button, gpointer user_data)
{
    struct lcrt_login *llogin = (struct lcrt_login *)user_data;
    debug_where();
    llogin->parent->save_passwd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(llogin->l_remember));
    gtk_widget_destroy(llogin->login);
    lcrt_destroy_login(llogin);
}
gboolean lcrt_login_on_window_delete(GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
    lcrt_login_on_cancelbutton_clicked(NULL, user_data);
    return FALSE;
}

