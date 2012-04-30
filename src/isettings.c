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
#include "iterminal.h"
#include "isettings.h"
#include "csettings.h"
#include "language.h"
#include "imenubar.h"
#include "iview.h"
#include "itoolbar.h"
#include "istatusbar.h"
#include "foperate.h"

int lcrt_settings_create_local_config(struct lcrt_settings *lsettings);
int lcrt_settings_load_config(struct lcrt_settings *lsettings);
int lcrt_settings_init_config(struct lcrt_settings *lsettings);
int lcrt_settings_convert_local_config(struct lcrt_settings *lsettings);
int lcrt_settings_save_local_config(struct lcrt_settings *lsettings);

struct lcrt_settings *lcrt_create_settings(struct lcrt_window *parent)
{
    GtkWidget *dialog_settings;
    GtkWidget *dialog_vbox;
    GtkWidget *notebook;
    GtkWidget *vbox_general;
    GtkWidget *frame_language;
    GtkWidget *alignment_language;
    GtkWidget *vbox2;
    GtkWidget *g_lb_language_support_note;
    GtkWidget *hbox1;
    GtkWidget *g_lb_current_language;
    GtkWidget *g_cb_language;
    GtkWidget *g_lb_language_support;
    GtkWidget *frame_dialog;
    GtkWidget *alignment_dialog;
    GtkWidget *vbox_dialog;
    GtkWidget *g_rb_connect;
    GSList *g_rb_connect_group = NULL;
    GtkWidget *g_rb_quick_connect;
    GtkWidget *g_rb_none;
    GtkWidget *g_lb_dialog;
    GtkWidget *frame_settings;
    GtkWidget *alignment_settings;
    GtkWidget *vbox_settings;
    GtkWidget *g_cb_show_toolbar;
    GtkWidget *g_cb_show_statusbar;
    GtkWidget *g_lb_settings;
    GtkWidget *g_nb_general;
    GtkWidget *frame_keybindings;
    GtkWidget *alignment_kebindings;
    GtkWidget *vbox8;
    GtkWidget *k_lb_keyboard_shortcuts_note;
    GtkWidget *scrolledwindow2;
    GtkWidget *k_tv_shortcurts;
    GtkWidget *k_lb_keyboard_shortcut;
    GtkWidget *k_cb_enable_f10;
    GtkWidget *k_nb_keybindings;
    GtkWidget *frame_terminal;
    GtkWidget *alignment_terminal;
    GtkWidget *vbox_terminal;
    GtkWidget *t_lb_terminal_note;
    GtkWidget *table_terminal;
    GtkWidget *t_lb_font;
    GtkWidget *t_lb_foreground_color;
    GtkWidget *t_bt_foreground_color;
    GtkWidget *t_bt_font;
    GtkWidget *t_lb_background_color;
    GtkWidget *t_bt_background_color;
    GtkWidget *t_lb_background_image;
    GtkWidget *t_lb_transparent;
    GtkObject *t_sb_scrollback_lines_adj;
    GtkWidget *t_sb_scrollback_lines;
    GtkWidget *hbox4;
    GtkWidget *t_et_background_image;
    GtkWidget *t_bt_background_image;
    GtkWidget *image3;
    GtkWidget *t_hl_transparent;
    GtkTooltips *tooltips; 
    GtkWidget *t_lb_scrollback_lines;
    GtkWidget *hbox8;
    GtkWidget *t_cb_bell;
    GtkWidget *fixed1;
    GtkWidget *t_bt_restore_default;

    GtkWidget *t_lb_terminal_settings;
    GtkWidget *t_nb_terminal;
    GtkWidget *frame_application;
    GtkWidget *alignment6;
    GtkWidget *vbox7;
    GtkWidget *a_lb_application_note;
    GtkWidget *table2;
    GtkWidget *a_lb_shell;
    GtkWidget *a_lb_ssh;
    GtkWidget *a_lb_telnet;
    GtkWidget *a_et_ssh;
    GtkWidget *a_et_telnet;
    GtkWidget *a_bt_telnet;
    GtkWidget *a_bt_shell;
    GtkWidget *a_et_shell;
    GtkWidget *a_bt_ssh;
    GtkWidget *a_lb_application;
    GtkWidget *a_nb_application;
    GtkWidget *dialog_action_area;
    GtkWidget *applybutton;
    GtkWidget *alignment7;
    GtkWidget *hbox5;
    GtkWidget *image4;
    GtkWidget *lb_applybutton;
    GtkWidget *cancelbutton;
    GtkWidget *alignment8;
    GtkWidget *hbox6;
    GtkWidget *image5;
    GtkWidget *lb_cancelbutton;
    GtkWidget *okbutton;
    GtkWidget *alignment9;
    GtkWidget *hbox7;
    GtkWidget *image6;
    GtkWidget *lb_okbutton;

    struct lcrt_settings *lsettings;
    lsettings =(struct lcrt_settings *)calloc(1, sizeof(struct lcrt_settings));
    if (lsettings == NULL)
        goto err;

    lcrt_settings_init_config(lsettings);
    lcrt_settings_load_config(lsettings);
    lcrt_settings_convert_local_config(lsettings);

    lsettings->parent = parent;

    dialog_settings = gtk_dialog_new();
    lsettings->dialog_settings = dialog_settings;
    gtk_widget_set_size_request(dialog_settings, 500, 400);

    gtk_window_set_transient_for(GTK_WINDOW(dialog_settings), GTK_WINDOW(parent->window));
    gtk_window_set_title(GTK_WINDOW (dialog_settings), 
                         lsettings->config.value[LCRT_T_DIALOG_SETTINGS]);
    gtk_window_set_position(GTK_WINDOW (dialog_settings), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_window_set_resizable(GTK_WINDOW (dialog_settings), FALSE);
    gtk_window_set_type_hint(GTK_WINDOW (dialog_settings), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_dialog_set_has_separator(GTK_DIALOG (dialog_settings), FALSE);

    dialog_vbox = GTK_DIALOG(dialog_settings)->vbox;
    gtk_widget_show(dialog_vbox);

    notebook = gtk_notebook_new();
    lsettings->notebook = notebook;
    gtk_widget_show(notebook);
    gtk_box_pack_start(GTK_BOX(dialog_vbox), notebook, TRUE, TRUE, 0);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_LEFT);
    
    vbox_general = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox_general);
    gtk_container_add(GTK_CONTAINER(notebook), vbox_general);

