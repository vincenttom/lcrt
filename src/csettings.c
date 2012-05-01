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

//#define __LCRT_DEBUG__

#include <gtk/gtk.h>
#include <string.h>
#include "isettings.h"
#include "csettings.h"
#include "debug.h"
#include "language.h"
#include "mkconfig.h"
#include "iwindow.h"
#include "imenubar.h"
#include "iview.h"
#include "itoolbar.h"
#include "istatusbar.h"
#include "inotebook.h"
#include "iterminal.h"

void lcrt_settings_on_applybutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_settings *lsettings = (struct lcrt_settings *)user_data;
    int index = gtk_combo_box_get_active(GTK_COMBO_BOX(lsettings->g_cb_language));
    const struct lcrt_support_language *languages;
    typedef void (*func_t)(GtkWidget *widget);
    func_t gtk_widget_show_func;
    gboolean show_toolbar, show_statusbar;
    struct lcrt_terminal *lterminal;
    int scrolllines;

    debug_where();

    if (lsettings->session) {
        lterminal = lsettings->parent->w_notebook->current_terminal;
        if (lterminal)
            vte_terminal_set_audible_bell(VTE_TERMINAL(lterminal->terminal), 
                    gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lsettings->t_cb_bell)));
        return;
    }
    /* genral settings, language support */
    index = gtk_combo_box_get_active(GTK_COMBO_BOX(lsettings->g_cb_language));
    languages = lcrt_get_languages();
    if (strstr(lcrt_config_get_language(), languages[index].db_name) == NULL) {
        debug_print("SELECT LANGUAGE = %s\n", languages[index].db_name);
        lcrt_config_save_language(languages[index].db_name);
    }
    /*genral settings, dialog*/
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lsettings->g_rb_connect)))
        lsettings->lt_g_show_dialog = LCRT_LT_SHOW_DIALOG_CONNECT;
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lsettings->g_rb_quick_connect)))
        lsettings->lt_g_show_dialog = LCRT_LT_SHOW_DIALOG_QUICK_CONNECT;
    else
        lsettings->lt_g_show_dialog = LCRT_LT_SHOW_DIALOG_NONE;

    /*general settings, view*/
    show_toolbar = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lsettings->g_cb_show_toolbar));
    show_statusbar = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lsettings->g_cb_show_statusbar));
    lsettings->lt_t_bell =  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lsettings->t_cb_bell));
    if (show_toolbar)
        gtk_widget_show_func = &gtk_widget_show;
    else
        gtk_widget_show_func = &gtk_widget_hide;

    gtk_widget_show_func(lsettings->parent->w_toolbar->toolbar);
    lsettings->lt_g_show_toolbar = show_toolbar;

    if (show_statusbar)
        gtk_widget_show_func = &gtk_widget_show;
    else
        gtk_widget_show_func = &gtk_widget_hide;

    gtk_widget_show_func(lsettings->parent->w_statusbar->statusbar);
    lsettings->lt_g_show_statusbar = show_statusbar;
    /* Keybindings, Keyboard sortcuts */
    lsettings->enable_f10_key = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lsettings->k_cb_enable_f10));
    /* Terminal, Terminal bell */
    list_for_each_entry(lterminal, &lsettings->parent->w_notebook->child, brother) {
        vte_terminal_set_audible_bell(VTE_TERMINAL(lterminal->terminal), lsettings->lt_t_bell);
    }

    scrolllines = gtk_adjustment_get_value(GTK_ADJUSTMENT(lsettings->t_sb_scrollback_lines_adj));
    
    debug_print("scrolllines = %d\n", scrolllines);
    if (scrolllines > 0)
        lsettings->lt_t_scrolllines = scrolllines;

}

void lcrt_settings_on_cancelbutton_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_settings *lsettings = (struct lcrt_settings *)user_data;
    lsettings->session = FALSE;
    gtk_font_button_set_font_name(GTK_FONT_BUTTON(lsettings->t_bt_font), lsettings->lt_t_font);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(lsettings->t_bt_foreground_color), &lsettings->lt_t_fcolor);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(lsettings->t_bt_background_color), &lsettings->lt_t_bcolor);
    gtk_adjustment_set_value(GTK_ADJUSTMENT(lsettings->t_sb_scrollback_lines_adj), lsettings->lt_t_scrolllines);
    gtk_entry_set_text(GTK_ENTRY(lsettings->t_et_background_image), lsettings->lt_t_backimage);
    gtk_adjustment_set_value(GTK_ADJUSTMENT(lsettings->t_hl_transparent), lsettings->lt_t_transparent);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lsettings->t_cb_bell), lsettings->lt_t_bell);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lsettings->k_cb_enable_f10), lsettings->enable_f10_key);

    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(lsettings->notebook), TRUE);
    gtk_widget_hide(lsettings->dialog_settings);
}

