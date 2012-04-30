/*
 * Copyright (c) 2009-~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Sun 20 Feb 2011 01:54:10 PM CST
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
#include "imkdir.h"

#include "language.h"

static struct lcrt_language_config ifile = {
    .table_name = LCRT_IFILE_TABLE,
    .members = LCRT_F_NUMBER,
    .config = {
         {"f_menuitem",                      "文件(_F)"},
         {"f_connect",                       "连接(_C) ..."},
         {"f_quick_connect",                 "快速连接(_Q) ..."},
         {"f_connect_in_tab",                "在标签中打开(_B)"},
         {"f_reconnect",                     "重新连接(_R)"},
         {"f_reconnect_all",                 "全部重新连接(_A)"},
         {"f_disconnect",                    "断开(_D)"},
         {"f_disconnect_all",                "全部断开(_O)"},
         {"f_clone_session",                 "克隆会话(_N)"},
         {"f_lock_session",                  "锁定会话(_K) ..."},
         {"f_print",                         "打印(_P)"},
         {"f_print_setup",                   "打印设置(_U) ..."},
         {"f_log_session",                   "记录会话(_L)"},
         {"f_recent_session",                "最近访问"},
         {"f_exit",                          "退出(_E)"},
    }
};

static struct lcrt_language_config iedit = {
    .table_name = LCRT_IEDIT_TABLE,
    .members = LCRT_E_NUMBER,
    .config = {
         {"e_menuitem",                      "编辑(_E)"},
         {"e_copy",                          "复制(_C)"},
         {"e_paste",                         "粘贴(_P)"},
         {"e_copy_and_paste",                "复制并粘贴(_O)"},
         {"e_select_all",                    "全选(_A)"},
         {"e_find",                          "查找(_F) ..."},
         {"e_clear_scrollback",              "清除回滚(_B)"},
         {"e_clear_screen",                  "清除屏幕(_S)"},
         {"e_clear_screen_and_scrollback",   "清除屏幕和回滚(_K)"},
         {"e_reset",                         "重置(_R)"},
         {"e_show_menubar",                  "显示主菜单(_M)"},
    }
};

static struct lcrt_language_config iview = {
    .table_name = LCRT_IVIEW_TABLE,
    .members = LCRT_V_NUMBER,
    .config = {
         {"v_menuitem",                      "查看(_V)"},
         {"v_menubar",                       "菜单栏(_M)"},
         {"v_toolbar",                       "工具栏(_T)"},
         {"v_status_bar",                    "状态栏(_S)"},
         {"v_always_on_top",                 "总在最上(_A)"},
         {"v_full_screen",                   "全屏(_F)"},
    }
};

static struct lcrt_language_config ioptions = {
    .table_name = LCRT_IOPTIONS_TABLE,
    .members = LCRT_O_NUMBER,
    .config = {
         {"o_menuitem",                      "选项(_O)"},
         {"o_session_options",               "会话选项(_S) ..."},
         {"o_global_options",                "全局选项(_G) ..."},
         {"o_auto_save_options",             "自动保存选项(_A)"},
         {"o_save_settings_now",             "立刻保存设置(_N)"},
    }
};

static struct lcrt_language_config ihelp = {
    .table_name = LCRT_IHELP_TABLE,
    .members = LCRT_H_NUMBER,
    .config = {
         {"h_menuitem",                      "帮助(_H)"},
         {"h_help",                          "帮助主题"},
         {"h_home_page",                     "主页(_H)"},
         {"h_check_update",                  "检查更新(_D)"},
         {"h_about",                         "关于(_A)"},
         {"h_missing_doc",                   "没有找到帮助文档!"},
         {"h_no_new_version",                "没有新版本!"},
         {"h_new_version",                   "检查到新版本:%s\n下载页面: %s"},
         {"h_check_update_err",              "检查新版本出错，可能由于网络不可达!"},
    }
};

static struct lcrt_language_config itoolbar = {
    .table_name = LCRT_ITOOLBAR_TABLE,
    .members = LCRT_B_NUMBER,
    .config = {
         {"b_connect",                       "连接"},
         {"b_quick_connect",                 "快速连接"},
         {"b_connect_on_tab",                "在标签中连接"},
         {"b_reconnect",                     "重新连接"},
         {"b_disconnect",                    "断开连接"},
         {"b_copy",                          "复制"},
         {"b_paste",                         "粘贴"},
         {"b_terminal",                      "打开终端"},
    }
};

static struct lcrt_language_config iconnect = {
    .table_name = LCRT_ICONNECT_TABLE,
    .members = LCRT_C_NUMBER,
    .config = {
         {"c_connect_in_tab",                "在标签中连接"},
         {"c_connect",                       "连接"},
         {"c_quick_connect",                 "快速连接"},
         {"c_rename",                        "重命名会话"},
         {"c_delete",                        "删除会话"},
         {"c_find",                          "查找会话"},
         {"c_session_option",                "会话选项"},
         {"c_mkdir",                         "新建目录"},
         {"c_session",                       "会话"},
         {"c_show_dialog_on_startup",        "启动时显示(_S)"},
         {"c_connect_button",                "连接(_C)"},
         {"c_close_button",                  "关闭"},
         {"c_warn_msg",                      "确定删除选中会话?"},       
         {"c_err_msg",                       "创建窗口失败!"},
    }
};

static struct lcrt_language_config ilock = {
    .table_name = LCRT_ILOCK_TABLE,
    .members = LCRT_K_NUMBER,
    .config = {
         {"k_lock_title",                    "锁定会话"},
         {"k_unlock_title",                  "解锁会话"},
         {"k_lock_tip",                      "输入锁定密码，该密码会在\n解锁会话的时候用到."},
         {"k_unlock_tip",                    "输入解锁密码."},
         {"k_password",                      "密码: "},
         {"k_repassword",                    "再次输入密码:"},
         {"k_cancel",                        "关闭"},
         {"k_ok",                            "确定"},
         {"k_error",                         "密码不匹配!"},
         {"k_err_msg",                       "无法创建窗口!"},
    }
};

static struct lcrt_language_config ilogin = {
    .table_name = LCRT_ILOGIN_TABLE,
    .members = LCRT_L_NUMBER,
    .config = {
         {"l_login",                            "登录"},
         {"l_info",                             "输入用户名和密码:"},
         {"l_again",                            "用户名或密码错误:"},
         {"l_username",                         "用户名:"},
         {"l_password",                         "密码:"},
         {"l_remember",                         "记住密码?"},
         {"l_okbutton",                         "确定"},
         {"l_cancelbutton",                     "关闭"},
         {"l_err_msg",                          "无法创建窗口!"},
    }
};

static struct lcrt_language_config ipopup = {
    .table_name = LCRT_IPOPUP_TABLE,
    .members = LCRT_P_NUMBER,
    .config = {
         {"p_rename",                           "重命名(_M)"},
         {"p_reconnect",                        "重新连接(_R)"},
         {"p_disconnect",                       "断开(_D)"},
         {"p_close",                            "关闭(_C)"},
         {"p_lock_session",                     "锁定会话(_K) ..."},
         {"p_unlock_session",                   "解锁会话(_K) ..."},
         {"p_clone_session",                    "克隆会话(_N)"},
         {"p_session_options",                  "会话选项(_O) ..."},
    }
};

static struct lcrt_language_config iqconnect = {
    .table_name = LCRT_IQCONNECT_TABLE,
    .members = LCRT_Q_NUMBER,
    .config = {
         {"q_connect",                          "快速连接"},
         {"q_session_option",                   "会话选项"},
         {"q_lb_protocol",                      "  协议:"},
         {"q_lb_hostname",                      "  主机名:"},
         {"q_lb_username",                      "  用户名:"},
         {"q_lb_firewall",                      "  防火墙:"},
         {"q_lb_port",                          "  端口: "},
         {"q_lb_frame",                         "认证"},
         {"q_cb_passwd",                        "Password"},
         {"q_cb_publickey",                     "PublicKey"},
         {"q_cb_keyboard",                      "KeyBoard Interactive"},
         {"q_cb_gssapi",                        "GSSAPI"},
         {"q_cb_rsa",                           "RSA"},
         {"q_cb_tis",                           "TIS"},
         {"q_bt_properties",                    "属性 ..."},
         {"q_lb_default_command",               "  默认命令:"},
         {"q_cb_show_qconnect",                 "在启动时显示快速连接"},
         {"q_cb_save_session",                  "保存会话"},
         {"q_cb_open_in_a_tab",                 "在新标签中打开"},
         {"q_bt_connect",                       "连接"},
         {"q_bt_ok",                            "确定"},
         {"q_bt_cancel",                        "关闭"},
         {"q_err_msg",                          "无法创建窗口!"},
         {"q_sport",                            "  端口:"},
         {"q_sbaud_rate",                       "  波特率:"},
         {"q_sdata_bits",                       "  数据位:"},
         {"q_sparity",                          "  奇偶校验位:"},
         {"q_sstop_bits",                       "  停止位:"},
         {"q_sflow_control",                    "流控制"},
         {"q_shell",                            "  终端:"},
         {"q_ptitle",                           "密码属性"},
         {"q_pprompt",                          "修改密码"},
         {"q_pstore_password",                  "  输入密码:"},
         {"q_pconfirm_password",                "  确认密码:"},
         {"p_pcancel",                          "取消"},
         {"p_pok",                              "确定"},
         {"p_ppw_unmatch",                      "密码不匹配!"},
         {"q_snot_found",                       "未找到可执行程序!\n"},
    }
};

static struct lcrt_language_config irename = {
    .table_name = LCRT_IRENAME_TABLE,
    .members = LCRT_R_NUMBER,
    .config = {
         {"r_title",                            "重命名"},
         {"r_tip",                              "输入新名称:"},
         {"r_cancel",                           "关闭"},
         {"r_ok",                               "确定"},
         {"r_error",                            "已经有相同名称的会话，请重新输入!"},
         {"r_err_msg",                          "无法创建窗口!"},
    }
};

static struct lcrt_language_config imkdir = {
    .table_name = LCRT_IMKDIR_TABLE,
    .members = LCRT_M_NUMBER,
    .config = {
         {"m_title",                            "新建目录"},
         {"m_tip",                              "输入新目录名:"},
         {"r_cancel",                           "关闭"},
         {"m_ok",                               "确定"},
         {"m_error",                            "已经有相同名称的会话，请重新输入!"},
         {"m_err_msg",                          "无法创建窗口!"},
    }
};

static struct lcrt_language_config istatus = {
    .table_name = LCRT_ISTATUS_TABLE,
    .members = LCRT_S_NUMBER,
    .config = {
         {"s_tooltip",                          "Linux下的远程登录工具"},
         {"s_show_status",                      "显示主窗口"},
         {"w_quit",                             "退出"},
         {"w_window_exit",                      "当前有%d个会话已经连接，是否退出?"},
         {"w_session_disconnect",               "确定从%s 断开连接?"},
    }
};
static struct lcrt_language_config isettings = {
    .table_name = LCRT_ISETTINGS_TABLE,
    .members = LCRT_T_NUMBER,
    .config = {
        {"dialog_settings",                    "全局选项"},
        {"g_nb_general",                       "常规"},
        {"g_lb_language_support",              "<b>语言支持</b>"},
        {"g_lb_language_support_note",         "选择软件界面的语言显示，设置会在重启软件后生效."},
        {"g_lb_current_language",              "当前语言:"},
        {"g_lb_dialog",                        "<b>对话框</b>"},
        {"g_rb_connect",                       "启动时显示连接对话框"},
        {"g_rb_quick_connect",                 "启动时显示快速连接对话框"},
        {"g_rb_none",                          "不显示"},
        {"g_lb_view",                          "显示"},
        {"g_cb_show_toolbar",                  "显示工具栏"},
        {"g_cb_show_statusbar",                "显示状态栏"},
        {"k_nb_keybindings",                   "快捷键"},
        {"k_lb_keyboard_shortcut",             "<b>快捷键</b>"},
        {"k_lb_keyboard_shortcuts_note",       "设置各种操作的快捷键，设置会在重启软件后生效."},
        {"k_cb_enable_f10",                     "启用菜单快捷键(默认是 F10)"},
        {"t_nb_terminal",                      "虚拟终端"},
        {"t_lb_terminal_settings",             "<b>虚拟终端设置</b>"},
        {"t_lb_terminal_note",                 "以下是对虚拟终端(VTE)的设置。只有VTE库被载入时\n这些设置才会生效."},
        {"t_lb_font",                          "终端字体:"},
        {"t_lb_foregroud_color",               "字体颜色:"},
        {"t_lb_background_color",              "背景颜色:"},
        {"t_lb_background_image",              "背景图片:"},
        {"t_lb_transparent",                   "透明度: "},
        {"t_tp_transparent",                   "只有透明度被设置为0的时候，背景图片才会显示."},
        {"t_lb_scrollback_lines",              "回滚行数:"},
        {"t_cb_bell",                          "终端响铃"},
        {"t_bt_restore_defaul",                "恢复默认"},
        {"a_nb_application",                   "应用软件"},
        {"a_lb_application",                   "<b>应用软件设置</b>"},
        {"a_lb_application_note",              "LCRT在运行时会调用本地的应用程序以建立\n连接，在这里你可以设置被调用的应用软件."},
        {"a_lb_shell",                         "shell:"},
        {"a_lb_ssh",                           "ssh:"},
        {"a_lb_telnet",                        "telnet:"},
        {"lb_applybutton",                     "应用"},
        {"lb_cancelbutton",                    "取消"},
        {"lb_okbutton",                        "确定"},
        {"error_msg",                          "创建对话框失败!"},
    }
};

static struct lcrt_language_config iaccels = {
    .table_name = LCRT_IACCELS_TABLE,
    .members = LCRT_KB_NUMBER,
    .config = {
        {"kb_action",                           "行为"},
        {"kb_shortcut_key",                     "快捷键"},
        {"kb_file",                             "文件"},
        {"kb_f_connect",                        "连接"},
        {"kb_f_quick_connect",                  "快速连接"},
        {"kb_f_connect_in_tab",                 "在标签中打开"},
        {"kb_f_reconnect",                      "重新连接"},
        {"kb_edit",                             "编辑"},
        {"kb_e_copy",                           "复制"},
        {"kb_e_paste",                          "粘贴"},
        {"kb_session",                          "会话"},
        {"kb_s_clone",                          "克隆"},
        {"kb_s_close",                          "关闭"},
        {"kb_s_move_next",                      "向前移动"},
        {"kb_s_move_prev",                      "向后移动"},
        {"kb_s_switch_next",                    "切换到前一个"},
        {"kb_s_switch_prev",                    "切换到后一个"},
        {"kb_warn_msg",                         "快捷键\"%s\"已经被绑定到\"%s\"上，请重新选择!"},
        {"kb_err_msg",                          "创建对话框失败!"}
    }
};
static struct lcrt_language_config iterminal = {
    .table_name = LCRT_ITERMINAL_TABLE,
    .members = LCRT_TM_NUMBER,
    .config = {
        {"tm_connection_timed_out",             "连接超时!"},
        {"tm_connection_refused",               "连接失败，可能远端服务没有开启!"},
        {"tm_connection_failed",                "用户名或密码错误!"},
        {"tm_connection_prog_not_found",        "没有找到程序\"%s\", 请安装！"},
        {"tm_connection_not_support",           "当前版本不支持！"},
        {"tm_serial_error",                     "无法打开 \"%s\",%s."},
    }
};

struct lcrt_language zh_CN = {
    .language_name = "简体中文",
    .db_name       = "zh_CN.db",
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
        &imkdir,
        &istatus,
        &isettings,
        &iaccels,
        &iterminal
    },
};

