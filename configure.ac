AC_INIT([bled], [0.2.0], [https://github.com/pbatard/bled/issues], [bled], [https://github.com/pbatard/bled])
AM_INIT_AUTOMAKE([-Wno-portability foreign no-dist no-dependencies])
AC_CONFIG_SRCDIR([src/bled.c])
AC_CONFIG_MACRO_DIR([m4])
AM_SILENT_RULES([yes])

AC_PREREQ([2.50])
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_SED
AC_PATH_PROG(RM, rm, rm)
AC_CHECK_TOOL(STRIP, strip, strip)
AC_C_INLINE
AC_DEFINE([_GNU_SOURCE], [], [Use GNU extensions])

# Clang needs an explicit WIN32_WINNT defined else it produces warnings
# in msapi_utf8.h - including winver.h only doesn't work
AM_CFLAGS="${AM_CFLAGS} -DWINVER=0x501 -D_WIN32_IE=0x501 -D_WIN32_WINNT=0x501"
AM_LDFLAGS="${AM_LDFLAGS} -Wl,-no-undefined"

# Debug symbols
AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug],
		[keep debug symbols for gdb (default=yes)])],
	[debug_enabled=$enableval],
	[debug_enabled='yes'])
if test "x$debug_enabled" != "xno" ; then
  CFLAGS="-g -O0"
else
  CFLAGS="-Os"
  LDFLAGS="-s"
fi

AC_MSG_RESULT([enabling Large File Support])
AM_CFLAGS="$AM_CFLAGS -D_FILE_OFFSET_BITS=64 -D_OFF_T_ -D_off_t=off64_t -Doff_t=off64_t -Doff32_t=long"

# check for -Wno-pointer-sign compiler support (GCC >= 4)
saved_CFLAGS="${CFLAGS}"
CFLAGS="$CFLAGS -Wno-pointer-sign"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
	[nopointersign_cflags="-Wno-pointer-sign"], [nopointersign_cflags=""])
CFLAGS="${saved_CFLAGS}"

AM_CFLAGS="$AM_CFLAGS -std=gnu99 -Wshadow -Wall -Wundef -Wunused -Wstrict-prototypes -Werror-implicit-function-declaration $nopointersign_cflags"

AC_SUBST([VISIBILITY_CFLAGS])
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])
AC_SUBST([SUFFIX])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([test/Makefile])
AC_OUTPUT
