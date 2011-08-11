#ifndef __LCRT_SERIAL_H__
#define __LCRT_SERIAL_H__

#include "protocol.h"

#define LCRT_SERIAL_PORT_NUMBER 7
#define LCRT_SERIAL_PORT \
    "console", \
    "ttyS0", \
    "ttyS1", \
    "ttyS2", \
    "ttyUSB0", \
    "ttyUSB1", \
    "ttyUSB2",

#define LCRT_SERIAL_BAUD_RATE_NUMBER 15
#define LCRT_SERIAL_BAUD_RATE \
    "111", \
    "300", \
    "600", \
    "1200", \
    "2400", \
    "4800", \
    "9600", \
    "11400", \
    "19200", \
    "57600", \
    "115200", \
    "230400", \
    "380400", \
    "460800", \
    "921600",

#define LCRT_SERIAL_DATA_BITS_NUMBER 4
#define LCRT_SERIAL_DATA_BITS \
    "5", \
    "6", \
    "7", \
    "8"

#define LCRT_SERIAL_PARITY_NUMBER 5
#define LCRT_SERIAL_PARITY \
    "None", \
    "Odd", \
    "Even", \
    "Mark", \
    "Space"

#define LCRT_SERIAL_STOP_BITS_NUMBER 3
#define LCRT_SERIAL_STOP_BITS \
    "1", \
    "1.5", \
    "2"
enum {
    LCRT_FC_DTR_DSR,
    LCRT_FC_CTS_CRS,
    LCRT_FC_XON_XOFF,
    LCRT_FC_NUMBER
};
#define LCRT_FLOW_CONTROL \
    "DTR/DSR", \
    "CTS/CRS", \
    "XON/XOFF",

#define SERIAL_COMBOBOX_WIDTH 220

extern struct lcrt_protocol_callback lcrt_protocol_serial_callbacks;

#endif
