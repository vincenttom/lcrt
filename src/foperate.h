#ifndef __LCRT_FOPERATE_H__
#define __LCRT_FOPERATE_H__

int lcrt_fmkdir(const char *dir);
int lcrt_fcopy(const char *fsrc, const char *fdst);
int lcrt_fdircopy(const char *dsrc, const char *ddst);
int lcrt_fmove(const char *fsrc, const char *fdst);
int lcrt_remove(const char *fname);
int lcrt_fappend(const char *fname, const char *format, ...);
int lcrt_fcreate(const char *fname);
int lcrt_echeck(const char *prog);
#endif
