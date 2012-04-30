/**
 * @file serial.c
 * <h2>License</h2>
 * Copyright (c) 2010-2011 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * @author Niu Tao <niutao0602@gmail.com>
 * @version $Id
 * @date Sat 06 Aug 2011 10:55:20 PM CST
 *
 * @brief  
 */

//#define __LCRT_DEBUG__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <libgen.h>
#include <gtk/gtk.h>
#include <vte/vte.h>
#include "iwindow.h"
#include "iterminal.h"
#include "iqconnect.h"
#include "cterminal.h"
#include "user.h"
#include "serial.h"
#include "debug.h"
#include "message.h"
#include "cstatusbar.h"

#if 0
/**
 * fix compile error in hurd-i386, kfreebsd-amd64,
 * kfreebsd-i386, mips, mipsel
 */
#ifndef CMSPAR
#define CMSPAR 010000000000
#endif
#ifndef CBAUD
#define CBAUD  0010017
#endif
#endif

struct lcrt_serial_map {
    int index;
    char *name;
    int data;
};
struct lcrt_serial_if {
    GtkWidget *port;
    GtkWidget *baud_rate;
    GtkWidget *data_bits;
    GtkWidget *parity;
    GtkWidget *stop_bits;
    /* for flow control */
    GtkWidget *software;
    GtkWidget *hardware;
};
struct lcrt_serial_tm {
    int fd; /**< the opened file descriptor of serial device */
    unsigned int input; /**< A unique id for the input event source */
    unsigned int commit; /**< terminal commit signal id */
};

static void lcrt_serial_disconnect(struct lcrt_terminal *lterminal);

static void lcrt_serial_receive(struct lcrt_terminal *lterminal)
{
#if 0
    VteTerminal *vteterminal = lterminal->terminal;
    struct lcrt_window *lwindow = lterminal->parent->parent;
    struct lcrtc_user *user = lterminal->user;
#endif
    debug_where();
    lcrt_terminal_set_connected_status(lterminal);
}
/**
 * @brief Open and configure the serial port.
 * @param port the device of serial port.
 * @param baud_rate baud rate. see bits/termios.h
 * @param databit data bit. see bits/termios.h
 * @param parity the pattern of parity.\n
 *        0 None\n
 *        1 Odd\n
 *        2 Even\n
 *        3 Mark\n
 *        4 Space\n
 * @param stopbit the number of stop bit.\n
 *        0 1bit\n
 *        1 1.5bit\n
 *        2 2bit\n
 * @param software software flow control.\n
 *        0 not use software flow control\n
 *        1 use software flow control\n
 * @param hardware hardware flow control.\n
 *        0 not use hardware flow control\n
 *        1 use hardware flow control\n
 * @return >0 serial file descriptor \n
 *         <0 error code.
 */
int lcrt_serial_config(const char *port, int baud_rate, int databit,
    int parity, int stopbit, int software, int hardware)
{
    static struct termios ltermios;
    int fd;
    if ((fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY)) == -1) {
        return -errno;
    }
    //tcgetattr(fd, &ltermios);
	memset(&ltermios, 0, sizeof(struct termios));
#ifdef CBAUD
    ltermios.c_cflag |= (baud_rate & CBAUD);
#endif
#ifdef CSIZE
    ltermios.c_cflag |= (databit & CSIZE);