    frame_language = gtk_frame_new(NULL);
    gtk_widget_show(frame_language);
    gtk_box_pack_start(GTK_BOX(vbox_general), frame_language, FALSE, FALSE, 0);
    gtk_widget_set_size_request(frame_language, -1, 100);
    gtk_frame_set_shadow_type(GTK_FRAME(frame_language), GTK_SHADOW_IN);
    
    g_lb_language_support = gtk_label_new(lsettings->config.value[LCRT_T_G_LB_LANGUAGE_SUPPORT]);
    gtk_widget_show(g_lb_language_support);
    gtk_frame_set_label_widget(GTK_FRAME(frame_language), g_lb_language_support);
    gtk_label_set_use_markup(GTK_LABEL(g_lb_language_support), TRUE);

    alignment_language = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment_language);
    gtk_container_add(GTK_CONTAINER(frame_language), alignment_language);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment_language), 0, 0, 12, 0);

    vbox2 = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox2);
    gtk_container_add(GTK_CONTAINER(alignment_language), vbox2);
    gtk_widget_set_size_request(vbox2, -1, 60);

    g_lb_language_support_note = gtk_label_new(lsettings->config.value[LCRT_T_G_LB_LANGUAGE_SUPPORT_NOTE]);
    gtk_widget_show(g_lb_language_support_note);
    gtk_widget_set_size_request (g_lb_language_support_note, -1, 40);
    gtk_box_pack_start(GTK_BOX(vbox2), g_lb_language_support_note, FALSE, FALSE, 0);
    gtk_misc_set_alignment(GTK_MISC(g_lb_language_support_note), 0.01, 0.5);

    hbox1 = gtk_hbox_new(FALSE, 0);
    gtk_widget_show(hbox1);
    gtk_box_pack_start(GTK_BOX(vbox2), hbox1, FALSE, TRUE, 0);

    g_lb_current_language = gtk_label_new(lsettings->config.value[LCRT_T_G_LB_CURRENT_LANGUAGE]);
    gtk_widget_show(g_lb_current_language);
    gtk_box_pack_start(GTK_BOX(hbox1), g_lb_current_language, FALSE, FALSE, 0);
    gtk_widget_set_size_request(g_lb_current_language, 130, 30);
    gtk_misc_set_alignment(GTK_MISC(g_lb_current_language), 0.01, 0.5);
    
    g_cb_language = gtk_combo_box_new_text ();
    lsettings->g_cb_language = g_cb_language;
    gtk_widget_show(g_cb_language);
    gtk_box_pack_start(GTK_BOX(hbox1), g_cb_language, FALSE, FALSE, 0);
    gtk_widget_set_size_request(g_cb_language, 180, 30);

    const struct lcrt_support_language *languages = lcrt_get_languages();
    int i, current_language;
    for (i = 0; i < LCRT_SUPPORT_LANGUAGE; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX(g_cb_language), languages[i].language_name);
        if (strstr(lcrt_config_get_language(), languages[i].db_name) != NULL)
            current_language = i;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX (g_cb_language), current_language);
    //gtk_entry_set_editable(GTK_ENTRY(GTK_BIN(g_cb_language)->child), FALSE);


    g_nb_general = gtk_label_new(lsettings->config.value[LCRT_T_G_NB_GENERAL]);
    lsettings->g_nb_general = g_nb_general;
    gtk_widget_show(g_nb_general);
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), 
            gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), 0), g_nb_general);
    gtk_widget_set_size_request(g_nb_general, 80, -1);
    gtk_misc_set_alignment(GTK_MISC(g_nb_general), 0.01, 0.5);
    GTK_WIDGET_UNSET_FLAGS(g_nb_general, GTK_CAN_FOCUS);

    frame_dialog = gtk_frame_new(NULL);
    gtk_widget_show(frame_dialog);
    gtk_box_pack_start(GTK_BOX(vbox_general), frame_dialog, TRUE, FALSE, 0);
    gtk_frame_set_shadow_type(GTK_FRAME(frame_dialog), GTK_SHADOW_IN);

    alignment_dialog = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment_dialog);
    gtk_container_add(GTK_CONTAINER(frame_dialog), alignment_dialog);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment_dialog), 0, 0, 12, 0);

    vbox_dialog = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox_dialog);
    gtk_container_add(GTK_CONTAINER(alignment_dialog), vbox_dialog);

    g_rb_connect = gtk_radio_button_new_with_mnemonic(NULL, lsettings->config.value[LCRT_T_G_RB_CONNECT]);
    lsettings->g_rb_connect = g_rb_connect;
    gtk_widget_show(g_rb_connect);
    gtk_box_pack_start(GTK_BOX(vbox_dialog), g_rb_connect, FALSE, FALSE, 0);
    gtk_widget_set_size_request(g_rb_connect, -1, 30);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(g_rb_connect), g_rb_connect_group);
    g_rb_connect_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(g_rb_connect));

    g_rb_quick_connect = gtk_radio_button_new_with_mnemonic(NULL, lsettings->config.value[LCRT_T_G_RB_QUICK_CONNECT]);
    lsettings->g_rb_quick_connect = g_rb_quick_connect;
    gtk_widget_show(g_rb_quick_connect);
    gtk_box_pack_start(GTK_BOX(vbox_dialog), g_rb_quick_connect, FALSE, FALSE, 0);
    gtk_widget_set_size_request(g_rb_quick_connect, -1, 30);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(g_rb_quick_connect), g_rb_connect_group);
    g_rb_connect_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(g_rb_quick_connect));

    g_rb_none = gtk_radio_button_new_with_mnemonic(NULL, lsettings->config.value[LCRT_T_G_RB_NONE]);
    lsettings->g_rb_none = g_rb_none;
    gtk_widget_show(g_rb_none);
    gtk_box_pack_start(GTK_BOX(vbox_dialog), g_rb_none, FALSE, FALSE, 0);
    gtk_widget_set_size_request(g_rb_none, -1, 30);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(g_rb_none), g_rb_connect_group);
    g_rb_connect_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(g_rb_none));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_rb_none),TRUE);

    g_lb_dialog = gtk_label_new(lsettings->config.value[LCRT_T_G_LB_DIALOG]);
    gtk_widget_show(g_lb_dialog);
    gtk_frame_set_label_widget(GTK_FRAME(frame_dialog), g_lb_dialog);
    gtk_label_set_use_markup(GTK_LABEL(g_lb_dialog), TRUE);

    frame_settings = gtk_frame_new(NULL);
    gtk_widget_show(frame_settings);
    gtk_box_pack_start(GTK_BOX(vbox_general), frame_settings, TRUE, TRUE, 0);
    gtk_frame_set_shadow_type(GTK_FRAME(frame_settings), GTK_SHADOW_IN);

    alignment_settings = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment_settings);
    gtk_container_add(GTK_CONTAINER(frame_settings), alignment_settings);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment_settings), 0, 0, 12, 0);

    vbox_settings = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox_settings);
    gtk_container_add(GTK_CONTAINER(alignment_settings), vbox_settings);

    g_cb_show_toolbar = gtk_check_button_new_with_mnemonic(lsettings->config.value[LCRT_T_G_CB_SHOW_TOOLBAR]);
    lsettings->g_cb_show_toolbar = g_cb_show_toolbar;
    gtk_widget_show(g_cb_show_toolbar);
    gtk_box_pack_start(GTK_BOX(vbox_settings), g_cb_show_toolbar, FALSE, FALSE, 0);
    gtk_widget_set_size_request(g_cb_show_toolbar, -1, 30);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_cb_show_toolbar), 
            lsettings->lt_g_show_toolbar);

    g_cb_show_statusbar = gtk_check_button_new_with_mnemonic(lsettings->config.value[LCRT_T_G_CB_SHOW_STATUSBAR]);
    lsettings->g_cb_show_statusbar = g_cb_show_statusbar;
    gtk_widget_show(g_cb_show_statusbar);
    gtk_box_pack_start(GTK_BOX(vbox_settings), g_cb_show_statusbar, FALSE, FALSE, 0);
    gtk_widget_set_size_request(g_cb_show_statusbar, -1, 30);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(g_cb_show_statusbar), 
            lsettings->lt_g_show_statusbar);

    g_lb_settings = gtk_label_new(lsettings->config.value[LCRT_T_G_LB_VIEW]);
    gtk_widget_show(g_lb_settings);
    gtk_frame_set_label_widget(GTK_FRAME(frame_settings), g_lb_settings);
    gtk_label_set_use_markup(GTK_LABEL(g_lb_settings), TRUE);

    frame_keybindings = gtk_frame_new(NULL);
    gtk_widget_show(frame_keybindings);
    gtk_container_add(GTK_CONTAINER(notebook), frame_keybindings);
    gtk_frame_set_shadow_type(GTK_FRAME(frame_keybindings), GTK_SHADOW_OUT);

    alignment_kebindings = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment_kebindings);
    gtk_container_add(GTK_CONTAINER(frame_keybindings), alignment_kebindings);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment_kebindings), 0, 0, 12, 0);

    vbox8 = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox8);
    gtk_container_add(GTK_CONTAINER(alignment_kebindings), vbox8);

    k_lb_keyboard_shortcuts_note = gtk_label_new(lsettings->config.value[LCRT_T_K_LB_KEYBOARD_SHORTCUTS_NOTE]);
    gtk_widget_show(k_lb_keyboard_shortcuts_note);
    gtk_box_pack_start(GTK_BOX(vbox8), k_lb_keyboard_shortcuts_note, FALSE, FALSE, 0);
    gtk_widget_set_size_request(k_lb_keyboard_shortcuts_note, -1, 50);
    gtk_misc_set_alignment(GTK_MISC(k_lb_keyboard_shortcuts_note), 0.01, 0.5);

    k_cb_enable_f10 = gtk_check_button_new_with_mnemonic(lsettings->config.value[LCRT_T_K_CB_ENABLE_F10]);
    lsettings->k_cb_enable_f10 = k_cb_enable_f10;
    gtk_widget_show(k_cb_enable_f10);
    gtk_box_pack_start(GTK_BOX(vbox8), k_cb_enable_f10, FALSE, FALSE, 0);
    //gtk_widget_set_size_request(k_cb_enable_f10, -1, 30);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(k_cb_enable_f10), 
            lsettings->enable_f10_key);


    scrolledwindow2 = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_show(scrolledwindow2);
    gtk_box_pack_start(GTK_BOX(vbox8), scrolledwindow2, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow2), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow2), GTK_SHADOW_IN);

    k_tv_shortcurts = gtk_tree_view_new();
    lsettings->k_tv_shortcurts = k_tv_shortcurts;
    gtk_widget_show(k_tv_shortcurts);
    gtk_container_add(GTK_CONTAINER(scrolledwindow2), k_tv_shortcurts);

    k_lb_keyboard_shortcut = gtk_label_new(lsettings->config.value[LCRT_T_K_LB_KEYBOARD_SHORTCUT]);
    gtk_widget_show(k_lb_keyboard_shortcut);
    gtk_frame_set_label_widget(GTK_FRAME(frame_keybindings), k_lb_keyboard_shortcut);
    gtk_label_set_use_markup(GTK_LABEL(k_lb_keyboard_shortcut), TRUE);

    k_nb_keybindings = gtk_label_new(lsettings->config.value[LCRT_T_K_NB_KEYBINDINGS]);
    lsettings->k_nb_keybindings = k_nb_keybindings;
    gtk_widget_show(k_nb_keybindings);
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), 1), k_nb_keybindings);
    gtk_misc_set_alignment(GTK_MISC(k_nb_keybindings), 0.01, 0.5);

    frame_terminal = gtk_frame_new(NULL);
    gtk_widget_show(frame_terminal);
    gtk_container_add(GTK_CONTAINER(notebook), frame_terminal);
    gtk_container_set_border_width(GTK_CONTAINER(frame_terminal), 5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame_terminal), GTK_SHADOW_OUT);

    alignment_terminal = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment_terminal);
    gtk_container_add(GTK_CONTAINER(frame_terminal), alignment_terminal);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment_terminal), 0, 0, 12, 0);

    vbox_terminal = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox_terminal);
    gtk_container_add(GTK_CONTAINER(alignment_terminal), vbox_terminal);

    t_lb_terminal_note = gtk_label_new(lsettings->config.value[LCRT_T_T_LB_TERMINAL_NOTE]);
    gtk_widget_show(t_lb_terminal_note);
    gtk_box_pack_start(GTK_BOX(vbox_terminal), t_lb_terminal_note, FALSE, FALSE, 0);
    gtk_widget_set_size_request(t_lb_terminal_note, -1, 70);
    gtk_misc_set_alignment(GTK_MISC(t_lb_terminal_note), 0.01, 0.5);

    table_terminal = gtk_table_new(6, 2, FALSE);
    gtk_widget_show(table_terminal);
    gtk_box_pack_start(GTK_BOX(vbox_terminal), table_terminal, FALSE, TRUE, 0);
    gtk_table_set_row_spacings(GTK_TABLE(table_terminal), 3);

    t_lb_font = gtk_label_new(lsettings->config.value[LCRT_T_T_LB_FONT]);
    gtk_widget_show(t_lb_font);
    gtk_table_attach(GTK_TABLE(table_terminal), t_lb_font, 0, 1, 0, 1,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(t_lb_font, 125, 30);
    gtk_misc_set_alignment(GTK_MISC(t_lb_font), 0, 0.5);

    t_bt_font = gtk_font_button_new ();
    lsettings->t_bt_font = t_bt_font;
    gtk_widget_show (t_bt_font);
    gtk_table_attach (GTK_TABLE (table_terminal), t_bt_font, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_widget_set_size_request(t_bt_font, -1, 30);
    g_signal_connect(t_bt_font, "font-set",
                     G_CALLBACK(lcrt_settings_terminal_font_button_clicked),
                     lsettings);
    //if (lcrt_check_space_string(lsettings->lt_t_font) == FALSE)
    gtk_font_button_set_font_name(GTK_FONT_BUTTON(t_bt_font), lsettings->lt_t_font);
    t_lb_foreground_color = gtk_label_new(lsettings->config.value[LCRT_T_T_LB_FOREGROUND_COLOR]);
    gtk_widget_show(t_lb_foreground_color);
    gtk_table_attach(GTK_TABLE(table_terminal), t_lb_foreground_color, 0, 1, 1, 2,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(t_lb_foreground_color, 125, 30);
    gtk_misc_set_alignment(GTK_MISC(t_lb_foreground_color), 0, 0.5);

    t_bt_foreground_color = gtk_color_button_new ();
    lsettings->t_bt_foreground_color = t_bt_foreground_color;
    gtk_widget_show (t_bt_foreground_color);
    gtk_table_attach (GTK_TABLE (table_terminal), t_bt_foreground_color, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_widget_set_size_request(t_bt_foreground_color, -1, 30);
    g_signal_connect(t_bt_foreground_color , "color-set", 
                     G_CALLBACK(lcrt_settings_terminal_color_button_clicked),
                     lsettings);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(t_bt_foreground_color), &lsettings->lt_t_fcolor);

    t_lb_background_color = gtk_label_new(lsettings->config.value[LCRT_T_T_LB_BACKGROUND_COLOR]);
    gtk_widget_show(t_lb_background_color);
    gtk_table_attach(GTK_TABLE(table_terminal), t_lb_background_color, 0, 1, 2, 3,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(t_lb_background_color, 125, 30);
    gtk_misc_set_alignment(GTK_MISC(t_lb_background_color), 0, 0.5);

    t_bt_background_color = gtk_color_button_new ();
    lsettings->t_bt_background_color = t_bt_background_color;
    gtk_widget_show (t_bt_background_color);
    gtk_table_attach (GTK_TABLE (table_terminal), t_bt_background_color, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_widget_set_size_request(t_bt_background_color, -1, 30);
    g_signal_connect(t_bt_background_color , "color-set", 
                     G_CALLBACK(lcrt_settings_terminal_color_button_clicked),
                     lsettings);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(t_bt_background_color), &lsettings->lt_t_bcolor);
    t_lb_background_image = gtk_label_new(lsettings->config.value[LCRT_T_T_LB_BACKGROUND_IMAGE]);
    gtk_widget_show(t_lb_background_image);
    gtk_table_attach(GTK_TABLE(table_terminal), t_lb_background_image, 0, 1, 4, 5,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(t_lb_background_image, 125, 30);
    gtk_misc_set_alignment(GTK_MISC(t_lb_background_image), 0, 0.5);

    t_lb_transparent = gtk_label_new(lsettings->config.value[LCRT_T_T_LB_TRANSPARENT]);
    gtk_widget_show(t_lb_transparent);
    gtk_table_attach(GTK_TABLE(table_terminal), t_lb_transparent, 0, 1, 5, 6,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(t_lb_transparent, -1, 30);
    gtk_misc_set_alignment(GTK_MISC(t_lb_transparent), 0, 0.5);

    t_sb_scrollback_lines_adj = gtk_adjustment_new(lsettings->lt_t_scrolllines, 500, 100000, 100, 0, 0);
    lsettings->t_sb_scrollback_lines_adj = t_sb_scrollback_lines_adj;
    t_sb_scrollback_lines = gtk_spin_button_new(GTK_ADJUSTMENT(t_sb_scrollback_lines_adj), 1, 0);
    gtk_widget_show(t_sb_scrollback_lines);
    gtk_table_attach(GTK_TABLE(table_terminal), t_sb_scrollback_lines, 1, 2, 3, 4,
                     (GtkAttachOptions)(GTK_EXPAND | GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(t_sb_scrollback_lines, -1, 30);

    hbox4 = gtk_hbox_new(FALSE, 0);
    gtk_widget_show(hbox4);
    gtk_table_attach(GTK_TABLE(table_terminal), hbox4, 1, 2, 4, 5,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(GTK_FILL), 0, 0);

    t_et_background_image = gtk_entry_new();
    lsettings->t_et_background_image = t_et_background_image;
    gtk_widget_show(t_et_background_image);
    gtk_box_pack_start(GTK_BOX(hbox4), t_et_background_image, TRUE, TRUE, 0);
    gtk_widget_set_size_request(t_et_background_image, -1, 30);
    gtk_entry_set_invisible_char(GTK_ENTRY(t_et_background_image), 9679);
    gtk_entry_set_text(GTK_ENTRY(t_et_background_image), lsettings->lt_t_backimage);

    t_bt_background_image = gtk_button_new();
    lsettings->t_bt_background_image = t_bt_background_image;
    gtk_widget_show(t_bt_background_image);
    gtk_box_pack_start(GTK_BOX(hbox4), t_bt_background_image, FALSE, FALSE, 0);
    g_signal_connect((gpointer)t_bt_background_image, "clicked",
                      G_CALLBACK(lcrt_settings_on_button_background_image_clicked),
                      lsettings);

    image3 = gtk_image_new_from_stock("gtk-open", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image3);
    gtk_container_add(GTK_CONTAINER(t_bt_background_image), image3);

    lsettings->t_hl_transparent = gtk_adjustment_new(lsettings->lt_t_transparent, 0, 100, 1, 0, 0);
    t_hl_transparent = gtk_hscale_new(GTK_ADJUSTMENT(lsettings->t_hl_transparent));
    gtk_widget_show(t_hl_transparent);
    gtk_table_attach(GTK_TABLE(table_terminal), t_hl_transparent, 1, 2, 5, 6,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(GTK_FILL), 0, 0);
    tooltips = gtk_tooltips_new();
    gtk_tooltips_set_tip(tooltips, t_hl_transparent, lsettings->config.value[LCRT_T_T_TP_TRANSPARENT], NULL);
    gtk_widget_set_size_request(t_hl_transparent, -1, 30);
    gtk_scale_set_draw_value(GTK_SCALE(t_hl_transparent), FALSE);
    g_signal_connect((gpointer)lsettings->t_hl_transparent, "value-changed",
                      G_CALLBACK(lcrt_settings_terminal_on_transparent_value_changed),
                      lsettings);

    t_lb_scrollback_lines = gtk_label_new(lsettings->config.value[LCRT_T_T_LB_SCROLLBACK_LINES]);
    gtk_widget_show(t_lb_scrollback_lines);
    gtk_table_attach(GTK_TABLE(table_terminal), t_lb_scrollback_lines, 0, 1, 3, 4,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(t_lb_scrollback_lines, 125, 30);
    gtk_misc_set_alignment(GTK_MISC(t_lb_scrollback_lines), 0, 0.5);

    hbox8 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox8);
    gtk_box_pack_start (GTK_BOX (vbox_terminal), hbox8, FALSE, FALSE, 0);

    t_cb_bell = gtk_check_button_new_with_mnemonic(lsettings->config.value[LCRT_T_T_CB_BELL]);
    lsettings->t_cb_bell = t_cb_bell;
    gtk_widget_show(t_cb_bell);
    gtk_box_pack_start (GTK_BOX (hbox8), t_cb_bell, FALSE, FALSE, 0);
    gtk_widget_set_size_request(t_cb_bell, -1, 30);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(t_cb_bell), lsettings->lt_t_bell);


    fixed1 = gtk_fixed_new ();
    gtk_widget_show (fixed1);
    gtk_box_pack_start (GTK_BOX (hbox8), fixed1, FALSE, FALSE, 0);
    gtk_widget_set_size_request (fixed1, 120, -1);

    t_bt_restore_default = gtk_button_new_with_mnemonic(lsettings->config.value[LCRT_T_T_BT_RESTORE_DEFAULT]);
    lsettings->t_bt_restore_default = t_bt_restore_default;
    gtk_widget_show (t_bt_restore_default);
    gtk_box_pack_start (GTK_BOX (hbox8), t_bt_restore_default, FALSE, FALSE, 0);
    gtk_widget_set_size_request(t_bt_restore_default, 90, 30);
    g_signal_connect((gpointer)t_bt_restore_default, "clicked",
                     G_CALLBACK(lcrt_settings_terminal_on_restore_default_clicked),
                     lsettings);

    t_lb_terminal_settings = gtk_label_new(lsettings->config.value[LCRT_T_T_LB_TERMINAL_SETTINGS]);
    gtk_widget_show(t_lb_terminal_settings);
    gtk_frame_set_label_widget(GTK_FRAME(frame_terminal), t_lb_terminal_settings);
    gtk_label_set_use_markup(GTK_LABEL(t_lb_terminal_settings), TRUE);

    t_nb_terminal = gtk_label_new(lsettings->config.value[LCRT_T_T_NB_TERMINAL]);
    lsettings->t_nb_terminal = t_nb_terminal;
    gtk_widget_show(t_nb_terminal);
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), 2), t_nb_terminal);
    gtk_widget_set_size_request(t_nb_terminal, 80, -1);
    gtk_misc_set_alignment(GTK_MISC(t_nb_terminal), 0.01, 0.5);

    frame_application = gtk_frame_new(NULL);
    gtk_widget_show(frame_application);
    gtk_container_add(GTK_CONTAINER(notebook), frame_application);
    gtk_frame_set_shadow_type(GTK_FRAME(frame_application), GTK_SHADOW_NONE);

    alignment6 = gtk_alignment_new(0.5, 0.5, 1, 1);
    gtk_widget_show(alignment6);
    gtk_container_add(GTK_CONTAINER(frame_application), alignment6);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment6), 0, 0, 12, 0);

    vbox7 = gtk_vbox_new(FALSE, 0);
    gtk_widget_show(vbox7);
    gtk_container_add(GTK_CONTAINER(alignment6), vbox7);

    a_lb_application_note = gtk_label_new(lsettings->config.value[LCRT_T_A_LB_APPLICATION_NOTE]);
    gtk_widget_show(a_lb_application_note);
    gtk_box_pack_start(GTK_BOX(vbox7), a_lb_application_note, FALSE, FALSE, 0);
    gtk_widget_set_size_request(a_lb_application_note, -1, 80);
    gtk_misc_set_alignment(GTK_MISC(a_lb_application_note), 0.01, 0.5);

    table2 = gtk_table_new(3, 3, FALSE);
    gtk_widget_show(table2);
    gtk_box_pack_start(GTK_BOX(vbox7), table2, TRUE, TRUE, 0);

    a_lb_shell = gtk_label_new(lsettings->config.value[LCRT_T_A_LB_SHELL]);
    gtk_widget_show(a_lb_shell);
    gtk_table_attach(GTK_TABLE(table2), a_lb_shell, 0, 1, 0, 1,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(a_lb_shell, 60, -1);
    gtk_misc_set_alignment(GTK_MISC(a_lb_shell), 0, 0.5);

    a_lb_ssh = gtk_label_new(lsettings->config.value[LCRT_T_A_LB_SSH]);
    gtk_widget_show(a_lb_ssh);
    gtk_table_attach(GTK_TABLE(table2), a_lb_ssh, 0, 1, 1, 2,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(a_lb_ssh, 60, -1);
    gtk_misc_set_alignment(GTK_MISC(a_lb_ssh), 0, 0.5);

    a_lb_telnet = gtk_label_new(lsettings->config.value[LCRT_T_A_LB_TELNET]);
    gtk_widget_show(a_lb_telnet);
    gtk_table_attach(GTK_TABLE(table2), a_lb_telnet, 0, 1, 2, 3,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(a_lb_telnet, 60, -1);
    gtk_misc_set_alignment(GTK_MISC(a_lb_telnet), 0, 0.5);

    a_et_ssh = gtk_entry_new();
    lsettings->a_et_ssh = a_et_ssh;
    gtk_widget_show(a_et_ssh);
    gtk_table_attach(GTK_TABLE(table2), a_et_ssh, 1, 2, 1, 2,
                     (GtkAttachOptions)(GTK_EXPAND | GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_entry_set_invisible_char(GTK_ENTRY(a_et_ssh), 9679);

    a_et_telnet = gtk_entry_new();
    lsettings->a_et_telnet = a_et_telnet;
    gtk_widget_show(a_et_telnet);
    gtk_table_attach(GTK_TABLE(table2), a_et_telnet, 1, 2, 2, 3,
                     (GtkAttachOptions)(GTK_EXPAND | GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_entry_set_invisible_char(GTK_ENTRY(a_et_telnet), 9679);

    a_bt_telnet = gtk_button_new();
    lsettings->a_bt_telnet = a_bt_telnet;
    gtk_widget_show(a_bt_telnet);
    gtk_table_attach(GTK_TABLE(table2), a_bt_telnet, 2, 3, 2, 3,
                     (GtkAttachOptions)(GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(a_bt_telnet, 32, 32);

    image6 = gtk_image_new_from_stock("gtk-open", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image6);
    gtk_container_add(GTK_CONTAINER(a_bt_telnet), image6);

    a_bt_shell = gtk_button_new();
    lsettings->a_bt_shell = a_bt_shell;
    gtk_widget_show(a_bt_shell);
    gtk_table_attach(GTK_TABLE(table2), a_bt_shell, 2, 3, 0, 1,
                     (GtkAttachOptions)(0),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(a_bt_shell, 32, 32);

    image4 = gtk_image_new_from_stock("gtk-open", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image4);
    gtk_container_add(GTK_CONTAINER(a_bt_shell), image4);

    a_et_shell = gtk_entry_new();
    lsettings->a_et_shell = a_et_shell;
    gtk_widget_show(a_et_shell);
    gtk_table_attach(GTK_TABLE(table2), a_et_shell, 1, 2, 0, 1,
                     (GtkAttachOptions)(GTK_EXPAND | GTK_FILL),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_entry_set_invisible_char(GTK_ENTRY(a_et_shell), 9679);

    a_bt_ssh = gtk_button_new();
    lsettings->a_bt_ssh = a_bt_ssh;
    gtk_widget_show(a_bt_ssh);
    gtk_table_attach(GTK_TABLE(table2), a_bt_ssh, 2, 3, 1, 2,
                     (GtkAttachOptions)(0),
                     (GtkAttachOptions)(0), 0, 0);
    gtk_widget_set_size_request(a_bt_ssh, 32, 32);

    image5 = gtk_image_new_from_stock("gtk-open", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image5);
    gtk_container_add(GTK_CONTAINER(a_bt_ssh), image5);

    a_lb_application = gtk_label_new(lsettings->config.value[LCRT_T_A_LB_APPLICATION]);
    gtk_widget_show(a_lb_application);
    gtk_frame_set_label_widget(GTK_FRAME(frame_application), a_lb_application);
    gtk_label_set_use_markup(GTK_LABEL(a_lb_application), TRUE);

    a_nb_application = gtk_label_new(lsettings->config.value[LCRT_T_A_NB_APPLICATION]);
    lsettings->a_nb_application = a_nb_application;
    gtk_widget_show(a_nb_application);
    gtk_widget_set_size_request(a_nb_application, 80, -1);
    gtk_misc_set_alignment(GTK_MISC(a_nb_application), 0.01, 0.5);
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), 3), a_nb_application);

    dialog_action_area = GTK_DIALOG(dialog_settings)->action_area;
    gtk_widget_show(dialog_action_area);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(dialog_action_area), GTK_BUTTONBOX_END);

    applybutton = gtk_button_new();
    gtk_widget_show(applybutton);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_settings), applybutton, GTK_RESPONSE_APPLY);
    GTK_WIDGET_SET_FLAGS(applybutton, GTK_CAN_DEFAULT);
  
    alignment7 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment7);
    gtk_container_add(GTK_CONTAINER(applybutton), alignment7);
  
    hbox5 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox5);
    gtk_container_add(GTK_CONTAINER(alignment7), hbox5);
  
    image4 = gtk_image_new_from_stock("gtk-apply", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image4);
    gtk_box_pack_start(GTK_BOX(hbox5), image4, FALSE, FALSE, 0);

    lb_applybutton = gtk_label_new_with_mnemonic(lsettings->config.value[LCRT_T_LB_APPLYBUTTON]);
    gtk_widget_show(lb_applybutton);
    gtk_box_pack_start(GTK_BOX(hbox5), lb_applybutton, FALSE, FALSE, 0);
  
    cancelbutton = gtk_button_new();
    gtk_widget_show(cancelbutton);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_settings), cancelbutton, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS(cancelbutton, GTK_CAN_DEFAULT);
  
    alignment8 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment8);
    gtk_container_add(GTK_CONTAINER(cancelbutton), alignment8);
  
    hbox6 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox6);
    gtk_container_add(GTK_CONTAINER(alignment8), hbox6);
  
    image5 = gtk_image_new_from_stock("gtk-cancel", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image5);
    gtk_box_pack_start(GTK_BOX(hbox6), image5, FALSE, FALSE, 0);
  
    lb_cancelbutton = gtk_label_new_with_mnemonic(lsettings->config.value[LCRT_T_LB_CANCELBUTTON]);
    gtk_widget_show(lb_cancelbutton);
    gtk_box_pack_start(GTK_BOX(hbox6), lb_cancelbutton, FALSE, FALSE, 0);
  
    okbutton = gtk_button_new();
    gtk_widget_show(okbutton);
    gtk_dialog_add_action_widget(GTK_DIALOG(dialog_settings), okbutton, GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS(okbutton, GTK_CAN_DEFAULT);
  
    alignment9 = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_widget_show(alignment9);
    gtk_container_add(GTK_CONTAINER(okbutton), alignment9);
  
    hbox7 = gtk_hbox_new(FALSE, 2);
    gtk_widget_show(hbox7);
    gtk_container_add(GTK_CONTAINER(alignment9), hbox7);
  
    image6 = gtk_image_new_from_stock("gtk-ok", GTK_ICON_SIZE_BUTTON);
    gtk_widget_show(image6);
    gtk_box_pack_start(GTK_BOX(hbox7), image6, FALSE, FALSE, 0);
  
    lb_okbutton = gtk_label_new_with_mnemonic(lsettings->config.value[LCRT_T_LB_OKBUTTON]);
    gtk_widget_show(lb_okbutton);
    gtk_box_pack_start(GTK_BOX(hbox7), lb_okbutton, FALSE, FALSE, 0);

    g_signal_connect((gpointer)applybutton, "clicked",
                      G_CALLBACK(lcrt_settings_on_applybutton_clicked),
                      lsettings);
    g_signal_connect((gpointer)cancelbutton, "clicked",
                      G_CALLBACK(lcrt_settings_on_cancelbutton_clicked),
                      lsettings);
    g_signal_connect((gpointer)okbutton, "clicked",
                      G_CALLBACK(lcrt_settings_on_okbutton_clicked),
                      lsettings);
    g_signal_connect((gpointer) dialog_settings, "delete_event",
                      G_CALLBACK (lcrt_settings_on_window_delete),
                      lsettings);
    return lsettings;
err:
    lcrt_message_error(parent->window, lsettings->config.value[LCRT_T_ERR_MSG]);
    return NULL;

}
void lcrt_destroy_settings(struct lcrt_settings *lsettings)
{
    if (lsettings->auto_save_config)
        lcrt_settings_save_local_config(lsettings);

    if (lsettings)
        free(lsettings);
}
static const char *lcrt_settings_get_ldb_name(struct lcrt_settings *lsettings)
{
    return lsettings == NULL ? NULL : lsettings->config.ldb.db_name;
}
static const char *lcrt_settings_get_ltb_name(struct lcrt_settings *lsettings)
{
    return lsettings == NULL ? NULL : lsettings->config.ldb.db_table;
}

static const char *lcrt_settings_get_db_name(struct lcrt_settings *lsettings)
{
    return lsettings->config.db.db_name;
}
static const char *lcrt_settings_get_tb_name(struct lcrt_settings *lsettings)
{
    return lsettings->config.db.db_table;
}

int lcrt_settings_init_language_config(struct lcrt_settings *lsettings)
{
    char db_name[256];
    int i;
    static char *name[LCRT_T_NUMBER] = {LCRT_T_NAME};
    static char *value[LCRT_T_NUMBER] = {LCRT_T_VALUE};

    assert(lsettings);

    memset(&lsettings->config, 0, sizeof(struct lcrtc_settings));
    snprintf(db_name, sizeof(db_name), "%s", lcrt_config_get_language());
    lcrt_config_init(&lsettings->config.db, db_name, LCRT_ISETTINGS_TABLE);

    for (i = 0; i < LCRT_T_NUMBER; i++) {
        lsettings->config.name[i] = name[i];
        strncpy(lsettings->config.value[i], value[i], sizeof(lsettings->config.value[i]));
    }
    lsettings->get_db = lcrt_settings_get_db_name;
    lsettings->get_tb = lcrt_settings_get_tb_name;

    return 0;
}
int lcrt_settings_init_local_config(struct lcrt_settings *lsettings)
{
    char db_name[256];
    int i;
    static char *config[LCRT_LT_NUMBER] = {LCRT_LT_CONFIG};

    snprintf(db_name, sizeof(db_name), "%s/%s/", lcrt_config_get_local_dir(), LCRT_SETTINGS_CONFIG_DIR);
    lcrt_fmkdir(db_name);
    strcat(db_name, LCRT_SETTINGS_DB);
    lcrt_config_init(&lsettings->config.ldb, db_name, LCRT_ISETTINGS_TABLE);

    for (i = 0; i < LCRT_LT_NUMBER; i++) {
        strncpy(lsettings->config.local[i], config[i], sizeof(lsettings->config.local[i]));
    }
    lsettings->get_ldb = lcrt_settings_get_ldb_name;
    lsettings->get_ltb = lcrt_settings_get_ltb_name;

    return 0;
}
int lcrt_settings_init_config(struct lcrt_settings *lsettings)
{
    assert(lsettings);

    memset(&lsettings->config, 0, sizeof(struct lcrtc_settings));
    lcrt_settings_init_language_config(lsettings);
    lcrt_settings_init_local_config(lsettings);
    return LCRTE_OK;
}
int lcrt_settings_load_language_config(struct lcrt_settings *lsettings)
{
    int rv, i;

    assert(lsettings);

    rv = lsettings->config.db.select(&lsettings->config.db, "SELECT * FROM %s", lsettings->get_tb(lsettings));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_T_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lsettings->config.value[i], lsettings->config.db.get_text_col(&lsettings->config.db, 1), 
                sizeof(lsettings->config.value[i]));
        rv = lsettings->config.db.get_row(&lsettings->config.db);
        debug_print("[%-10s|%-10s]: name = [%-20s] value = [%-20s]\n",
                basename((char *)lsettings->get_db(lsettings)),
                lsettings->get_tb(lsettings),
                lsettings->config.name[i],
                lsettings->config.value[i]);
    }

    return rv;
}
int lcrt_settings_load_local_config(struct lcrt_settings *lsettings)
{
    int rv, i;

    static char *name[LCRT_LT_NUMBER] = {LCRT_LT_NAME};
    assert(lsettings);

    rv = lsettings->config.ldb.select(&lsettings->config.ldb, "SELECT * FROM %s", lsettings->get_ltb(lsettings));
    debug_where();
    if (rv == LCRTE_NO_TABLE) {
        lcrt_settings_create_local_config(lsettings);
        return LCRTE_NO_CONFIG;
    }
    debug_where();
    for (i = 0; i < LCRT_LT_NUMBER && rv == LCRTE_OK; i++) {
        strncpy(lsettings->config.local[i], lsettings->config.ldb.get_text_col(&lsettings->config.ldb, 1), 
                sizeof(lsettings->config.local[i]));
        rv = lsettings->config.ldb.get_row(&lsettings->config.ldb);
        debug_print("[%-10s|%-10s]: name = [%-20s] config = [%-20s]\n",
                basename((char *)lsettings->get_ldb(lsettings)),
                lsettings->get_ltb(lsettings),
                name[i],
                lsettings->config.local[i]);
    }

    return rv;
}
int lcrt_settings_load_config(struct lcrt_settings *lsettings)
{
    lcrt_settings_load_language_config(lsettings);
    lcrt_settings_load_local_config(lsettings);
    return LCRTE_OK;
}


