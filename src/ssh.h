#ifndef __LCRT_SSH_H__
#define __LCRT_SSH_H__

#include "iqconnect.h"
#include "iterminal.h"
#include "user.h"

void lcrt_qconnect_create_ssh_subbox(struct lcrt_qconnect *lqconnect);
void *lcrt_terminal_ssh_contents_changed(struct lcrt_terminal *lterminal);
struct lcrtc_user *lcrt_qconnect_ssh_on_button_connect_clicked(struct lcrt_qconnect *lqconnect);

#endif
