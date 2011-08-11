#ifndef __LCRT_FOPERATE_H__
#define __LCRT_FOPERATE_H__

int lcrt_mkdir(const char *dir);
int lcrt_copy(const char *fsrc, const char *fdst);
int lcrt_dircopy(const char *dsrc, const char *ddst);
int lcrt_move(const char *fsrc, const char *fdst);
int lcrt_remove(const char *fname);
int lcrt_fappend(const char *fname, const char *format, ...);
int lcrt_fcreate(const char *fname);
int lcrt_echeck(const char *prog);
#endif
