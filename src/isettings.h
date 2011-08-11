#ifndef __LCRT_ISETTINGS_H__
#define __LCRT_ISETTINGS_H__
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "mkconfig.h"
#include "imenubar.h"

#define LCRT_ISETTINGS_TABLE "isettings"
#define LCRT_SETTINGS_CONFIG_DIR "etc/settings"
#define LCRT_SETTINGS_DB "settings.db"

enum {
    LCRT_T_DIALOG_SETTINGS,
    /*general settings*/
    LCRT_T_G_NB_GENERAL,
    LCRT_T_G_LB_LANGUAGE_SUPPORT, 
    LCRT_T_G_LB_LANGUAGE_SUPPORT_NOTE,
    LCRT_T_G_LB_CURRENT_LANGUAGE,
    LCRT_T_G_LB_DIALOG,
    LCRT_T_G_RB_CONNECT, 
    LCRT_T_G_RB_QUICK_CONNECT,
    LCRT_T_G_RB_NONE,
    LCRT_T_G_LB_VIEW,
    LCRT_T_G_CB_SHOW_TOOLBAR,
    LCRT_T_G_CB_SHOW_STATUSBAR,
    /*keybinding settings*/
    LCRT_T_K_NB_KEYBINDINGS,
    LCRT_T_K_LB_KEYBOARD_SHORTCUT,
    LCRT_T_K_LB_KEYBOARD_SHORTCUTS_NOTE,
    LCRT_T_K_CB_ENABLE_F10,
    /*terminal settings*/
    LCRT_T_T_NB_TERMINAL,
    LCRT_T_T_LB_TERMINAL_SETTINGS,
    LCRT_T_T_LB_TERMINAL_NOTE,
    LCRT_T_T_LB_FONT,
    LCRT_T_T_LB_FOREGROUND_COLOR,
    LCRT_T_T_LB_BACKGROUND_COLOR,
    LCRT_T_T_LB_BACKGROUND_IMAGE,
    LCRT_T_T_LB_TRANSPARENT,
    LCRT_T_T_TP_TRANSPARENT,
    LCRT_T_T_LB_SCROLLBACK_LINES,
    LCRT_T_T_CB_BELL,
    LCRT_T_T_BT_RESTORE_DEFAULT,
    /*application settings*/
    LCRT_T_A_NB_APPLICATION,
    LCRT_T_A_LB_APPLICATION,
    LCRT_T_A_LB_APPLICATION_NOTE,
    LCRT_T_A_LB_SHELL,
    LCRT_T_A_LB_SSH,
    LCRT_T_A_LB_TELNET,
    LCRT_T_LB_APPLYBUTTON,
    LCRT_T_LB_CANCELBUTTON,
    LCRT_T_LB_OKBUTTON,
    LCRT_T_ERR_MSG,
    LCRT_T_NUMBER
};
#define LCRT_T_NAME \
    "dialog_settings", \
    /*general settings*/ \
    "g_nb_general", \
    "g_lb_language_support", \
    "g_lb_language_support_note", \
    "g_lb_current_language", \
    "g_lb_dialog", \
    "g_rb_connect", \
    "g_rb_quick_connect", \
    "g_rb_none", \
    "g_lb_view", \
    "g_cb_show_toolbar", \
    "g_cb_show_statusbar", \
    /*keybinding settings*/ \
    "k_nb_keybindings", \
    "k_lb_keyboard_shortcut", \
    "k_lb_keyboard_shortcuts_note", \
    "k_cb_enable_f10", \
    /*terminal settings*/ \
    "t_nb_terminal", \
    "t_lb_terminal_settings", \
    "t_lb_terminal_note", \
    "t_lb_font", \
    "t_lb_foreground_color", \
    "t_lb_background_color", \
    "t_lb_background_image", \
    "t_lb_transparent", \
    "t_tp_transparent", \
    "t_lb_scrollback_lines", \
    "t_cb_bell", \
    "t_bt_restore_default", \
    /*application settintgs*/ \
    "a_nb_application", \
    "a_lb_application", \
    "a_lb_application_note", \
    "a_lb_shell", \
    "a_lb_ssh", \
    "a_lb_telnet", \
    "lb_applybutton", \
    "lb_cancelbutton", \
    "lb_okbutton", \
    "error_msg"

#define LCRT_T_VALUE \
    "Global options", \
    /*general settings*/ \
    "General", \
    "<b>Language support</b>", \
    "Select language support, and will be effect on \nnext time.", \
    "Current language:", \
    "<b>Dialog</b>", \
    "Show connect dialog on startup", \
    "Show quick connect dialog on startup", \
    "None", \
    "<b>View</b>", \
    "Show toolbar", \
    "Show statusbar", \
    /*keybinding settings*/ \
    "Keybindings", \
    "<b>Keyboard shortcuts</b>", \
    "Here you can change keyboard shortcuts for\nvarious actions.", \
    "Enable the menu shortcut key(F10 by default)", \
    /*terminal settings*/ \
    "Terminal", \
    "<b>Terminal settings</b>", \
    "These settings for the virtual terminal emulator \nwidget(VTE)only apply if the VTE library could be \nloaded.", \
    "Terminal font:", \
    "Font color:", \
    "Background color:", \
    "Background image:", \
    "Transparent ", \
    "Notice: only transparent is set to 0, the background image can be displayed.", \
    "Scrollback lines:", \
    "Terminal bell", \
    "Reset", \
    /*application settintgs*/ \
    "Application", \
    "<b>Applications</b>", \
    "As you known, lcrt will invoke local application\nwhen you open a connect such as ssh, so in here,\nyou can config it.", \
    "shell:", \
    "ssh:", \
    "telnet:", \
    "Apply", \
    "Cancel", \
    "OK", \
    "There is no memory to create dialog"

