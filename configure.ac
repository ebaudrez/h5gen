#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([h5gen], [0.1], [edward.baudrez@gmail.com])
AC_CONFIG_SRCDIR([h5gen.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PATH_PROG([DIFF], [diff])

# Checks for libraries.

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([inttypes.h libintl.h malloc.h stddef.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strdup strerror])

# HDF5 library
AX_WITH_HDF5([], [AC_MSG_ERROR([cannot find HDF5 library])])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([verify_ddl.sh], [chmod +x verify_ddl.sh])
AC_CONFIG_FILES([verify_h5.sh], [chmod +x verify_h5.sh])
AC_OUTPUT