int lcrt_settings_create_local_config(struct lcrt_settings *lsettings)
{
    int i;

    char *name[LCRT_LT_NUMBER] = {LCRT_LT_NAME};
    assert(lsettings);
    lsettings->config.ldb.exec(&lsettings->config.ldb, 
                         "CREATE TABLE %s( \
                          name VARCHAR(%d) PRIMARY KEY, \
                          value VARCHAR(%d))",
                         lsettings->get_ltb(lsettings),
                         LCRT_CONFIG_NAME_LEN,
                         LCRT_CONFIG_VALUE_LEN);
    for (i = 0; i < LCRT_LT_NUMBER; i++) {
        lsettings->config.ldb.exec(&lsettings->config.ldb,
                             "INSERT INTO %s VALUES('%s', '%s')",
                             lsettings->get_ltb(lsettings),
                             name[i],
                             lsettings->config.local[i]);
    }
    lsettings->config.ldb.close(&lsettings->config.ldb);
    return 0;
}
int lcrt_settings_convert_local_config(struct lcrt_settings *lsettings)
{
    struct lcrtc_settings *config;

    if(lsettings == NULL)
        return EINVAL;

    config = &lsettings->config;

    lsettings->lt_g_show_dialog = atoi(config->local[LCRT_LT_G_SHOW_DIALOG]);
    lsettings->lt_g_show_toolbar = atoi(config->local[LCRT_LT_G_SHOW_TOOLBAR]);
    lsettings->lt_g_show_statusbar = atoi(config->local[LCRT_LT_G_SHOW_STATUSBAR]);
    strcpy(lsettings->lt_t_font, config->local[LCRT_LT_T_FONT]);

    sscanf(config->local[LCRT_LT_T_FCOLOR], "%x %x %x %x", 
            (unsigned int *)&lsettings->lt_t_fcolor.pixel,
            (unsigned int *)&lsettings->lt_t_fcolor.red,
            (unsigned int *)&lsettings->lt_t_fcolor.green,
            (unsigned int *)&lsettings->lt_t_fcolor.blue);

    sscanf(config->local[LCRT_LT_T_BCOLOR], "%x %x %x %x", 
            (unsigned int *)&lsettings->lt_t_bcolor.pixel,
            (unsigned int *)&lsettings->lt_t_bcolor.red,
            (unsigned int *)&lsettings->lt_t_bcolor.green,
            (unsigned int *)&lsettings->lt_t_bcolor.blue);

    lsettings->lt_t_scrolllines = atoi(config->local[LCRT_LT_T_SCROLLLINES]);
    lsettings->lt_t_transparent = atoi(config->local[LCRT_LT_T_TRANSPARENT]);
    lsettings->lt_t_bell = atoi(config->local[LCRT_LT_T_BELL]);
    strcpy(lsettings->lt_t_backimage, config->local[LCRT_LT_T_BACKIMAGE]);

    strcpy(lsettings->lt_a_shell, config->local[LCRT_LT_A_SHELL]);
    strcpy(lsettings->lt_a_ssh, config->local[LCRT_LT_A_SSH]);
    strcpy(lsettings->lt_a_telnet, config->local[LCRT_LT_A_TELNET]);

    lsettings->auto_save_config = atoi(config->local[LCRT_LT_AUTO_SAVE_CONFIG]);
    lsettings->enable_f10_key = atoi(config->local[LCRT_LT_ENABLE_F10]);

    return LCRTE_OK;
}
int lcrt_settings_save_local_config(struct lcrt_settings *lsettings)
{
    int i;
    char *name[LCRT_LT_NUMBER] = {LCRT_LT_NAME};
    struct lcrtc_settings *config;

    if(lsettings == NULL)
        return EINVAL;

    config = &lsettings->config;
    sprintf(config->local[LCRT_LT_G_SHOW_DIALOG], "%s", !!lsettings->lt_g_show_dialog ? "1" : "0");
    sprintf(config->local[LCRT_LT_G_SHOW_TOOLBAR], "%s", !!lsettings->lt_g_show_toolbar ? "1" : "0");
    sprintf(config->local[LCRT_LT_G_SHOW_STATUSBAR], "%s", !!lsettings->lt_g_show_statusbar ? "1" : "0");
    strcpy(config->local[LCRT_LT_T_FONT], lsettings->lt_t_font);

    sprintf(config->local[LCRT_LT_T_FCOLOR], "%8.8x %4.4x %4.4x %4.4x", 
            lsettings->lt_t_fcolor.pixel,
            lsettings->lt_t_fcolor.red,
            lsettings->lt_t_fcolor.green,
            lsettings->lt_t_fcolor.blue);

    sprintf(config->local[LCRT_LT_T_BCOLOR], "%8.8x %4.4x %4.4x %4.4x",
            lsettings->lt_t_bcolor.pixel,
            lsettings->lt_t_bcolor.red,
            lsettings->lt_t_bcolor.green,
            lsettings->lt_t_bcolor.blue);
    sprintf(config->local[LCRT_LT_T_SCROLLLINES], "%d", lsettings->lt_t_scrolllines);
    sprintf(config->local[LCRT_LT_T_TRANSPARENT], "%d", lsettings->lt_t_transparent);
    sprintf(config->local[LCRT_LT_T_BELL], "%s", !!lsettings->lt_t_bell? "1" : "0");
    strcpy(config->local[LCRT_LT_T_BACKIMAGE], lsettings->lt_t_backimage);

    strcpy(config->local[LCRT_LT_A_SHELL], lsettings->lt_a_shell);
    strcpy(config->local[LCRT_LT_A_SSH], lsettings->lt_a_ssh);
    strcpy(config->local[LCRT_LT_A_TELNET], lsettings->lt_a_telnet);

    sprintf(config->local[LCRT_LT_AUTO_SAVE_CONFIG], "%d", lsettings->auto_save_config);
    sprintf(config->local[LCRT_LT_ENABLE_F10], "%d", lsettings->enable_f10_key);

    for (i = 0; i < LCRT_LT_NUMBER; i++) {
        lsettings->config.ldb.exec(&lsettings->config.ldb, 
            "UPDATE %s SET value='%s' WHERE name='%s'",
            lsettings->get_ltb(lsettings),
            lsettings->config.local[i],
            name[i]);
    
        if (lsettings->config.ldb.changes(&lsettings->config.ldb) == 0) {
            lsettings->config.ldb.exec(&lsettings->config.ldb,
                           "INSERT INTO %s VALUES('%s', '%s')",
                           lsettings->get_ltb(lsettings),
                           name[i],
                           lsettings->config.local[i]);
        }
    }
    lsettings->config.ldb.close(&lsettings->config.ldb);
    return LCRTE_OK;
}

