/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : Manfred Mueller<quax@lin2go.com>
 * Created Time: Tue 29 Mar 2011 07:19:45 PM CEST
 * File Name   : de_DE.c
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
         {"f_menuitem",                      "_Datei"},
         {"f_connect",                       "_Verbinden ..."},
         {"f_quick_connect",                 "_Schnell verbinden ..."},
         {"f_connect_in_tab",                "In Ta_b verbinden"},
         {"f_reconnect",                     "_Erneut verbinden"},
         {"f_reconnect_all",                 "_Alle erneut verbinden"},
         {"f_disconnect",                    "_Trennen"},
         {"f_disconnect_all",                "Alle T_rennen"},
         {"f_clone_session",                 "Sitzung Klo_nen"},
         {"f_lock_session",                  "Sitzung _sperren ..."},
         {"f_print",                         "_Drucken"},
         {"f_print_setup",                   "Ein_stellungen drucken ..."},
         {"f_log_session",                   "_Sitzungsprotokoll"},
         {"f_recent_session",                "Letzte Sitzung"},
         {"f_exit",                          "E_nde"},
    }
};

static struct lcrt_language_config iedit = {
    .table_name = LCRT_IEDIT_TABLE,
    .members = LCRT_E_NUMBER,
    .config = {
         {"e_menuitem",                      "_Bearbeiten"},
         {"e_copy",                          "_Kopieren"},
         {"e_paste",                         "_Einfügen"},
         {"e_copy_and_paste",                "K_opieren und Einfügen"},
         {"e_select_all",                    "_Alle auswählen"},
         {"e_find",                          "_Finden ..."},
         {"e_clear_scrollback",              "_Verlauf löschen"},
         {"e_clear_screen",                  "_Bildschirm löschen"},
         {"e_clear_screen_and_scrollback",   "Bildschirm und _Verlauf _löschen"},
         {"e_reset",                         "Ver_werfen"},
         {"e_show_menubar",                  "Menü anzeigen"},
    }
};

static struct lcrt_language_config iview = {
    .table_name = LCRT_IVIEW_TABLE,
    .members = LCRT_V_NUMBER,
    .config = {
         {"v_menuitem",                      "_Anzeige"},
         {"v_menubar",                       "_Menü"},
         {"v_toolbar",                       "_Werkzeugleiste"},
         {"v_status_bar",                    "_Statusleiste"},
         {"v_always_on_top",                 "_Immer oben"},
         {"v_full_screen",                   "_Vollbild"},
    }
};

static struct lcrt_language_config ioptions = {
    .table_name = LCRT_IOPTIONS_TABLE,
    .members = LCRT_O_NUMBER,
    .config = {
         {"o_menuitem",                      "_Optionen"},
         {"o_session_options",               "_Sitzungs-Optionen ..."},
         {"o_global_options",                "_Globale Optionen ..."},
         {"o_auto_save_options",             "Optionen _automatisch speichern"},
         {"o_save_settings_now",             "Einstellungen _jetzt speichern"},
    }
};

static struct lcrt_language_config ihelp = {
    .table_name = LCRT_IHELP_TABLE,
    .members = LCRT_H_NUMBER,
    .config = {
         {"h_menuitem",                      "_Hilfe"},
         {"h_help",                          "Hilfe"},
         {"h_home_page",                     "_Webseite"},
         {"h_check_update",                  "_Aktualisierung"},
         {"h_about",                         "_Über"},
         {"h_missing_doc",                   "Hilfedokument fehlt!"},
         {"h_no_new_version",                "Ihre Version ist aktuell!"},
         {"h_new_version",                   "Neuere Version %s gefunden!\nHerunterladen von: %s"},
         {"h_check_update_err",              "Aktualisierungsfehler: Das Netzwerk ist nicht erreichbar!"},
    }
};

