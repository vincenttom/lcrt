/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Sun 20 Feb 2011 01:54:10 PM CST
 * File Name   : en_US.c
 *
 * Description:  
 */

#include <stdio.h>
#include "iconnect.h"
#include "iedit.h"
#include "ifile.h"
#include "ifind.h"
#include "ihelp.h"
#include "ilock.h"
#include "ilogin.h"
#include "ioptions.h"
#include "ipopup.h"
#include "iqconnect.h"
#include "irename.h"
#include "istatusbar.h"
#include "istatus.h"
#include "itoolbar.h"
#include "iview.h"
#include "isettings.h"
#include "iaccels.h"
#include "inotebook.h"

#include "language.h"

static struct lcrt_language_config ifile = {
    .table_name = LCRT_IFILE_TABLE,
    .members = LCRT_F_NUMBER,
    .config = {
         {"f_menuitem",                      "_File"},
         {"f_connect",                       "_Connect ..."},
         {"f_quick_connect",                 "_Quick Connect ..."},
         {"f_connect_in_tab",                "Connect in Ta_b"},
         {"f_reconnect",                     "_Reconnect"},
         {"f_reconnect_all",                 "Reconnect _All"},
         {"f_disconnect",                    "_Disconnect"},
         {"f_disconnect_all",                "Disc_onnect All"},
         {"f_clone_session",                 "Clo_ne Session"},
         {"f_lock_session",                  "Loc_k Session ..."},
         {"f_print",                         "_Print"},
         {"f_print_setup",                   "Print Set_up ..."},
         {"f_log_session",                   "_Log Session"},
         {"f_recent_session",                "Recent Session"},
         {"f_exit",                          "E_xit"},
    }
};

static struct lcrt_language_config iedit = {
    .table_name = LCRT_IEDIT_TABLE,
    .members = LCRT_E_NUMBER,
    .config = {
         {"e_menuitem",                      "_Edit"},
         {"e_copy",                          "_Copy"},
         {"e_paste",                         "_Paste"},
         {"e_copy_and_paste",                "C_opy and Paste"},
         {"e_select_all",                    "Select _All"},
         {"e_find",                          "_Find ..."},
         {"e_clear_scrollback",              "Clear Scroll_back"},
         {"e_clear_screen",                  "Clear _Screen"},
         {"e_clear_screen_and_scrollback",   "Clear Screen and Scrollbac_k"},
         {"e_reset",                         "_Reset"},
         {"e_show_menubar",                  "Show Menubar"},
    }
};

static struct lcrt_language_config iview = {
    .table_name = LCRT_IVIEW_TABLE,
    .members = LCRT_V_NUMBER,
    .config = {
         {"v_menuitem",                      "_View"},
         {"v_menubar",                       "_Menu Bar"},
         {"v_toolbar",                       "_Toolbar"},
         {"v_status_bar",                    "_Status Bar"},
         {"v_always_on_top",                 "_Always on Top"},
         {"v_full_screen",                   "_Full Screen"},
    }
};

static struct lcrt_language_config ioptions = {
    .table_name = LCRT_IOPTIONS_TABLE,
    .members = LCRT_O_NUMBER,
    .config = {
         {"o_menuitem",                      "_Options"},
         {"o_session_options",               "_Session Options ..."},
         {"o_global_options",                "_Global Options ..."},
         {"o_auto_save_options",             "_Auto Save Options"},
         {"o_save_settings_now",             "Save Settings _Now"},
    }
};

static struct lcrt_language_config ihelp = {
    .table_name = LCRT_IHELP_TABLE,
    .members = LCRT_H_NUMBER,
    .config = {
         {"h_menuitem",                      "_Help"},
         {"h_help",                          "help"},
         {"h_home_page",                     "_Home Page"},
         {"h_check_update",                  "_Check update"},
         {"h_about",                         "_About"},
         {"h_missing_doc",                   "Missing help document!"},
         {"h_no_new_version",                "There is not new version!"},
         {"h_new_version",                   "There is a new verison:%s\nDownload page: %s"},
         {"h_check_update_err",              "Check update error: may be network is unreachable!"},
    }
};

static struct lcrt_language_config itoolbar = {
    .table_name = LCRT_ITOOLBAR_TABLE,
    .members = LCRT_B_NUMBER,
    .config = {
         {"b_connect",                       "Connect"},
         {"b_quick_connect",                 "Quick connect"},
         {"b_connect_on_tab",                "Conect in tab"},
         {"b_reconnect",                     "Reconnect current session"},
         {"b_disconnect",                    "Disconnect current session"},
         {"b_copy",                          "Copy"},
         {"b_paste",                         "Paste"},
         {"b_terminal",                      "Open a terminal"},
    }
};

