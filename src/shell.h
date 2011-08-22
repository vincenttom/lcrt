#ifndef __LCRT_SHELL_H__
#define __LCRT_SHELL_H__

#include <gtk/gtk.h>
#include "protocol.h"

#define LCRT_SHELL_SUPPORT_NUMBER 4
#define LCRT_SHELL_SUPPORT \
	"/bin/bash", \
	"/bin/sh", \
	"/bin/dash", \
	"/bin/zsh"

extern struct lcrt_protocol_callback lcrt_protocol_shell_callbacks;

#endif
