# AX_WITH_HDF5([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
# ------------------------------------------------------
#
# Upon success, sets the variables HDF5_LDFLAGS and HDF5_CPPFLAGS.
AC_DEFUN([AX_WITH_HDF5],[dnl
AC_ARG_WITH([hdf5],
            [AS_HELP_STRING([--with-hdf5=DIR],
                            [specify path to HDF5 library])])
AS_IF([test "x$with_hdf5" != x],
      [AS_IF([test -d "$with_hdf5/lib64"],
             [HDF5_LDFLAGS="-L$with_hdf5/lib64 -Wl,-rpath,$with_hdf5/lib64"],
             [HDF5_LDFLAGS="-L$with_hdf5/lib -Wl,-rpath,$with_hdf5/lib"])
       HDF5_CPPFLAGS="-I$with_hdf5/include"
       HDF5_BIN="$with_hdf5/bin"])
ax_have_hdf5=yes
ax_saved_LDFLAGS=$LDFLAGS
ax_saved_CPPFLAGS=$CPPFLAGS
ax_saved_PATH=$PATH
LDFLAGS="$HDF5_LDFLAGS $LDFLAGS"
CPPFLAGS="$HDF5_CPPFLAGS $CPPFLAGS"
PATH="$HDF5_BIN:$PATH"
AC_CHECK_LIB([hdf5], [H5Fopen], [], [ax_have_hdf5=no])
AC_CHECK_HEADER([hdf5.h], [], [ax_have_hdf5=no])
AC_PATH_PROG([H5DUMP], [h5dump])
LDFLAGS=$ax_saved_LDFLAGS
CPPFLAGS=$ax_saved_CPPFLAGS
PATH=$ax_saved_PATH
AS_IF([test "x$ax_have_hdf5" = xyes],
      [AC_SUBST([HDF5_LDFLAGS])
       AC_SUBST([HDF5_CPPFLAGS])
       $1],
      [$2])
])