#endif
    switch (stopbit) {
    case 0:
        ltermios.c_cflag &= ~CSTOPB; 
        break;
    case 1:
        ltermios.c_cflag |= CSTOPB; 
        break;
    case 2:
        ltermios.c_cflag &= ~CSTOPB; 
        break;
    default:
        return -EINVAL;
    }
   
    switch (parity) {
    case 0:
        ltermios.c_cflag &= ~PARENB; /* Clear parity enable */
        ltermios.c_iflag &= ~INPCK; /* Enable parity checking */ 
        break;
    case 1:
        ltermios.c_cflag |= (PARODD | PARENB); /* */
        break;
    case 2:
        ltermios.c_cflag |= PARENB; /* Enable parity */
        break;
#ifdef CMSPAR
    case 3:
        ltermios.c_cflag &= ~CMSPAR;
        break;
    case 4:
        ltermios.c_cflag |= CMSPAR;
        break;
#endif
    default:
        return -EINVAL;
    }
    ltermios.c_cflag |= CREAD;
    ltermios.c_iflag = IGNPAR | IGNBRK;
    
    if (software)
        ltermios.c_iflag |= (IXON | IXOFF | IXANY); //Enable Software Flow Control
    if (hardware)
        ltermios.c_cflag |= CRTSCTS;  //Enable hardware flow control
    if (!software && !hardware)
        ltermios.c_cflag |= CLOCAL;
    
    ltermios.c_oflag = 0;
    ltermios.c_lflag = 0;
    ltermios.c_cc[VTIME] = 0;
    ltermios.c_cc[VMIN] = 1;
    
    tcsetattr(fd, TCSANOW, &ltermios);
    tcflush(fd, TCOFLUSH);  
    tcflush(fd, TCIFLUSH);
    
    return fd;
}

/**
 * @brief the callback to read data from serial port and put the data to terminal.
 * @param user_data we use it to point to struct lcrt_terminal.
 * @param fd file descriptor of serial port.
 * @condition
 */
static void lcrt_serial_read(gpointer user_data, gint fd, GdkInputCondition condition)
{
    struct lcrt_terminal *lterminal = (struct lcrt_terminal *)user_data;
    int len = 0;
    unsigned char buffer[1024] = {0};    
    debug_where();
    len = read(fd, buffer, 1024);
    if (len <= 0) {
		/** serial device may be pull out  */
		if (errno == EAGAIN) {
			lcrt_serial_disconnect(lterminal);
			return;
		}
        return ;
    }
    vte_terminal_feed(VTE_TERMINAL(lterminal->terminal), (const char *)buffer, len);
}
/**
 * @brief the callback to write data to serial port.
 * @param widget
 * @param text the data to be write
 * @param length data length
 * @param point to struct lcrt_terminal.
 */
static void lcrt_serial_write(VteTerminal *widget, gchar *text, guint length, gpointer user_data)
{
    struct lcrt_terminal *lterminal = (struct lcrt_terminal *)user_data;
    struct lcrt_serial_tm *tserial = (struct lcrt_serial_tm *)lterminal->private_data;
    int fd = tserial->fd;

    debug_where();
    if (tserial->fd)
        write(fd, text, length);
}
static int lcrt_serial_connect(struct lcrt_terminal *lterminal)
{
    struct lcrtc_user *user;
    struct lcrt_serial_tm *tserial;
    /* port baud_rate databit parity stopbit software_control hardware_control*/
    char s_port[USERNAME_LEN];
    int s_baud_rate,s_databit,s_parity,s_stopbit,s_software,s_hardware;
    debug_where();
    if (lterminal == NULL)
       return -EINVAL;

    tserial = (struct lcrt_serial_tm *)calloc(1, sizeof(struct lcrt_serial_tm));
    if (tserial == NULL)
        return -ENOMEM;

    user = lterminal->user;
    sscanf(user->password, "%s %d %d %d %d %d %d", 
            s_port,
            &s_baud_rate,
            &s_databit,
            &s_parity,
            &s_stopbit,
            &s_software,
            &s_hardware);
    int fd ;
    fd = lcrt_serial_config(s_port, s_baud_rate,s_databit,
                            s_parity,s_stopbit,s_software,s_hardware);
    if (fd <= 0) {
        lcrt_message_error(lterminal->parent->parent->window, 
                lterminal->parent->config.value[LCRT_TM_SERIAL_ERROR], s_port, strerror(-fd));
        free(tserial);
        return fd;
    }
    tserial->fd = fd;
    lterminal->private_data = tserial;
    tserial->input = gtk_input_add_full(fd, GDK_INPUT_READ, 
            (GdkInputFunction)lcrt_serial_read, NULL, lterminal, NULL);
    tserial->commit = g_signal_connect(lterminal->terminal, "commit", 
                     G_CALLBACK(lcrt_serial_write), lterminal);
    lcrt_statusbar_set_user(lterminal->parent->parent->w_statusbar, lterminal->user);
    lcrt_terminal_set_connected_status(lterminal);
	return LCRTE_OK;
}

