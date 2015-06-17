# AX_WITH_HDF5([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
# ------------------------------------------------------
#
# Upon success, sets the variables HDF5_LDFLAGS, HDF5_CPPFLAGS, H5DUMP, and
# H5DIFF.
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
AX_PROG_H5DIFF
LDFLAGS=$ax_saved_LDFLAGS
CPPFLAGS=$ax_saved_CPPFLAGS
PATH=$ax_saved_PATH
AS_IF([test "x$ax_have_hdf5" = xyes],
      [AC_SUBST([HDF5_LDFLAGS])
       AC_SUBST([HDF5_CPPFLAGS])
       $1],
      [$2])
])

# AX_PROG_H5DIFF
# --------------
#
# Look for a suitable implementation of h5diff (older versions ignore
# attributes). Sets H5DIFF on success.
AC_DEFUN([AX_PROG_H5DIFF], [dnl
AC_PATH_PROG([H5DIFF], [h5diff])
AC_CACHE_CHECK([whether h5diff accepts the test files], [ax_cv_prog_h5diff_testfiles],
               [ax_cv_prog_h5diff_testfiles=yes
                $ac_cv_path_H5DIFF $srcdir/m4/base.h5 $srcdir/m4/base.h5 >/dev/null || ax_cv_prog_h5diff_testfiles=no])
AC_CACHE_CHECK([whether h5diff requires options at the end], [ax_cv_prog_h5diff_optatend],
               [ax_cv_prog_h5diff_optatend=yes
                $ac_cv_path_H5DIFF -q $srcdir/m4/base.h5 $srcdir/m4/base.h5 >/dev/null && ax_cv_prog_h5diff_optatend=no])
AC_CACHE_CHECK([whether h5diff detects differences], [ax_cv_prog_h5diff_testdiff],
               [ax_cv_prog_h5diff_testdiff=yes
                $ac_cv_path_H5DIFF $srcdir/m4/base.h5 $srcdir/m4/dset.h5 >/dev/null && ax_cv_prog_h5diff_testdiff=no])
AC_CACHE_CHECK([whether h5diff -d works], [ax_cv_prog_h5diff_testd],
               [ax_cv_prog_h5diff_testd=yes
                if test "x$ax_cv_prog_h5diff_optatend" = xyes; then
                    $ac_cv_path_H5DIFF $srcdir/m4/base.h5 $srcdir/m4/dset.h5 -d 0.0001 >/dev/null && ax_cv_prog_h5diff_testd=no
                    $ac_cv_path_H5DIFF $srcdir/m4/base.h5 $srcdir/m4/dset.h5 -d 0.001  >/dev/null || ax_cv_prog_h5diff_testd=no
                else
                    $ac_cv_path_H5DIFF -d 0.0001 $srcdir/m4/base.h5 $srcdir/m4/dset.h5 >/dev/null && ax_cv_prog_h5diff_testd=no
                    $ac_cv_path_H5DIFF -d 0.001  $srcdir/m4/base.h5 $srcdir/m4/dset.h5 >/dev/null || ax_cv_prog_h5diff_testd=no
                fi])
AC_CACHE_CHECK([whether h5diff -p works], [ax_cv_prog_h5diff_testp],
               [ax_cv_prog_h5diff_testp=yes
                if test "x$ax_cv_prog_h5diff_optatend" = xyes; then
                    $ac_cv_path_H5DIFF $srcdir/m4/base.h5 $srcdir/m4/dset.h5 -p 0.00001 >/dev/null && ax_cv_prog_h5diff_testp=no
                    $ac_cv_path_H5DIFF $srcdir/m4/base.h5 $srcdir/m4/dset.h5 -p 0.0001  >/dev/null || ax_cv_prog_h5diff_testp=no
                else
                    $ac_cv_path_H5DIFF -p 0.00001 $srcdir/m4/base.h5 $srcdir/m4/dset.h5 >/dev/null && ax_cv_prog_h5diff_testp=no
                    $ac_cv_path_H5DIFF -p 0.0001  $srcdir/m4/base.h5 $srcdir/m4/dset.h5 >/dev/null || ax_cv_prog_h5diff_testp=no
                fi])
AC_CACHE_CHECK([whether h5diff tests attributes], [ax_cv_prog_h5diff_testatt],
               [ax_cv_prog_h5diff_testatt=yes
                $ac_cv_path_H5DIFF $srcdir/m4/base.h5 $srcdir/m4/att.h5 >/dev/null && ax_cv_prog_h5diff_testatt=no])
ax_cv_prog_h5diff_works=yes
test "x$ax_cv_prog_h5diff_testfiles" = xyes || ax_cv_prog_h5diff_works=no
test "x$ax_cv_prog_h5diff_testdiff" = xyes || ax_cv_prog_h5diff_works=no
test "x$ax_cv_prog_h5diff_testd" = xyes || ax_cv_prog_h5diff_works=no
test "x$ax_cv_prog_h5diff_testp" = xyes || ax_cv_prog_h5diff_works=no
test "x$ax_cv_prog_h5diff_testatt" = xyes || ax_cv_prog_h5diff_works=no
AS_IF([test "x$ax_cv_prog_h5diff_works" = xyes],
      [AS_IF([test "x$ax_cv_prog_h5diff_optatend" = xyes],
             [AC_SUBST([H5DIFF], ["$ac_cv_path_H5DIFF"])
              AC_SUBST([H5DIFF_OPT], ["-r -p 0.00001"])],
             [AC_SUBST([H5DIFF], ["$ac_cv_path_H5DIFF -r -p 0.00001"])])],
      [AC_SUBST([H5DIFF], [no])])
])
