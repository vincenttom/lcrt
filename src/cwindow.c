#include <gtk/gtk.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include "message.h"
#include "debug.h"
#include "iwindow.h"
#include "imenubar.h"
#include "ioptions.h"
#include "iterminal.h"
#include "user.h"
#include "list.h"
#include "inotebook.h"
#include "istatus.h"
#include "isettings.h"

gboolean lcrt_window_get_auto_save(struct lcrt_window *lwindow)
{
    struct lcrt_options *loptions = lwindow->w_menubar->m_options;
    return gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(loptions->o_auto_save_options));
}
gboolean lcrt_window_on_state_event(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    debug_where();

    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    if (gdk_window_get_state(GDK_WINDOW(widget->window)) == GDK_WINDOW_STATE_ICONIFIED 
        && lwindow->hide == FALSE) {
#if 0
        gtk_widget_hide(widget);
        lwindow->hide = TRUE;
#endif
    }

     return FALSE;
}

gboolean lcrt_window_on_delete_event(GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    int active = 0, ret;
    struct lcrt_terminal *lterminal;
    list_for_each_entry(lterminal, &lwindow->w_notebook->child, brother) {
        active += (lterminal->connected == LCRT_TERMINAL_CONNECTED && 
                       lterminal->user->protocol != LCRT_PROTOCOL_SHELL);
    }
    if (active != 0) {
        ret = lcrt_message_choose(lwindow->window,
                        GTK_MESSAGE_WARNING,
                        lwindow->w_status->config.value[LCRT_S_WINDOW_EXIT], active);
        if (ret == FALSE)
            return TRUE;
    }
    lcrt_window_save_config(lwindow);
    lcrt_destroy_window(lwindow);
    gtk_main_quit();
    return FALSE;
}

gboolean lcrt_window_on_configure_event(
        GtkWidget *widget, 
        GdkEventConfigure *event, 
        gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    /* 
     * we should decrease some number to insure we can get 
     * correct position of current window, but I don't know
     * why?
     */
    lwindow->w_config.value[LCRT_W_START_X] = event->x - 1;
    lwindow->w_config.value[LCRT_W_START_Y] = event->y - 23;
    return FALSE;
}
gboolean lcrt_window_on_key_press_event(GtkWidget*widget,
            GdkEventKey *event, gpointer user_data)
{
    struct lcrt_window *lwindow = (struct lcrt_window *)user_data;
    struct lcrt_settings *lsettings = lwindow->w_settings;

    if (event->type == GDK_KEY_PRESS && 
        event->keyval == GDK_F10 && 
        lsettings->enable_f10_key == FALSE)
        return TRUE;
    return FALSE;
}
void lcrt_window_create_new(struct lcrt_window *lwindow)
{
    typedef int (*exec_func_t)(const char *path, const char *arg, ...);
    exec_func_t exec_func;
    char *programe_path;
    if (fork() == 0) {
        if (strstr(lcrt_config_get_language(), LCRT_INSTALL_DIR) != NULL) {
            exec_func = execlp;
            programe_path = basename(lwindow->programe_name);
        } else {
            exec_func = execl;
            programe_path = lwindow->programe_name;
        }

        exec_func(programe_path,
              basename(lwindow->programe_name),
              "-c",
              lwindow->current_user->name,
              NULL);
    }
}
