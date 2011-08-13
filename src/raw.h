#ifndef __LCRT_RAW_H__
#define __LCRT_RAW_H__

#include <gtk/gtk.h>
#include "protocol.h"

struct lcrt_raw {
    GtkWidget *hostname;
    GtkWidget *username;
    GtkWidget *firewall;
    GtkWidget *port;
};

extern struct lcrt_protocol_callback lcrt_protocol_raw_callbacks;
#endif
