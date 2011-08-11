#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_define([gt_version_major],[1])
m4_define([gt_version_minor],[1])
m4_define([gt_version_micro],[0])
m4_define([gt_version_extra],[])
m4_define([gt_version],[gt_version_major().gt_version_minor().gt_version_micro()])

#AC_PREREQ([2.63])
AC_INIT([lcrt], [gt_version], [niutao0602@gmail.com])
AC_CONFIG_SRCDIR(
    [src/Makefile.am],
    [language/Makefile.am],
    [help/Makefile.am],
    [pixmaps/Makefile.am],
    [gnome/Makefile.am])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.9 foreign])
AC_PREFIX_DEFAULT(/usr)
#GNOME_INIT
check_deps()
{
	echo "$LCRT_LIB_DEPS_PKG_ERRORS"
	echo "If the packages is not found,you can install them with follow command:"
	echo "In Unbutu/Debian system:"
	echo "    sudo apt-get install libgtk2.0-dev libvte-dev libsqlite3-dev"
	echo "In Fedora/CentOS:"
	echo "    sudo yum install gtk-devel vte-devel sqlite3-devel"
	echo ""
	exit 1
}
PKG_CHECK_MODULES(LCRT_LIB_DEPS,
    gtk+-2.0 >= 2.0.0
    vte >= 0.22.2
    sqlite3 >= 3.0.0
	,,check_deps)
# Checks for programs.
AC_PROG_CC
AC_ISC_POSIX

# Checks for libraries.
AC_PROG_RANLIB
AM_PROG_INSTALL_STRIP

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h libintl.h limits.h stddef.h stdint.h stdlib.h string.h sys/file.h sys/ioctl.h sys/mount.h sys/param.h sys/time.h unistd.h sqlite3.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_TYPE_UINT8_T
AC_CHECK_TOOL([STRIP],[strip])
# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([clock_gettime fdatasync ftruncate getcwd gettimeofday localtime_r memmove memset mkdir munmap select strchr strerror strstr])

#debug options support
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],[debug program(default is no)])],
    [CFLAGS="${CFLAGS} -D__LCRT_DEBUG__ -Wall"])

AC_CONFIG_FILES([
    Makefile 
    src/Makefile 
    language/Makefile
    help/Makefile
    help/man/Makefile
    help/man/lcrt.1
    pixmaps/Makefile
    gnome/Makefile])
AC_OUTPUT


echo "
lcrt-$VERSION:

	prefix:                 ${prefix}
	source code location:   ${srcdir}
	compiler:               ${CC}

"