static struct lcrt_language_config itoolbar = {
    .table_name = LCRT_ITOOLBAR_TABLE,
    .members = LCRT_B_NUMBER,
    .config = {
         {"b_connect",                       "Verbinden"},
         {"b_quick_connect",                 "Schnell verbinden"},
         {"b_connect_on_tab",                "In Tab verbinden"},
         {"b_reconnect",                     "Aktuelle Sitzung erneut verbinden"},
         {"b_disconnect",                    "Aktuelle Sitzung trennen"},
         {"b_copy",                          "Kopieren"},
         {"b_paste",                         "Einfügen"},
         {"b_terminal",                      "Ein Terminal öffnen"},
    }
};

static struct lcrt_language_config iconnect = {
    .table_name = LCRT_ICONNECT_TABLE,
    .members = LCRT_C_NUMBER,
    .config = {
         {"c_connect_in_tab",                "In Tab verbinden"},
         {"c_connect",                       "Verbinden"},
         {"c_quick_connect",                 "Schnell verbinden"},
         {"c_rename",                        "Eine Sitzung umbenennen"},
         {"c_delete",                        "Eine Sitzung löschen"},
         {"c_find",                          "Eine Sitzung finden"},
         {"c_session_option",                "Sitzungs-_Optionen"},
         {"c_session",                       "Sitzungen"},
         {"c_show_dialog_on_startup",        "_Startdialog anzeigen"},
         {"c_connect_button",                "Verbinden"},
         {"c_close_button",                  "Schliessen"},
         {"c_warn_msg",                      "Wollen Sie das Objekt wirklich löschen ?"},       
         {"c_err_msg",                       "Keine Daten zum Speichern vorhanden"},
    }
};

static struct lcrt_language_config ilock = {
    .table_name = LCRT_ILOCK_TABLE,
    .members = LCRT_K_NUMBER,
    .config = {
         {"k_lock_title",                    "Sitzung sperren"},
         {"k_unlock_title",                  "Sitzung entsperren"},
         {"k_lock_tip",                      "Das eingegebene Passwort wird zum \nEntsperren der Sitzung verwendet."},
         {"k_unlock_tip",                    "Bitte ein Passwort zum Entsperren eingeben."},
         {"k_password",                      "Passwort: "},
         {"k_repassword",                    "Passwort wiederholen:"},
         {"k_cancel",                        "Abbrechen"},
         {"k_ok",                            "OK"},
         {"k_error",                         "Passworte stimmen nicht überein!"},
         {"k_err_msg",                       "Keine Daten zum Speichern vorhanden"},
    }
};

static struct lcrt_language_config ilogin = {
    .table_name = LCRT_ILOGIN_TABLE,
    .members = LCRT_L_NUMBER,
    .config = {
         {"l_login",                            "Anmeldung"},
         {"l_info",                             "Benutzer & Passwort angeben:"},
         {"l_again",                            "Benutzer- oder Passwort-Fehler:"},
         {"l_username",                         "Benutzer:"},
         {"l_password",                         "Passwort:"},
         {"l_remember",                         "Merken ?"},
         {"l_okbutton",                         "OK"},
         {"l_cancelbutton",                     "Abbrechen"},
         {"l_err_msg",                          "Keine Daten zum Speichern vorhanden"},
    }
};

static struct lcrt_language_config ipopup = {
    .table_name = LCRT_IPOPUP_TABLE,
    .members = LCRT_P_NUMBER,
    .config = {
         {"p_rename",                           "U_mbenennen"},
         {"p_reconnect",                        "_Erneut verbinden"},
         {"p_disconnect",                       "_Trennen"},
         {"p_close",                            "_Schliessen"},
         {"p_lock_session",                     "Sitzung s_perren ..."},
         {"p_unlock_session",                   "Sitzung entspe_rren ..."},
         {"p_clone_session",                    "Sitzung _kopieren"},
         {"p_session_options",                  "Sitzungs-_Optionen ..."},
    }
};