static void lcrt_serial_disconnect(struct lcrt_terminal *lterminal)
{
    struct lcrt_serial_tm *tserial = lterminal->private_data;
    debug_where();
    if (lterminal->connected) {
        if (tserial && tserial->fd > 0) {
            close(tserial->fd);
        }
        lcrt_terminal_on_child_exited(NULL, lterminal);
        lcrt_terminal_set_status(lterminal, NULL, LCRT_TERMINAL_DISCONNECT);
    }
    if (tserial) {
        g_signal_handler_disconnect(lterminal->terminal, tserial->commit);
        gtk_input_remove(tserial->input);
        free(tserial);
        lterminal->private_data = NULL;
    }
    debug_where();
}
static void lcrt_serial_show(struct lcrt_qconnect *lqconnect)
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
    GtkWidget *checkbutton_software;
    GtkWidget *checkbutton_hardware;
    GtkWidget *label_flow_control;
    int i;
    const char *sport[LCRT_SERIAL_PORT_NUMBER] = {LCRT_SERIAL_PORT};
    const struct lcrt_serial_map sbaud_rate[LCRT_SERIAL_BAUD_RATE_NUMBER] = {LCRT_SERIAL_BAUD_RATE_TABLE};
    const struct lcrt_serial_map sdata_bits[LCRT_SERIAL_DATA_BITS_NUMBER] = {LCRT_SERIAL_DATA_BITS_TABLE};
    const struct lcrt_serial_map sparity[LCRT_SERIAL_PARITY_NUMBER] = {LCRT_SERIAL_PARITY_TABLE};
    const struct lcrt_serial_map sstop_bits[LCRT_SERIAL_STOP_BITS_NUMBER] = {LCRT_SERIAL_STOP_BITS_TABLE};
    const struct lcrt_serial_map flow_control[LCRT_SEROAL_FLOW_CONTROL_NUMBER] = {LCRT_SEROAL_FLOW_CONTROL_TABLE};
    static struct lcrt_serial_if slserial, *lserial = &slserial;
    struct lcrtc_user *user = NULL;
    char s_port[USERNAME_LEN];
	boolean f_option = FALSE;
    int s_baud_rate,s_databit,s_parity,s_stopbit,s_software,s_hardware;
	if (lqconnect->flag == LCRT_QCONNECT_SESSION_OPTION &&
    	(user = lcrt_user_find_by_name(&lqconnect->parent->u_config, lqconnect->uname)) != NULL) {
        sscanf(user->password, "%s %d %d %d %d %d %d", 
                s_port,
                &s_baud_rate,
                &s_databit,
                &s_parity,
                &s_stopbit,
                &s_software,
                &s_hardware);
        debug_print("SERIAL PORT: %s %d %d %d %d %d %d\n", 
                s_port,
                s_baud_rate,
                s_databit,
                s_parity,
                s_stopbit,
                s_software,
                s_hardware);

		f_option = TRUE;
	}

    memset(lserial, 0, sizeof(struct lcrt_serial_if));
    lqconnect->private_data = lserial;

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
    lserial->port = combobox_port;
    gtk_widget_show (combobox_port);
    gtk_box_pack_start (GTK_BOX (hbox1), combobox_port, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_port, SERIAL_COMBOBOX_WIDTH, 25);
    
    for (i = 0; i < LCRT_SERIAL_PORT_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_port), sport[i]);
		if (f_option && strcmp(s_port, sport[i]) == 0)
			gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_port), i);

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
    lserial->baud_rate = combobox_baud_rate;
    gtk_widget_show (combobox_baud_rate);
    gtk_box_pack_start (GTK_BOX (hbox1), combobox_baud_rate, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_baud_rate, SERIAL_COMBOBOX_WIDTH, 25);
    
    for (i = 0; i < LCRT_SERIAL_BAUD_RATE_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_baud_rate), sbaud_rate[i].name);
		if (f_option && s_baud_rate == sbaud_rate[i].data)
			gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_baud_rate), i);
    }
    debug_where();
    gtk_entry_set_editable(GTK_ENTRY(GTK_BIN(combobox_baud_rate)->child), FALSE);

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
    lserial->data_bits = combobox_data_bits;
    gtk_widget_show (combobox_data_bits);
    gtk_box_pack_start (GTK_BOX (hbox1), combobox_data_bits, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_data_bits, SERIAL_COMBOBOX_WIDTH, 25);
    
    for (i = 0; i < LCRT_SERIAL_DATA_BITS_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_data_bits), sdata_bits[i].name);
		if (f_option && s_databit == sdata_bits[i].data)
			gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_data_bits), i);
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
    lserial->parity = combobox_parity;
    gtk_widget_show (combobox_parity);
    gtk_box_pack_start (GTK_BOX (hbox1), combobox_parity, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_parity, SERIAL_COMBOBOX_WIDTH, 25);
    
    for (i = 0; i < LCRT_SERIAL_PARITY_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_parity), sparity[i].name);
    }
	if (f_option)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_parity), s_parity);
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
    lserial->stop_bits = combobox_stop_bits;
    gtk_widget_show (combobox_stop_bits);
    gtk_box_pack_start (GTK_BOX (hbox1), combobox_stop_bits, FALSE, TRUE, 0);
    gtk_widget_set_size_request (combobox_stop_bits, SERIAL_COMBOBOX_WIDTH, 25);
    
    for (i = 0; i < LCRT_SERIAL_STOP_BITS_NUMBER; i++) {
        gtk_combo_box_append_text (GTK_COMBO_BOX (combobox_stop_bits), sstop_bits[i].name);
    }
	if (f_option)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_stop_bits), s_stopbit);
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

    checkbutton_software = gtk_check_button_new_with_mnemonic (flow_control[0].name);
    lserial->software = checkbutton_software;
    gtk_widget_show (checkbutton_software);
    gtk_box_pack_start (GTK_BOX (hbox_frame), checkbutton_software, FALSE, FALSE, 0);
    gtk_widget_set_size_request (checkbutton_software, 150, -1);

	if (f_option)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_software), s_software);

    checkbutton_hardware = gtk_check_button_new_with_mnemonic (flow_control[1].name);
    lserial->hardware = checkbutton_hardware;
    gtk_widget_show (checkbutton_hardware);
    gtk_box_pack_start (GTK_BOX (hbox_frame), checkbutton_hardware, FALSE, FALSE, 0);
    gtk_widget_set_size_request (checkbutton_hardware, 150, -1);

	if (f_option)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_hardware), s_hardware);

    label_flow_control = gtk_label_new (lqconnect->config.value[LCRT_Q_SFLOW_CONTROL]);
    gtk_widget_show (label_flow_control);
    gtk_frame_set_label_widget (GTK_FRAME (frame), label_flow_control);
    gtk_label_set_use_markup (GTK_LABEL (label_flow_control), TRUE);

    if (!f_option) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_port), 1); //ttyS0
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_baud_rate), 10); //115200
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_data_bits), 3); //8
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_parity), 0); //None
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_stop_bits), 0); //1
        debug_print("active = %d\n", gtk_combo_box_get_active(GTK_COMBO_BOX(combobox_baud_rate)));
    }
    gtk_widget_set_sensitive(lqconnect->q_bt_connect, TRUE);

}