static struct lcrt_language_config iconnect = {
    .table_name = LCRT_ICONNECT_TABLE,
    .members = LCRT_C_NUMBER,
    .config = {
         {"c_connect_in_tab",                "Connect in Tab"},
         {"c_connect",                       "Connect"},
         {"c_quick_connect",                 "Quick Connect"},
         {"c_rename",                        "Rename a session"},
         {"c_delete",                        "Delete a session"},
         {"c_find",                          "Find a session"},
         {"c_session_option",                "Session option"},
         {"c_session",                       "Sessions"},
         {"c_show_dialog_on_startup",        "_Show dialog on startup"},
         {"c_connect_button",                "Connect"},
         {"c_close_button",                  "Close"},
         {"c_warn_msg",                      "Are you sure you want to delete the selected item ?"},       
         {"c_err_msg",                       "There is no memory to create window"},
    }
};

static struct lcrt_language_config ilock = {
    .table_name = LCRT_ILOCK_TABLE,
    .members = LCRT_K_NUMBER,
    .config = {
         {"k_lock_title",                    "Lock Session"},
         {"k_unlock_title",                  "Unlock Session"},
         {"k_lock_tip",                      "Enter password for session and will be used\nwhen unlock the session."},
         {"k_unlock_tip",                    "Enter password to unlock the session."},
         {"k_password",                      "Password: "},
         {"k_repassword",                    "Retype password:"},
         {"k_cancel",                        "Cancel"},
         {"k_ok",                            "OK"},
         {"k_error",                         "Password do not match!"},
         {"k_err_msg",                       "There is no memory to create dialog"},
    }
};

static struct lcrt_language_config ilogin = {
    .table_name = LCRT_ILOGIN_TABLE,
    .members = LCRT_L_NUMBER,
    .config = {
         {"l_login",                            "Login"},
         {"l_info",                             "Input username & password:"},
         {"l_again",                            "Username or password error:"},
         {"l_username",                         "Username:"},
         {"l_password",                         "Password:"},
         {"l_remember",                         "Remember ?"},
         {"l_okbutton",                         "OK"},
         {"l_cancelbutton",                     "Cancel"},
         {"l_err_msg",                          "There is no memory to create dialog"},
    }
};

static struct lcrt_language_config ipopup = {
    .table_name = LCRT_IPOPUP_TABLE,
    .members = LCRT_P_NUMBER,
    .config = {
         {"p_rename",                           "Rena_me"},
         {"p_reconnect",                        "_Reconnect"},
         {"p_disconnect",                       "_Disconnect"},
         {"p_close",                            "_Close"},
         {"p_lock_session",                     "Loc_k Session ..."},
         {"p_unlock_session",                   "Unloc_k Session ..."},
         {"p_clone_session",                    "Clo_ne Session"},
         {"p_session_options",                  "Session _Options ..."},
    }
};

static struct lcrt_language_config iqconnect = {
    .table_name = LCRT_IQCONNECT_TABLE,
    .members = LCRT_Q_NUMBER,
    .config = {
         {"q_connect",                          "Quick connect"},
         {"q_session_option",                   "Session Option"},
         {"q_lb_protocol",                      "  Protocol:"},
         {"q_lb_hostname",                      "  Hostname:"},
         {"q_lb_username",                      "  Username:"},
         {"q_lb_firewall",                      "  Firewall:"},
         {"q_lb_port",                          "  Port: "},
         {"q_lb_frame",                         "Authentication"},
         {"q_cb_passwd",                        "Password"},
         {"q_cb_publickey",                     "PublicKey"},
         {"q_cb_keyboard",                      "KeyBoard Interactive"},
         {"q_cb_gssapi",                        "GSSAPI"},
         {"q_cb_rsa",                           "RSA"},
         {"q_cb_tis",                           "TIS"},
         {"q_bt_properties",                    "Properties..."},
         {"q_lb_default_command",               "  Command:"},
         {"q_cb_show_qconnect",                 "Show Quick connect on startup"},
         {"q_cb_save_session",                  "Save Session"},
         {"q_cb_open_in_a_tab",                 "Open in a tab"},
         {"q_bt_connect",                       "Connect"},
         {"q_bt_ok",                            "OK"},
         {"q_bt_cancel",                        "Cancel"},
         {"q_err_msg",                          "There is no memory to create window"},
         {"q_sport",                            "  Part:"},
         {"q_sbaud_rate",                       "  Baud rate:"},
         {"q_sdata_bits",                       "  Data bits:"},
         {"q_sparity",                          "  Parity:"},
         {"q_sstop_bits",                       "  Stop bits:"},
         {"q_sflow_control",                    "Flow control"},
    }
};

static struct lcrt_language_config irename = {
    .table_name = LCRT_IRENAME_TABLE,
    .members = LCRT_R_NUMBER,
    .config = {
         {"r_title",                            "Rename"},
         {"r_tip",                              "Enter new name:"},
         {"r_cancel",                           "Cancel"},
         {"r_ok",                               "OK"},
         {"r_error",                            "There has a same one, try an another!"},
         {"r_err_msg",                          "There is no memory to create dialog"},
    }
};

static struct lcrt_language_config istatus = {
    .table_name = LCRT_ISTATUS_TABLE,
    .members = LCRT_S_NUMBER,
    .config = {
         {"s_tooltip",                          "Linux Remote login Tool"},
         {"s_show_status",                      "Main Window"},
         {"w_quit",                             "Quit"},
         {"w_window_exit",                      "There are %d session connected, do you want to quit ?"},
         {"w_session_disconnect",               "Do you wish to disconnect from %s ?"},
    }
};