typedef enum {
    LCRT_LT_SHOW_DIALOG_NONE,
    LCRT_LT_SHOW_DIALOG_CONNECT,
    LCRT_LT_SHOW_DIALOG_QUICK_CONNECT,
}lcrt_lt_show_dialog_t;

enum {
    LCRT_LT_G_SHOW_DIALOG,
    LCRT_LT_G_SHOW_TOOLBAR,
    LCRT_LT_G_SHOW_STATUSBAR,
    LCRT_LT_T_FONT,
    LCRT_LT_T_FCOLOR,
    LCRT_LT_T_BCOLOR,
    LCRT_LT_T_SCROLLLINES,
    LCRT_LT_T_BACKIMAGE,
    LCRT_LT_T_TRANSPARENT,
    LCRT_LT_T_BELL,
    LCRT_LT_A_SHELL,
    LCRT_LT_A_SSH,
    LCRT_LT_A_TELNET,
    LCRT_LT_AUTO_SAVE_CONFIG, /*options->auto save config*/
    LCRT_LT_ENABLE_F10,
    LCRT_LT_NUMBER,
};

#define LCRT_LT_NAME \
    "lt_g_show_dialog", \
    "lt_g_show_toolbar", \
    "lt_g_show_statusbar", \
    "lt_t_font", \
    "lt_t_fcolor", \
    "lt_t_bcolor", \
    "lt_t_scrolllines", \
    "lt_t_backimage", \
    "lt_t_transparent", \
    "lt_t_bell", \
    "lt_a_shell", \
    "lt_a_ssh", \
    "lt_a_telnet", \
    "lt_auto_save_config", \
    "lt_enable_f10"

#define LCRT_LT_CONFIG \
    "3", \
    "1", \
    "1", \
    " ", \
    "00000000 AAAA AAAA AAAA", \
    "00000000 0000 0000 0000", \
    "1000", \
    " ", \
    "0", \
    "1", \
    "/bin/bash", \
    "/usr/bin/ssh", \
    "/usr/bin/telnet", \
    "1", \
    "1"

struct lcrtc_settings {
    struct lcrt_config db;
    struct lcrt_config ldb;
    const char *name[LCRT_T_NUMBER];
    char value[LCRT_T_NUMBER][LCRT_CONFIG_VALUE_LEN];
    char local[LCRT_LT_NUMBER][LCRT_CONFIG_VALUE_LEN];
};
struct lcrt_settings {
    struct lcrt_window *parent;

    GtkWidget *dialog_settings;
    GtkWidget *notebook;
    /*general settings*/
    GtkWidget *g_nb_general;
    GtkWidget *g_cb_language;
    GtkWidget *g_rb_connect;
    GtkWidget *g_rb_quick_connect;
    GtkWidget *g_rb_none;
    GtkWidget *g_cb_show_toolbar;
    GtkWidget *g_cb_show_statusbar;
    /*keybinding settings*/
    GtkWidget *k_nb_keybindings;
    GtkWidget *k_tv_shortcurts;
    GtkWidget *k_cb_enable_f10;
    /*terminal settings*/
    GtkWidget *t_nb_terminal;
    GtkWidget *t_bt_foreground_color;
    GtkWidget *t_bt_font;
    GtkWidget *t_bt_background_color;
    GtkWidget *t_cb_bell;
    GtkObject *t_sb_scrollback_lines_adj;
    GtkWidget *t_et_background_image;
    GtkWidget *t_bt_background_image;
    GtkObject *t_hl_transparent;
    GtkWidget *t_bt_restore_default;
    /*application settings*/
    GtkWidget *a_nb_application;
    GtkWidget *a_et_ssh;
    GtkWidget *a_et_telnet;
    GtkWidget *a_bt_telnet;
    GtkWidget *a_bt_shell;
    GtkWidget *a_et_shell;
    GtkWidget *a_bt_ssh;

    lcrt_lt_show_dialog_t lt_g_show_dialog;
    int                   lt_g_show_toolbar;
    int                   lt_g_show_statusbar;

    char     lt_t_font[128];
    GdkColor lt_t_fcolor;
    GdkColor lt_t_bcolor;
    int      lt_t_scrolllines;
    char     lt_t_backimage[256];
    int      lt_t_transparent;
    int      lt_t_bell;

    char lt_a_shell[256];
    char lt_a_ssh[256];
    char lt_a_telnet[256];

    int auto_save_config;
    int enable_f10_key;

    gboolean session;

    struct lcrtc_settings config;
    const char *(*get_db)(struct lcrt_settings *lsettings);
    const char *(*get_tb)(struct lcrt_settings *lsettings);
    const char *(*get_ldb)(struct lcrt_settings *lsettings);
    const char *(*get_ltb)(struct lcrt_settings *lsettings);
};
struct lcrt_settings *lcrt_create_settings(struct lcrt_window *parent);
void lcrt_destroy_settings(struct lcrt_settings *lsettings);
int lcrt_settings_convert_local_config(struct lcrt_settings *lsettings);
int lcrt_settings_init_local_config(struct lcrt_settings *lsettings);
int lcrt_settings_save_local_config(struct lcrt_settings *lsettings);
#endif
