#ifndef __LCRT_CSTATUSBAR_H__
#define __LCRT_CSTATUSBAR_H__

#include "istatusbar.h"

void lcrt_statusbar_set_message(struct lcrt_statusbar *lstatusbar, const char *format, ...);
void lcrt_statusbar_set_user(struct lcrt_statusbar *lstatusbar, struct lcrtc_user *user);
#endif
