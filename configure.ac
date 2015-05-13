#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([h5gen], [0.1], [edward.baudrez@gmail.com])
AC_CONFIG_SRCDIR([h5gen.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
AM_SILENT_RULES

# Checks for programs.
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PATH_PROG([DIFF], [diff])
AC_PATH_PROG([H5DUMP], [h5dump])

# Checks for libraries.
AC_CHECK_LIB([hdf5], [H5Fopen])

# Checks for header files.
AC_CHECK_HEADER([hdf5.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([verify.sh], [chmod +x verify.sh])
AC_OUTPUT