static struct lcrtc_user *lcrt_serial_create(struct lcrt_qconnect *lqconnect)
{
    lcrt_protocol_t protocol = lqconnect->nproto;
    struct lcrt_window *lwindow = lqconnect->parent;
    struct lcrt_serial_if *lserial = (struct lcrt_serial_if *)lqconnect->private_data;
    struct lcrt_serial_map serial_buad_rate[] = {LCRT_SERIAL_BAUD_RATE_TABLE};
	struct lcrt_serial_map serial_databit[] = {LCRT_SERIAL_DATA_BITS_TABLE};
    struct lcrtc_user *user;
    char password[PASSWORD_LEN];
    char s_port[USERNAME_LEN];
    char name[HOSTNAME_LEN];
    char hostname[HOSTNAME_LEN];
#define get_text(widget) (gtk_combo_box_get_active_text(GTK_COMBO_BOX(lserial->widget)))
#define get_active(widget) (gtk_combo_box_get_active(GTK_COMBO_BOX(lserial->widget)))
    snprintf(s_port, USERNAME_LEN, "%s", get_text(port));
    debug_where();
    if (access(s_port, F_OK|R_OK|W_OK) == -1) {
        switch (errno) {
        case ENOENT:
            debug_print("device %s is not exist.\n", s_port);
            break;
        case EPERM:
            debug_print("you have no permition to open %s.\n", s_port);
            break;
        case EIO:
            debug_print("device %s is not ready.\n", s_port);
            break;
        default:
            debug_print("unknown error,error code is %d\n", errno);
            break;
        }
        return NULL;
    }
    int baud_rate,s_baud_rate;
    baud_rate = get_active(baud_rate);
    s_baud_rate = serial_buad_rate[baud_rate].data;
    debug_where();

