#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([h5gen], [0.1], [edward.baudrez@gmail.com])
AC_CONFIG_SRCDIR([h5gen.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
AM_SILENT_RULES
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PATH_PROG([DIFF], [diff])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# HDF5 library
AX_WITH_HDF5([], [AC_MSG_ERROR([cannot find HDF5 library])])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([verify.sh], [chmod +x verify.sh])
AC_OUTPUT
