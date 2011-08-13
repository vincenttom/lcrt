#ifndef __LCRT_SERIAL_H__
#define __LCRT_SERIAL_H__

#include <gtk/gtk.h>
#include "protocol.h"
#include <termios.h>

#define LCRT_SERIAL_PORT_NUMBER 7
#define LCRT_SERIAL_PORT \
    "/dev/console", \
    "/dev/ttyS0", \
    "/dev/ttyS1", \
    "/dev/ttyS2", \
    "/dev/ttyUSB0", \
    "/dev/ttyUSB1", \
    "/dev/ttyUSB2",

#define LCRT_SERIAL_BAUD_RATE_NUMBER 14
#define LCRT_SERIAL_BAUD_RATE_TABLE \
    {0, "110", B110}, \
    {1, "300", B300}, \
    {2, "600", B600}, \
    {3, "1200", B1200}, \
    {4, "2400", B2400}, \
    {5, "4800", B4800}, \
    {6, "9600", B9600}, \
    {7, "19200", B19200}, \
    {8, "38400", B38400}, \
    {9, "57600", B57600}, \
    {10, "115200", B115200}, \
    {11, "230400", B230400}, \
    {12, "460800", B460800}, \
    {13, "921600", B921600},

#define LCRT_SERIAL_DATA_BITS_NUMBER 4
#define LCRT_SERIAL_DATA_BITS_TABLE \
    {0, "5", CS5}, \
    {1, "6", CS6}, \
    {2, "7", CS7}, \
    {3, "8", CS8}

#define LCRT_SERIAL_PARITY_NUMBER 5
#define LCRT_SERIAL_PARITY_TABLE \
    {0, "None",  0}, \
    {1, "Odd",   0}, \
    {2, "Even",  0}, \
    {3, "Mark",  0}, \
    {4, "Space", 0}

#define LCRT_SERIAL_STOP_BITS_NUMBER 3
#define LCRT_SERIAL_STOP_BITS_TABLE \
    {0, "1", 0}, \
    {1, "1.5", 0}, \
    {2, "2", 0}

#define LCRT_SEROAL_FLOW_CONTROL_NUMBER 2
#define LCRT_SEROAL_FLOW_CONTROL_TABLE \
    {0, "Software", 0}, \
    {1, "Hardware", 0}

#define SERIAL_COMBOBOX_WIDTH 220

extern struct lcrt_protocol_callback lcrt_protocol_serial_callbacks;

#endif
