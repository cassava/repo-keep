#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([repo-keep], [1.9.4], [neembi@googlemail.com])
AC_CONFIG_AUX_DIR(config)

# Initialize Automake
AM_INIT_AUTOMAKE(repo-keep, 1.9.4)

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h math.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_LIB(m, ceil)
AC_CHECK_FUNCS([regcomp strchr strspn])

# What we want to output
AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile contrib/Makefile])

AC_OUTPUT
