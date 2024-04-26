#if ! defined C_MACROS_H
#define C_MACROS_H

/*
 *  This file is part of xpc66.
 *
 *  xpc66 is free software; you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation; either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  xpc66 is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with xpc66; if not, write to the Free Software Foundation, Inc., 59 Temple
 *  Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          c_macros.h
 *
 *    This module provides a very simple set of C/C++ macros.
 *
 * \library       Any application or library
 * \author        Chris Ahlstrom and other authors; see documentation
 * \date          2022-06-08
 * \updates       2024-04-26
 * \license       GNU GPL v2 or above
 *
 *  This file defines a minimal set of convenience macros for both C and C++11
 *  (or greater) code.
 */

#if ! defined __cplusplus
#include <stdio.h>
#endif

/**
 *  An undefined macro purely to emphasize a C/C++ "{}" scope used for various
 *  exception-safe locking mechanisms.
 */

#define XPC66_SCOPE_LOCK

#if ! defined __cplusplus

#define nullptr                 0
#define override
#define noexcept

#endif                                  /* defined __cplusplus  */

/**
 *    Provides a way to declare functions as having either a C++ or C
 *    interface.
 */

#if ! defined EXTERN_C_DEC

#if defined __cplusplus
#define EXTERN_C_DEC extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_DEC
#define EXTERN_C_END
#endif

#endif

#if ! defined not_NULL

/**
 *    Test for being a valid pointer.  The not_NULL() macro is meant for C
 *    code that returns NULL.
 */

#define not_NULL(x)                 ((x) != NULL)
#define not_nullptr(x)              ((x) != nullptr)
#define not_nullptr_2(x1, x2)       ((x1) != nullptr && (x2) != nullptr)

/**
 *    Test for being an invalid pointer.  The is_NULL() macro is meant for C
 *    code that returns NULL.
 */

#define is_NULL(x)                  ((x) == NULL)
#define is_nullptr(x)               ((x) == nullptr)
#define is_nullptr_2(x1, x2)        ((x1) == nullptr || (x2) == nullptr)
#define both_are_nullptr(x1, x2)    ((x1) == nullptr && (x2) == nullptr)

/**
 *  A macro that awaits the experiments std::observer_ptr type.
 *  All it does is make it obvious we should not delete the pointer.
 */

#define observer_ptr(type)          type *

#endif  // ! defined not_NULL

/**
 *  Provides a clearer specification than bare true/false. Needed for both C
 *  and C++ code.
 */

typedef enum
{
    opt_disabled    = 0,
    opt_enabled     = 1,
    opt_readonly    = 2,
    opt_deprecated  = 3,
    opt_obsolete    = 4

} opt_flags;

#if ! defined __cplusplus

/**
 *  Provides the "false" and "true" values of the bool type definition for
 *  C code.
 */

#define false    0
#define true     1

#endif          // __cplusplus

/**
 *    This macro tests the integer value against a success return value (i.e.
 *    zero).
 *
 *    Other related macros are:
 *
 *       -  is_posix_success()
 *       -  is_posix_error()
 *       -  not_posix_success()
 *       -  not_posix_error()
 *       -  set_posix_success()
 *       -  set_posix_error()
 *
 * \note
 *      -   Some functions return values other than error (-1)
 *          when an error occurs.
 *      -   Some functions return values other than success (0)
 *          when the function succeeds.
 *      -   Please refer to the online documentation for these quixotic
 *          functions, and decide which macro one want to use for the test, if
 *          any.
 *      -   In some case, one might want to use a clearer test.  For example,
 *          the socket functions return a result that is
 *          PLATFORM_POSIX_ERROR (-1) if the function fails, but
 *          non-zero integer values are returned if the function succeeds.
 *          For these functions, the is_valid_socket() and not_valid_socket()
 *          macros are much more appropriate to use.
 *
 *//*-------------------------------------------------------------------------*/

#if ! defined is_posix_success
#define is_posix_success(x)      ((x) == 0)
#endif

/**
 *  This macro tests the integer value against (-1).
 */

#if ! defined is_posix_error
#define is_posix_error(x)        ((x) == (-1))
#endif

/**
 *  This macro tests the integer value against (0).
 */

#if ! defined not_posix_success
#define not_posix_success(x)     ((x) != 0)
#endif

/**
 *  This macro tests the integer value against (-1).
 */

#if ! defined not_posix_error
#define not_posix_error(x)       ((x) != (-1))
#endif

/**
 *  This macro set the integer value to "success" (0).  The
 *  parameter must be an lvalue, as the assignment operator is used.
 */

#if ! defined set_posix_success
#define set_posix_success(x)     ((x) = 0)
#endif

/**
 *  This macro set the integer value to "error" (-1).  The
 *  parameter must be an lvalue, as the assignment operator is used.
 */

#if ! defined set_posix_error
#define set_posix_error(x)       ((x) = (-1))
#endif

/**
 *    GCC provides three magic variables which hold the name of the current
 *    function, as a string.  The first of these is `__func__', which is part
 *    of the C99 standard:
 *
 *    The identifier `__func__' is implicitly declared by the translator.  It
 *    is the name of the lexically-enclosing function.  This name is the
 *    unadorned name of the function.
 *
 *    `__FUNCTION__' is another name for `__func__'.  Older versions of GCC
 *    recognize only this name.  However, it is not standardized.  For maximum
 *    portability, use `__func__', but provide a fallback definition with
 *    the preprocessor, as done below.
 *
 *    `__PRETTY_FUNCTION__' is the decorated version of the function name.
 *    It is longer, but more informative.  It is also deprecated.  But, for
 *    now, we'll continue to use it
 *
 *    Visual Studio defines only __FUNCTION__, so a definition is provided
 *    below.
 */

#if defined __GNUC__                    /* but see platform_macros.h too    */

#if ! defined __func__
#if __STDC_VERSION__ < 199901L
#if __GNUC__ >= 2

/**
 *  Function names.
 */

#define __func__    __FUNCTION__        // bald func names

#else
#define __func__    "<unknown>"
#endif                                  // __GNUC__ >= 2
#endif                                  // __STDC_VERSION__
#endif                                  // __func__

#else                                   // ! __GNUC__

#if ! defined __func__
#define __func__    __FUNCTION__        // Windows?
#endif

#endif                                  // __GNUC__

/**
 *    Provides simple reporting macros.
 */

#define errprint(x)             fprintf(stderr, "%s\n", x)
#define errprintf(fmt, x)       fprintf(stderr, fmt, x)
#define errprintfunc(x)         fprintf(stderr, "%s: %s\n", __func__, x)

#define warnprint(x)            fprintf(stderr, "%s\n", x)
#define warnprintf(fmt, x)      fprintf(stderr, fmt, x)

#define infoprint(x)            fprintf(stdout, "%s\n", x)
#define infoprintf(fmt, x)      fprintf(stdout, fmt, x)
#define infoprintfunc()         fprintf(stdout, "%s\n", __func__)

#endif          // C_MACROS_H

/*
 * c_macros.h
 *
 * vim: sw=4 ts=4 wm=4 et ft=c
 */
