#include <gtk/gtk.h>
#include "istatusbar.h"
#include "debug.h"
#include "serial.h"

void lcrt_statusbar_set_message(struct lcrt_statusbar *lstatusbar, const char *format, ...)
{
    unsigned int id;
    char buffer[1024];
    va_list args;

    if (lstatusbar == NULL)
        return;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    id = gtk_statusbar_get_context_id(GTK_STATUSBAR(lstatusbar->statusbar), buffer);
    gtk_statusbar_push(GTK_STATUSBAR(lstatusbar->statusbar), id, buffer);
}
void lcrt_statusbar_set_user(struct lcrt_statusbar *lstatusbar, struct lcrtc_user *user)
{
    char *proto[LCRT_PROTOCOL_NUMBER] = {LCRT_PROTOCOL_NAME};

    if (user == NULL)
        return;

    if (!user->is_folder) {
         if (user->protocol == LCRT_PROTOCOL_SERIAL) {
             char s_port[USERNAME_LEN], *ss_buad_rate, *ss_databit;
	     int i;
             int s_baud_rate,s_databit,s_parity,s_stopbit,s_software,s_hardware;
             const struct lcrt_serial_map sbaud_rate[LCRT_SERIAL_BAUD_RATE_NUMBER] = {LCRT_SERIAL_BAUD_RATE_TABLE};
             const struct lcrt_serial_map sdata_bits[LCRT_SERIAL_DATA_BITS_NUMBER] = {LCRT_SERIAL_DATA_BITS_TABLE};
             const struct lcrt_serial_map sparity[LCRT_SERIAL_PARITY_NUMBER] = {LCRT_SERIAL_PARITY_TABLE};
             const struct lcrt_serial_map sstop_bits[LCRT_SERIAL_STOP_BITS_NUMBER] = {LCRT_SERIAL_STOP_BITS_TABLE};

             sscanf(user->password, "%s %d %d %d %d %d %d", 
                   s_port,
                   &s_baud_rate,
                   &s_databit,
                   &s_parity,
                   &s_stopbit,
                   &s_software,
                   &s_hardware);
             for (i = 0; i < LCRT_SERIAL_BAUD_RATE_NUMBER; i++) {
		     if (sbaud_rate[i].data == s_baud_rate)
			     ss_buad_rate = sbaud_rate[i].name;
	     }
             for (i = 0; i < LCRT_SERIAL_DATA_BITS_NUMBER; i++) {
		     if (sdata_bits[i].data == s_databit)
			     ss_databit = sdata_bits[i].name;
	     }
             lcrt_statusbar_set_message(lstatusbar,
                                "[%s] %s %s %s %s %s", 
                                proto[user->protocol],
                                s_port,
                                ss_buad_rate,
				ss_databit,
				sparity[s_parity].name,
				sstop_bits[s_stopbit].name);

         } else {
             lcrt_statusbar_set_message(lstatusbar,
                                "[%s] %s@%s %d", 
                                proto[user->protocol],
                                user->username,
                                user->hostname,
				user->port);
        }
    } else {
         lcrt_statusbar_set_message(lstatusbar,
                                "[FOLDER]: %s", 
                                user->folder);
    }
}
