#ifndef __LCRT_SSH_H__
#define __LCRT_SSH_H__

#include "protocol.h"
/* authentication for ssh */
enum {
    LCRT_AUTH_PASSWD = 0,
    LCRT_AUTH_PUBLICKEY,
    LCRT_AUTH_KEYBOARD,
    LCRT_AUTH_GSSAPI,
    LCRT_AUTH_RSA,
    LCRT_AUTH_TIS,
	LCRT_AUTH_NUMBER
};

extern struct lcrt_protocol_callback lcrt_protocol_ssh2_callbacks;
extern struct lcrt_protocol_callback lcrt_protocol_ssh1_callbacks;

#endif
