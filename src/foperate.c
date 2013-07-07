/**
 * @file    foperate.c
 * <h2>License</h2>
 * Copyright (c) 2012 ~ Niu Tao
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * @author  Niu Tao <niutao0602@gmail.com>
 * @version v1.0
 * @date    2010-11-16
 *
 * @brief  The functions below are written for file operation.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>
#include <stdlib.h>

/**
 * @brief  Create directory from root, if the parent directory is not exist,
 *         then make it until last one.
 * @param dir path or directory which want to create
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fmkdir(const char *dir)
{
    int len;
    struct stat st;
    char *p, *tmp;
    char logdir[512];

    if (dir == NULL)
        return -EINVAL;
    /** the directory has exist, so ship  */
    if (access(dir, F_OK | R_OK | W_OK) == 0)
        return 0;

    strncpy(logdir,dir ,512);
    len = strlen(logdir);

    if (len == 0)
        return -EINVAL;

    p = logdir + len - 1;
    while (*p == '/')
        *p-- = '\0';

    p = logdir;
    while (*p == '/')
        p++;

    while (p < (logdir + len)) {
        tmp = strchr(p, '/');
        if (tmp)
            *tmp = '\0';
        if (stat(logdir, &st) == -1) {
            if (errno == ENOENT)
                mkdir(logdir, 0755);
            else
                break;
        } else if (!S_ISDIR(st.st_mode)) {
            break;
        }

        if (tmp == NULL)
            break;

        *tmp = '/';
        p = tmp + 1;
    }
    return 0;
}

/**
 * @brief  copy a file from fsrc to fdst
 * @param fsrc the source file name which will be copy
 * @param fdst the target file name which will be generate
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fcopy(const char *fsrc, const char *fdst)
{
    unsigned char buffer[4096];
    FILE *fps, *fpd;
    int rv = 0, len;

    if (fsrc == NULL || fdst == NULL) {
        rv = -EINVAL;
        goto out;
    }

    if ((fps = fopen(fsrc, "r")) == NULL) {
        rv = -errno;
        goto out;
    }

    if ((fpd = fopen(fdst, "w")) == NULL) {
        rv = -errno;
        goto out1;
    }
    while (!feof(fps)) {
        len = fread(buffer, 1, sizeof(buffer), fps);
        if (fwrite(buffer, 1, len, fpd) != len) {
            rv = -errno;
            goto out2;
        }
    }
out2:
    fclose(fpd);
out1:
    fclose(fps);
out:
    return rv;
}

/**
 * @brief  copy a directory from another
 * @param dsrc the source directory name
 * @param ddst the target directory name
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fdircopy(const char *dsrc, const char *ddst)
{
    DIR *dir;
    struct dirent *direntp;
    char fsrc[512], fdst[512];
    int rv = 0;

    if (dsrc == NULL || ddst == NULL) {
        rv = -EINVAL;
        goto out;
    }

    if (access(dsrc, F_OK | R_OK) == -1) {
        rv = -errno;
        goto out;
    }
    if ((dir = opendir(dsrc)) == NULL) {
        rv = -errno;
        goto out;
    }
    lcrt_fmkdir(ddst);
    while ((direntp = readdir(dir)) != NULL) {
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
            continue;
        if (direntp->d_type == DT_DIR) {
            snprintf(fsrc, sizeof(fsrc), "%s/%s", dsrc, direntp->d_name);
            snprintf(fdst, sizeof(fdst), "%s/%s", ddst, direntp->d_name);
            lcrt_fmkdir(fdst);
            lcrt_fdircopy(fsrc, fdst);
            continue;
        }
        snprintf(fsrc, sizeof(fsrc), "%s/%s", dsrc, direntp->d_name);
        snprintf(fdst, sizeof(fdst), "%s/%s", ddst, direntp->d_name);
        lcrt_fcopy(fsrc, fdst);
    }
    closedir(dir);
out:
    return rv;
}

/**
 * @brief  move a file to another path
 * @param fsrc the source file name
 * @param fdst the target file name
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fmove(const char *fsrc, const char *fdst)
{
    int rv;

    if ((rv = lcrt_fcopy(fsrc, fdst)) == 0) {
        unlink(fsrc);
    }
    return rv;
}

/**
 * @brief  delete a file
 * @param fname the file name which will be deleted
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fremove(const char *fname)
{
    if (fname == NULL)
        return -EINVAL;

    if (unlink(fname) == -1)
        return -errno;

    return 0;
}

/**
 * @brief  print some content to the end of file
 * @param fname the file name
 * @param format the format of content
 * @param ... the argument
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fappend(const char *fname, const char *format, ...)
{
    va_list args;
    char buffer[4096];
    FILE *fp;
    int rv;

    if (fname == NULL)
        return -EINVAL;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if ((fp = fopen(fname, "a+")) == NULL)
        return -errno;

    rv = fprintf(fp, "%s", buffer);
    fclose(fp);

    return rv;
}

/**
 * @brief  create a file
 * @param fname the file name which will be created
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fcreate(const char *fname)
{
    FILE *fp;

    if (fname == NULL)
        return -EINVAL;

    if ((fp = fopen(fname, "w")) == NULL)
        return -errno;

    fclose(fp);

    return 0;
}
/**
 * @brief  check whether the executive programe is exist in directory 'PATH'
 * @param prog the executive programe name
 * @param p_path if found, store path to p_path
 * @return = 0, exist in directory 'PATH'
 *         < 0, error code
 */
int lcrt_echeck(const char *prog, char *p_path)
{
    char *env;
    char *vpath, *path;
    char buff[512];
    int rv = -1;

    if (prog == NULL) {
        return -EINVAL;
    }
    if ((env = getenv("PATH")) == NULL) {
        return -errno;
    }
    vpath = malloc(strlen(env) + 1);
    if (vpath == NULL)
        return -errno;
    strcpy(vpath, env);
    path = strtok(vpath, ":");
    while (path != NULL) {
        snprintf(buff, sizeof(buff), "%s/%s", path, prog);
        if (access(buff, F_OK | X_OK) == 0) {
            rv = 0;
	    strcpy(p_path, buff);
            break;
        }
        path = strtok(NULL, ":");
    }
    free(vpath);
    return rv;
}

/**
 * @brief loads the first non-empty  line from a file, trimmed from its terminating linefeed character
 * @param filepath the path for the file to load and read
 * @param buffer the char buffer where to store the content, untouched if the file does not exist
 * @param buflen the buffer size, if the line found is bigger than this, it ends up truncated
 * @return = 0, if a non-empty line was found and stored in the buffer
 *         < 0, error code, -2 if the file was not found, -1 if no non-empty line was found in the file
 */
int lcrt_floadline(char *filepath, char *buffer, int buflen) {
  FILE *fp;
  char tmpbuf[256];
  int len;
  int rv = -2;
  if ((fp = fopen(filepath, "r")) != NULL) {
    rv = -1;
    while (fgets(tmpbuf, sizeof(tmpbuf), fp)) {
      len = strlen(tmpbuf);
      if (len>0 && tmpbuf[len-1] == '\n') {
	tmpbuf[len-1]=0;
      }
      if (strlen(tmpbuf)>0) {
	strncpy(buffer, tmpbuf, buflen);
	rv = 0;
	break;
      }
    }
    fclose(fp);
  }
  return rv;
}
