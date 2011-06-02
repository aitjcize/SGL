#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([SimpleGL], [0.1.0], [aitjcize@gmail.com])
AC_CONFIG_AUX_DIR([autotools])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_SRCDIR([src/math/m_matrix.c])
AC_CONFIG_HEADERS([config.h])

# SGL Version
LIBSGL_CURRENT=1
LIBSGL_REVISION=0
LIBSGL_AGE=0

AC_SUBST(LIBSGL_CURRENT)
AC_SUBST(LIBSGL_REVISION)
AC_SUBST(LIBSGL_AGE)

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_LIBTOOL
AM_PROG_CC_C_O
AC_SUBST(LIBTOOL_DEPS)

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset sqrt])

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 src/sgl/Makefile
		 src/sglut/Makefile
		 ])

AC_OUTPUT