static struct lcrt_language_config isettings = {
    .table_name = LCRT_ISETTINGS_TABLE,
    .members = LCRT_T_NUMBER,
    .config = {
        {"dialog_settings",                     "Global options"},
        {"g_nb_general",                        "General"},
        {"g_lb_language_support",               "<b>Language support</b>"},
        {"g_lb_language_support_note",          "Select language support, and will be effect on\nnext time."},
        {"g_lb_current_language",               "Current language:"},
        {"g_lb_dialog",                         "<b>Dialog</b>"},
        {"g_rb_connect",                        "Show connect dialog on startup"},
        {"g_rb_quick_connect",                  "Show quick connect dialog on startup"},
        {"g_rb_none",                           "None"},
        {"g_lb_view",                           "View"},
        {"g_cb_show_toolbar",                   "Show toolbar"},
        {"g_cb_show_statusbar",                 "Show statusbar"},
        {"k_nb_keybindings",                    "Keybindings"},
        {"k_lb_keyboard_shortcut",              "<b>Keyboard shortcuts</b>"},
        {"k_lb_keyboard_shortcuts_note",        "Here you can change keyboard shortcuts for\nvarious actions."},
        {"k_cb_enable_f10",                     "Enable the menu shortcut key(F10 by default)"},
        {"t_nb_terminal",                       "Terminal"},
        {"t_lb_terminal_settings",              "<b>Terminal settings</b>"},
        {"t_lb_terminal_note",                  "These settings for the virtual terminal emulator\nwidget(VTE)only apply if the VTE library could be \nloaded."},
        {"t_lb_font",                           "Terminal font:"},
        {"t_lb_foregroud_color",                "Font color:"},
        {"t_lb_background_color",               "Background color:"},
        {"t_lb_background_image",               "Background image:"},
        {"t_lb_transparent",                    "Transparent "},
        {"t_tp_transparent",                    "Notice: only transparent is set to 0, the background image can be displayed."},
        {"t_lb_scrollback_lines",               "Scrollback lines:"},
        {"t_cb_bell",                           "Terminal bell"},
        {"t_bt_restore_defaul",                 "Reset"},
        {"a_nb_application",                    "Application"},
        {"a_lb_application",                    "<b>Applications</b>"},
        {"a_lb_application_note",               "As you known, lcrt will invoke local application\nwhen you open a connect such as ssh, so in here,\nyou can config it."},
        {"a_lb_shell",                          "shell:"},
        {"a_lb_ssh",                            "ssh:"},
        {"a_lb_telnet",                         "telnet:"},
        {"lb_applybutton",                      "Apply"},
        {"lb_cancelbutton",                     "Cancel"},
        {"lb_okbutton",                         "OK"},
        {"error_msg",                           "There is no memory to create dialog"},
    }
};

static struct lcrt_language_config iaccels = {
    .table_name = LCRT_IACCELS_TABLE,
    .members = LCRT_KB_NUMBER,
    .config = {
        {"kb_action",                           "Action"},
        {"kb_shortcut_key",                     "Shortcut key"},
        {"kb_file",                             "File"},
        {"kb_f_connect",                        "Connect"},
        {"kb_f_quick_connect",                  "Quick Connect"},
        {"kb_f_connect_in_tab",                 "Connect In Tab"},
        {"kb_f_reconnect",                      "Reconnect"},
        {"kb_edit",                             "Edit"},
        {"kb_e_copy",                           "Copy"},
        {"kb_e_paste",                          "Paste"},
        {"kb_session",                          "Session"},
        {"kb_s_clone",                          "Clone"},
        {"kb_s_close",                          "Close"},
        {"kb_s_move_next",                      "Move To Next"},
        {"kb_s_move_prev",                      "Move To Prev"},
        {"kb_s_switch_next",                    "Switch To Next"},
        {"kb_s_switch_prev",                    "Switch To Prev"},
        {"kb_warn_msg",                         "The shortcut key \"%s\" is already bound to the \"%s\" action"},
        {"kb_err_msg",                          "There is no memory to create dialog"}
    }
};
static struct lcrt_language_config iterminal = {
    .table_name = LCRT_ITERMINAL_TABLE,
    .members = LCRT_TM_NUMBER,
    .config = {
        {"tm_connection_timed_out",             "Connection timed out!"},
        {"tm_connection_refused",               "Connection refused!"},
        {"tm_connection_failed",                "Username or password error,exit!"},
        {"tm_connection_prog_not_found",        "Can not find the programe \"%s\",please install it!"},
        {"tm_connection_not_support",           "Current version not support!"}
    }
};
struct lcrt_language en_US = {
    .language_name = "English",
    .db_name       = "en_US.db",
    .language = {
        &ifile,
        &iedit,
        &iview,
        &ioptions,
        &ihelp,
        &itoolbar,
        &iconnect,
        &ilock,
        &ilogin,
        &ipopup,
        &iqconnect,
        &irename,
        &istatus,
        &isettings,
        &iaccels,
        &iterminal,
    },
};
