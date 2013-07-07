/**
 * @file    foperate.h
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

#ifndef __LCRT_FOPERATE_H__
#define __LCRT_FOPERATE_H__
/**
 * @brief  Create directory from root, if the parent directory is not exist,
 *         then make it until last one.
 * @param dir path or directory which want to create
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fmkdir(const char *dir);

/**
 * @brief  copy a file from fsrc to fdst
 * @param fsrc the source file name which will be copy
 * @param fdst the target file name which will be generate
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fcopy(const char *fsrc, const char *fdst);

/**
 * @brief  copy a directory from another
 * @param dsrc the source directory name
 * @param ddst the target directory name
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fdircopy(const char *dsrc, const char *ddst);

/**
 * @brief  move a file to another path
 * @param fsrc the source file name
 * @param fdst the target file name
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fmove(const char *fsrc, const char *fdst);

/**
 * @brief  delete a file
 * @param fname the file name which will be deleted
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_remove(const char *fname);

/**
 * @brief  print some content to the end of file
 * @param fname the file name
 * @param format the format of content
 * @param ... the argument
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fappend(const char *fname, const char *format, ...);

/**
 * @brief  create a file
 * @param fname the file name which will be created
 * @return = 0, success\n
 *         < 0, error code
 */
int lcrt_fcreate(const char *fname);

/**
 * @brief  check whether the executive programe is exist in directory 'PATH'
 * @param prog the executive programe name
 * @param p_path if found, store path to p_path
 * @return = 0, exist in directory 'PATH'
 *         < 0, error code
 */
int lcrt_echeck(const char *prog, char *p_path);

/**
 * @brief loads the first non-empty  line from a file, trimmed from its terminating linefeed character
 * @param filepath the path for the file to load and read
 * @param buffer the char buffer where to store the content, untouched if the file does not exist
 * @param buflen the buffer size, if the line found is bigger than this, it ends up truncated
 * @return = 0, if a non-empty line was found and stored in the buffer
 *         < 0, error code, -2 if the file was not found, -1 if no non-empty line was found in the file
 */
int lcrt_floadline(char *filepath, char *buffer, int buflen);
#endif