static struct lcrt_language_config iqconnect = {
    .table_name = LCRT_IQCONNECT_TABLE,
    .members = LCRT_Q_NUMBER,
    .config = {
         {"q_connect",                          "Schnell verbinden"},
         {"q_session_option",                   "_Sitzungs-Optionen"},
         {"q_lb_protocol",                      "  Protokoll:"},
         {"q_lb_hostname",                      "  Rechner:"},
         {"q_lb_username",                      "  Benutzer:"},
         {"q_lb_firewall",                      "  Firewall:"},
         {"q_lb_port",                          "  Port: "},
         {"q_lb_frame",                         "Authentifizierung"},
         {"q_cb_passwd",                        "Passwort"},
         {"q_cb_publickey",                     "PublicKey"},
         {"q_cb_keyboard",                      "KeyBoard Interactive"},
         {"q_cb_gssapi",                        "GSSAPI"},
         {"q_cb_rsa",                           "RSA"},
         {"q_cb_tis",                           "TIS"},
         {"q_bt_properties",                    "Eigenschaften..."},
         {"q_lb_default_command",               "  Commando:"},
         {"q_cb_show_qconnect",                 "Schnellverbindung beim Start anzeigen"},
         {"q_cb_save_session",                  "Sitzung speichern"},
         {"q_cb_open_in_a_tab",                 "In Tab öffnen"},
         {"q_bt_connect",                       "Verbinden"},
         {"q_bt_ok",                            "OK"},
         {"q_bt_cancel",                        "Abbrechen"},
         {"q_err_msg",                          "Keine Daten zum Speichern vorhanden"},
         {"q_sport",                            "  Havn:"},
         {"q_sbaud_rate",                       "  Baud rate:"},
         {"q_sdata_bits",                       "  Data bits:"},
         {"q_sparity",                          "  Paritet:"},
         {"q_sstop_bits",                       "  Stop bits:"},
         {"q_sflow_control",                    "flow-kontrol"},
    }
};

static struct lcrt_language_config irename = {
    .table_name = LCRT_IRENAME_TABLE,
    .members = LCRT_R_NUMBER,
    .config = {
         {"r_title",                            "Umbenennen"},
         {"r_tip",                              "Neuen Namen angeben:"},
         {"r_cancel",                           "Abbrechen"},
         {"r_ok",                               "OK"},
         {"r_error",                            "Angegebener Name bereits vorhanden!"},
         {"r_err_msg",                          "Keine Daten zum Speichern vorhanden"},
    }
};

static struct lcrt_language_config istatus = {
    .table_name = LCRT_ISTATUS_TABLE,
    .members = LCRT_S_NUMBER,
    .config = {
         {"s_tooltip",                          "Linux -Werkzeug für entfernte Anmeldung"},
         {"s_show_status",                      "Haupt-Fenster"},
         {"w_quit",                             "Beenden"},
         {"w_window_exit",                      "%d Sitzung(en) aktiv, wollen Sie wirklich trennen ?"},
         {"w_session_disconnect",               "Verbindung zu %s trennen?"},
    }
};