void lcrt_settings_on_okbutton_clicked(GtkButton *button, gpointer user_data)
{
    lcrt_settings_on_applybutton_clicked(NULL, user_data);
    lcrt_settings_on_cancelbutton_clicked(NULL, user_data);
}
gboolean lcrt_settings_on_window_delete(GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
    debug_where();
    lcrt_settings_on_cancelbutton_clicked(NULL, user_data);
    return TRUE;
}

void lcrt_settings_on_button_background_image_clicked(GtkButton *button, gpointer user_data)
{
    struct lcrt_settings *lsettings = (struct lcrt_settings *)user_data;
    int ret;
    char image_path[256];
    struct lcrt_terminal *lterminal;

    ret = lcrt_file_choose_dialog(lsettings->parent->window, "Open File", image_path);
    if (ret != LCRTE_OK)
        return;

    gtk_entry_set_text(GTK_ENTRY(lsettings->t_et_background_image), image_path);
    if (lsettings->session) {
        lterminal = lsettings->parent->w_notebook->current_terminal;
        if (lterminal)
            vte_terminal_set_background_image_file(VTE_TERMINAL(lterminal->terminal), image_path);
        return;
    }

    strcpy(lsettings->lt_t_backimage, image_path);
    list_for_each_entry(lterminal, &lsettings->parent->w_notebook->child, brother) {
        vte_terminal_set_background_image_file(VTE_TERMINAL(lterminal->terminal), lsettings->lt_t_backimage);
    }

}
void lcrt_settings_terminal_font_button_clicked(GtkFontButton *widget, gpointer user_data)
{
    struct lcrt_settings *lsettings = (struct lcrt_settings *)user_data;
    struct lcrt_terminal *lterminal;
    const gchar *font = gtk_font_button_get_font_name(widget);

    if (lsettings->session) {
        lterminal = lsettings->parent->w_notebook->current_terminal;
        if (lterminal)
            vte_terminal_set_font_from_string(lterminal->terminal, font);
        return;
    }
    debug_where();
    strcpy(lsettings->lt_t_font, font);

    list_for_each_entry(lterminal, &lsettings->parent->w_notebook->child, brother) {
        vte_terminal_set_font_from_string(lterminal->terminal, font);
    }

}
void lcrt_settings_terminal_color_button_clicked(GtkColorButton *widget, gpointer user_data)
{
    struct lcrt_settings *lsettings = (struct lcrt_settings *)user_data;
    struct lcrt_terminal *lterminal;
    GdkColor *color, col;
    typedef void (*vte_set_color_t)(VteTerminal *terminal, const GdkColor *background);
    vte_set_color_t lcrt_terminal_set_color;

    if (lsettings->session) {
        lterminal = lsettings->parent->w_notebook->current_terminal;
        if (lterminal == NULL)
            return;
        gtk_color_button_get_color(widget, &col);
        if (GTK_WIDGET(widget) == lsettings->t_bt_foreground_color)
            vte_terminal_set_color_foreground(lterminal->terminal, &col);
        else
            vte_terminal_set_color_background(lterminal->terminal, &col);
        return;
    }

    if (GTK_WIDGET(widget) == lsettings->t_bt_foreground_color) {
        color = &lsettings->lt_t_fcolor;
        lcrt_terminal_set_color = &vte_terminal_set_color_foreground;
    } else {
        color = &lsettings->lt_t_bcolor;
        lcrt_terminal_set_color = &vte_terminal_set_color_background;
    }

    gtk_color_button_get_color(widget, color);

    list_for_each_entry(lterminal, &lsettings->parent->w_notebook->child, brother) {
        lcrt_terminal_set_color(lterminal->terminal, color);
    }
}
void lcrt_settings_terminal_on_restore_default_clicked(GtkButton *widget, gpointer user_data)
{
    struct lcrt_settings *lsettings = (struct lcrt_settings *)user_data;
    struct lcrt_terminal *lterminal;

    lsettings->config.ldb.close(&lsettings->config.ldb);
    lcrt_settings_init_local_config(lsettings);
    lcrt_settings_convert_local_config(lsettings);

    gtk_font_button_set_font_name(GTK_FONT_BUTTON(lsettings->t_bt_font), lsettings->lt_t_font);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(lsettings->t_bt_foreground_color), &lsettings->lt_t_fcolor);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(lsettings->t_bt_background_color), &lsettings->lt_t_bcolor);
    gtk_adjustment_set_value(GTK_ADJUSTMENT(lsettings->t_sb_scrollback_lines_adj), lsettings->lt_t_scrolllines);
    gtk_entry_set_text(GTK_ENTRY(lsettings->t_et_background_image), lsettings->lt_t_backimage);
    gtk_adjustment_set_value(GTK_ADJUSTMENT(lsettings->t_hl_transparent), lsettings->lt_t_transparent);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lsettings->t_cb_bell), lsettings->lt_t_bell);

    list_for_each_entry(lterminal, &lsettings->parent->w_notebook->child, brother) {
        vte_terminal_set_font_from_string(lterminal->terminal, lsettings->lt_t_font);
        vte_terminal_set_color_foreground(VTE_TERMINAL(lterminal->terminal), &lsettings->lt_t_fcolor);
        vte_terminal_set_color_background(VTE_TERMINAL(lterminal->terminal), &lsettings->lt_t_bcolor);
        vte_terminal_set_background_transparent(lterminal->terminal, FALSE);
        vte_terminal_set_background_saturation(VTE_TERMINAL(lterminal->terminal), (double)lsettings->lt_t_transparent / 100.0);
        vte_terminal_set_scrollback_lines(VTE_TERMINAL(lterminal->terminal), lsettings->lt_t_scrolllines);
        vte_terminal_set_background_image_file(VTE_TERMINAL(lterminal->terminal), lsettings->lt_t_backimage);
        vte_terminal_set_audible_bell(VTE_TERMINAL(lterminal->terminal), lsettings->lt_t_bell);
    }
}
void lcrt_settings_terminal_on_transparent_value_changed(GtkButton *widget, gpointer user_data)
{
    struct lcrt_settings *lsettings = (struct lcrt_settings *)user_data;
    struct lcrt_terminal *lterminal;
    int transparent;

    transparent = gtk_adjustment_get_value(GTK_ADJUSTMENT(lsettings->t_hl_transparent));
    if (lsettings->session) {
        lterminal = lsettings->parent->w_notebook->current_terminal;
        if (lterminal)
            vte_terminal_set_background_saturation(VTE_TERMINAL(lterminal->terminal), 
                (double)transparent / 100.0);
        return;
    }
    lsettings->lt_t_transparent = transparent;
    list_for_each_entry(lterminal, &lsettings->parent->w_notebook->child, brother) {
        if (lsettings->lt_t_transparent != 0)
            vte_terminal_set_background_transparent(VTE_TERMINAL(lterminal->terminal), TRUE);
        else
            vte_terminal_set_background_transparent(VTE_TERMINAL(lterminal->terminal), FALSE);
        vte_terminal_set_background_saturation(VTE_TERMINAL(lterminal->terminal), 
                (double)lsettings->lt_t_transparent / 100.0);
    }
}
int lcrt_file_choose_dialog(GtkWidget *parent, const char *prompt, char *file)
{
    GtkWidget *dialog;
    int ret;
    dialog = gtk_file_chooser_dialog_new("Open File",
                              GTK_WINDOW(parent),
                              GTK_FILE_CHOOSER_ACTION_OPEN,
                              GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                              GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                              NULL);
    GtkFileFilter *filter2 = gtk_file_filter_new();
    gtk_file_filter_set_name(filter2, "Image File(*.png,*.jpg,*.jpeg,*.gif,*.xpm)");
    gtk_file_filter_add_pattern(filter2,"*.png");
    gtk_file_filter_add_pattern(filter2,"*.jpg");
    gtk_file_filter_add_pattern(filter2,"*.jpeg");
    gtk_file_filter_add_pattern(filter2,"*.gif");
    gtk_file_filter_add_pattern(filter2,"*.xpm");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter2);
    GtkFileFilter *filter1 = gtk_file_filter_new();
    gtk_file_filter_set_name(filter1, "All File(*)");
    gtk_file_filter_add_pattern(filter1,"*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter1);
    //gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);
    ret = gtk_dialog_run (GTK_DIALOG (dialog));
    if (ret == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        strcpy(file, filename);
        g_free (filename);
        gtk_widget_destroy (dialog);
        return LCRTE_OK;
    }
    gtk_widget_destroy (dialog);
    return LCRTE_NOT_FOUND;
}

