/*
 * Copyright (c) 2010-1011 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author      : NiuTao<niutao0602@gmail.com>
 * Created Time: Sat 06 Aug 2011 10:55:20 PM CST
 * File Name   : serial.c
 *
 * Description:  
 */
#define __LCRT_DEBUG__
#include <stdio.h>
#include <gtk/gtk.h>
#include <vte/vte.h>
#include "iwindow.h"
#include "iterminal.h"
#include "iqconnect.h"
#include "user.h"
#include "serial.h"
#include "debug.h"

static void lcrt_serial_contents_changed(struct lcrt_terminal *lterminal)
{
    VteTerminal *vteterminal = lterminal->terminal;
    struct lcrt_window *lwindow = lterminal->parent->parent;
    struct lcrtc_user *user = lterminal->user;
}

static int lcrt_serial_connect_remote(struct lcrt_terminal *lterminal)
{
}

static void lcrt_serial_create_subbox(struct lcrt_qconnect *lqconnect)
{
    GtkWidget *vbox;
    GtkWidget *vbox_spec;
    GtkWidget *hbox1;
    GtkWidget *label_port;
    GtkWidget *combobox_port;
    GtkWidget *label_baud_rate;
    GtkWidget *combobox_baud_rate;
    GtkWidget *label_data_bits;
    GtkWidget *combobox_data_bits;
    GtkWidget *label_parity;
    GtkWidget *combobox_parity;
    GtkWidget *label_stop_bits;
    GtkWidget *combobox_stop_bits;
    GtkWidget *frame;
    GtkWidget *alignment;
    GtkWidget *hbox_frame;
    GtkWidget *vbox_frame;
    GtkWidget *checkbutton_dir_dsr;
    GtkWidget *checkbutton_rts_cts;
    GtkWidget *checkbutton_xon_xoff;
    GtkWidget *label_flow_control;
    int i;
    static const char *sport[LCRT_SERIAL_PORT_NUMBER] = {LCRT_SERIAL_PORT};
    static const char *sbaud_rate[LCRT_SERIAL_BAUD_RATE_NUMBER] = {LCRT_SERIAL_BAUD_RATE};
    static const char *sdata_bits[LCRT_SERIAL_DATA_BITS_NUMBER] = {LCRT_SERIAL_DATA_BITS};
    static const char *sparity[LCRT_SERIAL_PARITY_NUMBER] = {LCRT_SERIAL_PARITY};
    static const char *sstop_bits[LCRT_SERIAL_STOP_BITS_NUMBER] = {LCRT_SERIAL_STOP_BITS};
    static const char *flow_control[LCRT_FC_NUMBER] = {LCRT_FLOW_CONTROL};

    vbox = GTK_DIALOG (lqconnect->q_connect)->vbox;
    debug_where();
    vbox_spec = gtk_vbox_new (FALSE, 0);
    lqconnect->q_vbox_spec = vbox_spec;
    gtk_widget_show (vbox_spec);
    gtk_box_pack_start (GTK_BOX (vbox), vbox_spec, TRUE, TRUE, 0);
    gtk_box_reorder_child (GTK_BOX (vbox), vbox_spec, 1);
    gtk_widget_set_size_request (vbox_spec, -1, 210);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox1, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox1, -1, 25);

    label_port = gtk_label_new (lqconnect->config.value[LCRT_Q_SPORT]);
    gtk_widget_show (label_port);
    gtk_box_pack_start (GTK_BOX (hbox1), label_port, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_port, 90, 25);
    gtk_misc_set_alignment (GTK_MISC (label_port), 0, 0.5);

    combobox_port = gtk_combo_box_entry_new_text ();
    lqconnect->pserial.port = combobox_port;
    gtk_widget_show (combobox_port);
    gtk_box_pack_start (GTK_BOX (hbox1), combobox_port, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_port, SERIAL_COMBOBOX_WIDTH, 25);
    
    for (i = 0; i < LCRT_SERIAL_PORT_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_port), sport[i]);
    }
    debug_where();
    //gtk_entry_set_editable(GTK_ENTRY(GTK_BIN(combobox_port)->child), FALSE);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox1, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox1, -1, 25);

    label_baud_rate = gtk_label_new (lqconnect->config.value[LCRT_Q_SBAUD_RATE]);
    gtk_widget_show (label_baud_rate);
    gtk_box_pack_start (GTK_BOX (hbox1), label_baud_rate, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_baud_rate, 90, 25);
    gtk_misc_set_alignment (GTK_MISC (label_baud_rate), 0, 0.5);

    combobox_baud_rate = gtk_combo_box_entry_new_text ();
    lqconnect->pserial.baud_rate = combobox_baud_rate;
    gtk_widget_show (combobox_baud_rate);
    gtk_box_pack_start (GTK_BOX (hbox1), combobox_baud_rate, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_baud_rate, SERIAL_COMBOBOX_WIDTH, 25);
    
    for (i = 0; i < LCRT_SERIAL_BAUD_RATE_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_baud_rate), sbaud_rate[i]);
    }
    debug_where();
    //gtk_entry_set_editable(GTK_ENTRY(GTK_BIN(combobox_baud_rate)->child), FALSE);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox1, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox1, -1, 25);

    label_data_bits = gtk_label_new (lqconnect->config.value[LCRT_Q_SDATA_BITS]);
    gtk_widget_show (label_data_bits);
    gtk_box_pack_start (GTK_BOX (hbox1), label_data_bits, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_data_bits, 90, 25);
    gtk_misc_set_alignment (GTK_MISC (label_data_bits), 0, 0.5);

    combobox_data_bits = gtk_combo_box_entry_new_text ();
    lqconnect->pserial.data_bits = combobox_data_bits;
    gtk_widget_show (combobox_data_bits);
    gtk_box_pack_start (GTK_BOX (hbox1), combobox_data_bits, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_data_bits, SERIAL_COMBOBOX_WIDTH, 25);
    
    for (i = 0; i < LCRT_SERIAL_DATA_BITS_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_data_bits), sdata_bits[i]);
    }
    debug_where();
    gtk_entry_set_editable(GTK_ENTRY(GTK_BIN(combobox_data_bits)->child), FALSE);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox1, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox1, -1, 25);

    label_parity = gtk_label_new (lqconnect->config.value[LCRT_Q_SPARITY]);
    gtk_widget_show (label_parity);
    gtk_box_pack_start (GTK_BOX (hbox1), label_parity, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_parity, 90, 25);
    gtk_misc_set_alignment (GTK_MISC (label_parity), 0, 0.5);

    combobox_parity = gtk_combo_box_entry_new_text ();
    lqconnect->pserial.parity = combobox_parity;
    gtk_widget_show (combobox_parity);
    gtk_box_pack_start (GTK_BOX (hbox1), combobox_parity, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_parity, SERIAL_COMBOBOX_WIDTH, 25);
    
    for (i = 0; i < LCRT_SERIAL_PARITY_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_parity), sparity[i]);
    }
    debug_where();
    gtk_entry_set_editable(GTK_ENTRY(GTK_BIN(combobox_parity)->child), FALSE);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_box_pack_start (GTK_BOX (vbox_spec), hbox1, TRUE, TRUE, 0);
    gtk_widget_set_size_request (hbox1, -1, 25);

    label_stop_bits = gtk_label_new (lqconnect->config.value[LCRT_Q_SSTOP_BITS]);
    gtk_widget_show (label_stop_bits);
    gtk_box_pack_start (GTK_BOX (hbox1), label_stop_bits, FALSE, FALSE, 0);
    gtk_widget_set_size_request (label_stop_bits, 90, 25);
    gtk_misc_set_alignment (GTK_MISC (label_stop_bits), 0, 0.5);

    combobox_stop_bits = gtk_combo_box_entry_new_text ();
    lqconnect->pserial.stop_bits = combobox_stop_bits;
    gtk_widget_show (combobox_stop_bits);
    gtk_box_pack_start (GTK_BOX (hbox1), combobox_stop_bits, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_stop_bits, SERIAL_COMBOBOX_WIDTH, 25);
    
    for (i = 0; i < LCRT_SERIAL_STOP_BITS_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_stop_bits), sstop_bits[i]);
    }
    debug_where();
    gtk_entry_set_editable(GTK_ENTRY(GTK_BIN(combobox_stop_bits)->child), FALSE);

    frame = gtk_frame_new (NULL);
    gtk_widget_show (frame);
    gtk_widget_set_size_request (frame, -1, 40);
    gtk_box_pack_start (GTK_BOX (vbox_spec), frame, TRUE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_OUT);

    alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (frame), alignment);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 12, 0);

    hbox_frame = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox_frame);
    gtk_container_add (GTK_CONTAINER (alignment), hbox_frame);

    checkbutton_dir_dsr = gtk_check_button_new_with_mnemonic (flow_control[LCRT_FC_DTR_DSR]);
    lqconnect->pserial.dir_dsr= checkbutton_dir_dsr;
    gtk_widget_show (checkbutton_dir_dsr);
    gtk_box_pack_start (GTK_BOX (hbox_frame), checkbutton_dir_dsr, FALSE, FALSE, 0);
    gtk_widget_set_size_request (checkbutton_dir_dsr, 100, -1);

    checkbutton_rts_cts = gtk_check_button_new_with_mnemonic (flow_control[LCRT_FC_CTS_CRS]);
    lqconnect->pserial.rts_cts= checkbutton_rts_cts;
    gtk_widget_show (checkbutton_rts_cts);
    gtk_box_pack_start (GTK_BOX (hbox_frame), checkbutton_rts_cts, FALSE, FALSE, 0);
    gtk_widget_set_size_request (checkbutton_rts_cts, 100, -1);

    checkbutton_xon_xoff = gtk_check_button_new_with_mnemonic (flow_control[LCRT_FC_XON_XOFF]);
    lqconnect->pserial.xon_xoff= checkbutton_xon_xoff;
    gtk_widget_show (checkbutton_xon_xoff);
    gtk_box_pack_start (GTK_BOX (hbox_frame), checkbutton_xon_xoff, FALSE, FALSE, 0);
    gtk_widget_set_size_request (checkbutton_xon_xoff, 100, -1);

    label_flow_control = gtk_label_new (lqconnect->config.value[LCRT_Q_SFLOW_CONTROL]);
    gtk_widget_show (label_flow_control);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label_flow_control);
    gtk_label_set_use_markup (GTK_LABEL (label_flow_control), TRUE);

    if (lqconnect->flag == LCRT_QCONNECT_SESSION_OPTION) {
    } else {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_port), 1); //ttyS0
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_baud_rate), 10); //115200
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_data_bits), 3); //8
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_parity), 0); //None
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_stop_bits), 0); //1
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_rts_cts), TRUE);
    }
    gtk_widget_set_sensitive(lqconnect->q_bt_connect, TRUE);

}

static struct lcrtc_user *lcrt_serial_create_session(struct lcrt_qconnect *lqconnect)
{
}

struct lcrt_protocol_callback lcrt_protocol_serial_callbacks = {
    .protocol         = LCRT_PROTOCOL_SERIAL,
    .contents_changed = lcrt_serial_contents_changed,
    .connect_remote   = lcrt_serial_connect_remote,
    .create_subbox    = lcrt_serial_create_subbox,
    .create_session   = lcrt_serial_create_session,
};