static struct lcrt_language_config isettings = {
    .table_name = LCRT_ISETTINGS_TABLE,
    .members = LCRT_T_NUMBER,
    .config = {
        {"dialog_settings",                     "Globale Optionen"},
        {"g_nb_general",                        "Allgemein"},
        {"g_lb_language_support",               "<b>Sprachunterstützung</b>"},
        {"g_lb_language_support_note",          "Zum Aktivieren der neuen Sprache\nbitte die Anwendung neu starten."},
        {"g_lb_current_language",               "Aktuelle Sprache:"},
        {"g_lb_dialog",                         "<b>Dialog</b>"},
        {"g_rb_connect",                        "Verbindungsdialog beim Start anzeigen"},
        {"g_rb_quick_connect",                  "Schnellverbindungsdialog beim Start anzeigen"},
        {"g_rb_none",                           "Kein"},
        {"g_lb_view",                           "Anzeige"},
        {"g_cb_show_toolbar",                   "Werkzeugleiste anzeigen"},
        {"g_cb_show_statusbar",                 "Statusleiste anzeigen"},
        {"k_nb_keybindings",                    "Tastenbelegung"},
        {"k_lb_keyboard_shortcut",              "<b>Tastatur-Schnellbefehle</b>"},
        {"k_lb_keyboard_shortcuts_note",        "Hier können Sie Tasten mit Befehlen belegen."},
        {"k_cb_enable_f10",                     "Aktivieren Sie das Menü Tastenkombination(F10 \nstandardmäßig)"},
        {"t_nb_terminal",                       "Terminal"},
        {"t_lb_terminal_settings",              "<b>Terminal-Einstellungen</b>"},
        {"t_lb_terminal_note",                  "Die Einstellungen für den virtuellen Terminal-\nEmulator gelten nur, falls die VTE-Bibliothek\ngeladen werden kann."},
        {"t_lb_font",                           "Terminal-Schrift:"},
        {"t_lb_foregroud_color",                "Schrift-Farbe:"},
        {"t_lb_background_color",               "Hintergrund-Farbe:"},
        {"t_lb_background_image",               "Hintergrund-Bild:"},
        {"t_lb_transparent",                    "Transparent "},
        {"t_tp_transparent",                    "HINWEIS: Zum Anzeigen des Bildes muss die Transparenz auf 0 gestellt sein."},
        {"t_lb_scrollback_lines",               "Zu rollende Zeilen:"},
        {"t_cb_bell",                           "Terminal-Glocke"},
        {"t_bt_restore_defaul",                 "Zurücksetzen"},
        {"a_nb_application",                    "Anwendung"},
        {"a_lb_application",                    "<b>Anwendungen</b>"},
        {"a_lb_application_note",               "LCRT kann eine vorher hier konfigurierte lokale\nAnwendung starten, sobald sie eine Verbindung\nherstellen."},
        {"a_lb_shell",                          "shell:"},
        {"a_lb_ssh",                            "ssh:"},
        {"a_lb_telnet",                         "telnet:"},
        {"lb_applybutton",                      "Anwenden"},
        {"lb_cancelbutton",                     "Abbrechen"},
        {"lb_okbutton",                         "OK"},
        {"error_msg",                           "Keine Daten zum Speichern vorhanden"},
    }
};

static struct lcrt_language_config iaccels = {
    .table_name = LCRT_IACCELS_TABLE,
    .members = LCRT_KB_NUMBER,
    .config = {
        {"kb_action",                           "Aktion"},
        {"kb_shortcut_key",                     "Befehl"},
        {"kb_file",                             "Datei"},
        {"kb_f_connect",                        "Verbinden"},
        {"kb_f_quick_connect",                  "Schnell verbinden"},
        {"kb_f_connect_in_tab",                 "In Tab verbinden"},
        {"kb_f_reconnect",                      "Erneut verbinden"},
        {"kb_edit",                             "Bearbeiten"},
        {"kb_e_copy",                           "Kopieren"},
        {"kb_e_paste",                          "Einfügen"},
        {"kb_session",                          "Sitzung"},
        {"kb_s_clone",                          "Kopieren"},
        {"kb_s_close",                          "Schliessen"},
        {"kb_s_move_next",                      "Gehe zum Nächsten"},
        {"kb_s_move_prev",                      "Gehe zum Vorherigen"},
        {"kb_s_switch_next",                    "Zum Nächsten umschalten"},
        {"kb_s_switch_prev",                    "Zum Vorherigen umschalten"},
        {"kb_warn_msg",                         "Die Taste \"%s\" ist bereits mit Befehl \"%s\" belegt"},
        {"kb_err_msg",                          "Keine Daten zum Speichern vorhanden"}
    }
};
static struct lcrt_language_config iterminal = {
    .table_name = LCRT_ITERMINAL_TABLE,
    .members = LCRT_TM_NUMBER,
    .config = {
        {"tm_connection_timed_out",             "Zeitüberschreitung!"},
        {"tm_connection_refused",               "Verbindungsaufnahme verweigert!"},
        {"tm_connection_failed",                "Benutzer- oder Passwort-Fehler, ABBRUCH!"},
        {"tm_connection_prog_not_found",        "Können Sie nicht das programe \"%s\", installieren Sie diese!"},
        {"tm_connection_not_support",           "Aktuelle Version nicht unterstützt!"}
    }
};
struct lcrt_language de_DE = {
    .language_name = "Deutsch",
    .db_name       = "de_DE.db",
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