    int databit, s_databit;
    databit = atoi(get_text(data_bits));
    databit = get_active(data_bits);
    s_databit = serial_databit[databit].data;
    debug_where();
    
    int s_parity;
    s_parity = get_active(parity);
    debug_where();

    int s_stopbits;
    s_stopbits = get_active(stop_bits);
    debug_where();

    int s_software, s_hardware;
    s_software = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lserial->software));
    s_hardware = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lserial->hardware));
    /* port baud_rate databit parity stopbit software_control hardware_control*/
    snprintf(password, PASSWORD_LEN, "%s %d %d %d %d %d %d", 
            s_port,
            s_baud_rate,
            s_databit,
            s_parity,
            s_stopbits,
            s_software,
            s_hardware);
    strcpy(hostname, basename(s_port));
    if (lqconnect->flag != LCRT_QCONNECT_SESSION_OPTION) {
        if ((user = lcrtc_user_create()) == NULL) {
            /* 
             * FIXME: There is no more memory, how can 
             * we handle this exception ?
             */
            return NULL;
        }

        lcrt_user_find_unused_label(lwindow, hostname, name);

        lcrtc_user_set_data(
           user,
           name,
           hostname,
           protocol,
           NULL,
           password,
           gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
           0,
           TRUE,
           lqconnect->folder,
           0
        );
        lcrtc_user_ref(user);
        lcrt_user_add(&lwindow->u_config, user);
        lcrt_window_set_current_user(lwindow, user);
        if (lqconnect->flag == LCRT_QCONNECT_IN_TAB) {
            lcrt_create_terminal(lwindow->w_notebook);
        }
    } else {
        if ((user = lcrt_user_find_by_name(&lwindow->u_config, lqconnect->uname)) != NULL) {
            lcrtc_user_set_data(
               user,
               lqconnect->uname,
               hostname,
               protocol,
               NULL,
               password,
               gtk_entry_get_text(GTK_ENTRY(lqconnect->q_et_default_command)),
               0,
               TRUE,
               NULL,
               -1
            );
        }
    }
    lcrtc_user_dump(user, __func__);
    return user;
#undef get_text
#undef get_active
}

struct lcrt_protocol_callback lcrt_protocol_serial_callbacks = {
    .protocol   = LCRT_PROTOCOL_SERIAL,
    .receive    = lcrt_serial_receive,
    .connect    = lcrt_serial_connect,
    .disconnect = lcrt_serial_disconnect,
    .show       = lcrt_serial_show,
    .create     = lcrt_serial_create,
    .changed    = NULL,
};

