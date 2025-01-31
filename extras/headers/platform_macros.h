#if ! defined PLATFORM_MACROS_H
#define PLATFORM_MACROS_H

/*
 *  This file is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  It is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this file; if not, write to the Free Software Foundation, Inc., 59
 *  Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          platform_macros.h
 *
 *  Provides a rationale and a set of macros to make compile-time
 *  decisions covering Windows versus Linux, GNU versus Microsoft, and
 *  MINGW versus GNU.
 *
 * \library       Any application or library
 * \author        Chris Ahlstrom
 * \date          2015-07-24
 * \updates       2024-03-11
 * \license       GNU GPLv2 or above
 *
 *  Copyright (C) 2013-2024 Chris Ahlstrom <ahlstromcj@gmail.com>
 *
 *  We need a uniform way to specify OS and compiler features without
 *  littering the code with macros from disparate compilers.  Put all
 *  the compiler-specific stuff here to define "PLATFORM" macros.
 *
 * Determining useful macros:
 *
 *    -  GNU:  cpp -dM myheaderfile
 *
 * Settings to distinguish, based on compiler-supplied macros:
 *
 *    -  Platform macros:
 *       -  Windows
 *       -  Linux
 *       -  MacOSX
 *    -  Platform macros (in the absense of Windows, Linux macros):
 *       -  PLATFORM_WINDOWS
 *       -  PLATFORM_WINDOWS_32
 *       -  PLATFORM_WINDOWS_64
 *       -  PLATFORM_WINDOWS_UNICODE
 *       -  PLATFORM_LINUX
 *       -  PLATFORM_FREEBSD
 *       -  PLATFORM_MACOSX
 *       -  PLATFORM_IPHONE_OS (to do!)
 *       -  PLATFORM_UNIX
 *    -  Architecture size macros:
 *       -  PLATFORM_32_BIT
 *       -  PLATFORM_64_BIT
 *    -  Debugging macros:
 *       -  PLATFORM_DEBUG
 *       -  PLATFORM_RELEASE
 *    -  Compiler:
 *       -  PLATFORM_CLANG
 *       -  PLATFORM_MSVC (alternative to _MSC_VER)
 *       -  PLATFORM_GNU
 *       -  PLATFORM_XSI
 *       -  PLATFORM_MINGW
 *       -  PLATFORM_MING_OR_WINDOWS
 *       -  PLATFORM_CYGWIN
 *    -  API:
 *       -  PLATFORM_POSIX_API (alternative to POSIX)
 *    -  Language:
 *       -  PLATFORM_CPP_11
 *       -  PLATFORM_CPP_14
 *       -  PLATFORM_CPP_17
 *
 *  http://stackoverflow.com/questions/11053960/
 *      how-are-the-cplusplus-directive-defined-in-various-compilers
 *
 *    The 199711L stands for Year=1997, Month = 11 (i.e., November of 1997) --
 *    the date when the committee approved the standard that the rest of the
 *    ISO approved in early 1998.
 *
 *    For the 2003 standard, there were few enough changes that the committee
 *    (apparently) decided to leave that value unchanged.
 *
 *    For the 2011 standard, it's defined as 201103L, (year=2011, month = 03),
 *    meaning that the committee approved the standard as finalized in March
 *    of 2011.
 *
 *    For the 2014 standard, it's defined as 201402L, interpreted the same way
 *    as above (February 2014).
 *
 *    For the 2017 standard, it's defined as 201703L, interpreted the same way
 *    as above (March 2017).
 *
 *    Before the original standard was approved, quite a few compilers
 *    normally defined it as 0 (or just an empty definition like #define
 *    __cplusplus) to signify "not-conforming". When asked for their strictest
 *    conformance, many defined it to 1.  Ancient news!
 */

#undef PLATFORM_32_BIT
#undef PLATFORM_64_BIT
#undef PLATFORM_CLANG
#undef PLATFORM_CPP_11
#undef PLATFORM_CPP_14
#undef PLATFORM_CPP_17
#undef PLATFORM_CYGWIN
#undef PLATFORM_DEBUG
#undef PLATFORM_FREEBSD
#undef PLATFORM_GNU
#undef PLATFORM_IPHONE_OS
#undef PLATFORM_LINUX
#undef PLATFORM_MACOSX
#undef PLATFORM_MINGW
#undef PLATFORM_MING_OR_WINDOWS
#undef PLATFORM_MSVC
#undef PLATFORM_POSIX_API
#undef PLATFORM_RELEASE
#undef PLATFORM_UNIX
#undef PLATFORM_WIN32_STRICT
#undef PLATFORM_WINDOWS
#undef PLATFORM_WINDOWS_32
#undef PLATFORM_WINDOWS_64
#undef PLATFORM_WINDOWS_UNICODE
#undef PLATFORM_XSI

/**
 *  Provides a "Windows" macro, in case the environment doesn't provide
 *  it.  This macro is defined if not already defined and _WIN32 or WIN32
 *  are encountered.
 */

#if defined Windows                     /* defined by nar-maven-plugin      */
#define PLATFORM_WINDOWS
#else

#if defined _WIN32                      /* defined by Microsoft compiler    */
#define PLATFORM_WINDOWS_32
#define PLATFORM_WINDOWS
#define Windows
#define PLATFORM_32_BIT
#else
#if defined WIN32                       /* defined by Mingw compiler        */
#define PLATFORM_WINDOWS_32
#define PLATFORM_WINDOWS
#define Windows
#define PLATFORM_32_BIT
#endif
#endif

#if defined _WIN64                      /* defined by Microsoft compiler    */
#define PLATFORM_WINDOWS_64
#define PLATFORM_WINDOWS
#define Windows
#define PLATFORM_64_BIT
#else
#if defined WIN64                       /* defined by Mingw compiler        */
#define PLATFORM_WINDOWS_64
#define PLATFORM_WINDOWS
#define Windows
#define PLATFORM_64_BIT
#endif
#endif

#endif

/**
 *  FreeBSD macros.
 */

#if defined __FreeBSD__
#define PLATFORM_FREEBSD
#define PLATFORM_PTHREADS
#define PLATFORM_UNIX
#endif

/**
 *  Provides a "Linux" macro, in case the environment doesn't provide it.
 *  This macro is defined if not already defined and XXXXXX is
 *  encountered.
 */

#if defined Linux                      /* defined by nar-maven-plugin       */
#define PLATFORM_LINUX
#else
#if defined __linux__                  /* defined by the GNU compiler       */
#define Linux
#define PLATFORM_LINUX
#endif
#endif

#if defined PLATFORM_LINUX

#if ! defined POSIX
#define POSIX                          /* defined for legacy code purposes  */
#endif

#define PLATFORM_POSIX_API
#define PLATFORM_PTHREADS
#define PLATFORM_UNIX

#endif                                 /* PLATFORM_LINUX              */

/**
 *  Provides a "MacOSX" macro, in case the environment doesn't provide it.
 *  This macro is defined if not already defined and __APPLE__ and
 *  __MACH__ are encountered.
 */

#if defined MacOSX
#define PLATFORM_MACOSX
#else
#if defined __APPLE__ && defined __MACH__    /* defined by Apple compiler   */
#define PLATFORM_MACOSX
#define MacOSX
#endif
#endif

#if defined PLATFORM_MACOSX
#define PLATFORM_UNIX
#endif

/*
 *  To do: detect the iOS platform.
 *
 * #define PLATFORM_IPHONE_OS
 */

/**
 *  Provides macros that mean 32-bit, and only 32-bit Windows.  For
 *  example, in Windows, _WIN32 is defined for both 32- and 64-bit
 *  systems, because Microsoft didn't want to break people's 32-bit code.
 *  So we need a specific macro.
 *
 *      -  PLATFORM_32_BIT is defined on all platforms.
 *      -  WIN32 is defined on Windows platforms.
 *
 *  Prefer the former macro.  The second is defined only for legacy
 *  purposes for Windows builds, and might eventually disappear.
 */

#if defined PLATFORM_WINDWS
#if defined _WIN32 && ! defined _WIN64

#if ! defined WIN32
#define WIN32                          /* defined for legacy purposes        */
#endif

#if ! defined PLATFORM_32_BIT
#define PLATFORM_32_BIT
#endif

#endif
#endif                                 /* PLATFORM_WINDOWS            */

/**
 *  Provides macros that mean 64-bit, and only 64-bit.
 *
 *      -  PLATFORM_64_BIT is defined on all platforms.
 *      -  WIN64 is defined on Windows platforms.
 *
 *  Prefer the former macro.  The second is defined only for legacy
 *  purposes for Windows builds, and might eventually disappear.
 *
 */

#if defined PLATFORM_WINDWS
#if defined _WIN64

#if ! defined WIN64
#define WIN64
#endif

#if ! defined PLATFORM_64_BIT
#define PLATFORM_64_BIT
#endif

#endif
#endif                                 /* PLATFORM_WINDOWS            */

/**
 *  Provides macros that mean 64-bit versus 32-bit when gcc or g++ are
 *  used. This can occur on Linux and other systems, and with mingw on
 *  Windows.
 *
 *      -  PLATFORM_64_BIT is defined on all platforms.
 *
 *  Prefer the former macro.  The second is defined only for legacy
 *  purposes for Windows builds, and might eventually disappear.
 */

#if defined __GNUC__
#if defined __x86_64__ || __ppc64__

#if ! defined PLATFORM_64_BIT
#define PLATFORM_64_BIT
#endif

#else

#if ! defined PLATFORM_32_BIT
#define PLATFORM_32_BIT
#endif

#endif
#endif

/**
 *  Provides macros that indicate if Microsoft C/C++ versus GNU are being
 *  used.  THe compiler being used normally provides test macros for itself.
 *
 *      -  PLATFORM_ClANG (replaces __clang__)
 *      -  PLATFORM_MSVC (replaces _MSC_VER)
 *      -  PLATFORM_GNU (replaces __GNUC__)
 *      -  PLATFORM_MINGW (replaces __MINGW32__)
 *      -  PLATFORM_CYGWIN
 */

#if defined __clang__
#define PLATFORM_CLANG
#endif

#if defined _MSC_VER
#define PLATFORM_MSVC
#define PLATFORM_WINDOWS
#endif

#if defined __GNUC__
#define PLATFORM_GNU __GNUC__
#endif

#if (_POSIX_C_SOURCE >= 200112L) && ! _GNU_SOURCE
#define PLATFORM_XSI

/*
 * Hit this one compiling in Qt Creator on Linux.
 *
 * #error XSI defined, this is just a test
 */

#endif

/*
 * PLATFORM_WIN32_STRICT replaces checks for WIN32 with CYGWIN not defined.
 */

#if defined __CYGWIN__ || __CYGWIN32__
#define PLATFORM_CYGWIN
#define PLATFORM_WINDOWS_32
#else
#if defined PLATFORM_WINDOWS_32
#define PLATFORM_WIN32_STRICT
#endif
#endif

#if defined __MINGW32__
#define PLATFORM_MINGW
#define PLATFORM_WINDOWS
#define PLATFORM_WINDOWS_32
#endif

#if defined __MINGW64__
#define PLATFORM_MINGW
#define PLATFORM_WINDOWS
#define PLATFORM_WINDOWS_64
#endif

#if defined PLATFORM_WINDOWS

/*
 *  Without this #define, the InitializeCriticalSectionAndSpinCount() function
 *  is undefined.  This version level means "Windows 2000 and higher".
 *  For Windows 10, the value would be 0x0A00.
 */

#if ! defined _WIN32_WINNT
#define _WIN32_WINNT        0x0500
#endif

#if defined UNICODE || defined _UNICODE
#define PLATFORM_WINDOWS_UNICODE
#endif

#endif  // defined PLATFORM_WINDOWS

/**
 *  Provides a way to flag unused parameters at each "usage", without disabling
 *  them globally.  Use it like this:
 *
 *     void foo(int UNUSED(bar)) { ... }
 *     static void UNUSED_FUNCTION(foo)(int bar) { ... }
 *
 *  The UNUSED macro won't work for arguments which contain parenthesis,
 *  so an argument like float (*coords)[3] one cannot do,
 *
 *      float UNUSED((*coords)[3]) or float (*UNUSED(coords))[3].
 *
 *  This is the only downside to the UNUSED macro; in these cases fall back to
 *
 *      (void) coords;
 *
 *  Another possible definition is casting the unused value to void in the
 *  function body.
 */

#if defined __GNUC__
#define UNUSED(x)               UNUSED_ ## x __attribute__((__unused__))
#else
#define UNUSED(x)               UNUSED_ ## x
#endif

#if defined __GNUC__
#define UNUSED_FUNCTION(x)      __attribute__((__unused__)) UNUSED_ ## x
#else
#define UNUSED_FUNCTION(x)      UNUSED_ ## x
#endif

#define UNUSED_VOID(x)          (void) (x)

/**
 *  Provides macros to indicate the level standards support for some key
 *  cases.  We may have to play with this a bit to get it right.  The main
 *  use-case right now is in avoiding defining the nullptr macro in C++11.
 */

#if defined PLATFORM_MSVC

#if _MSC_VER >= 1700                /* __cplusplus value doesn't work, MS!  */
#define PLATFORM_CPP_11
#endif

#else

#if __cplusplus >= 201103L          /* i.e. C++11                           */
#define PLATFORM_CPP_11
#endif

#if __cplusplus >= 201402L          /* i.e. C++14                           */
#define PLATFORM_CPP_14
#endif

#if __cplusplus >= 201703L          /* i.e. C++17                           */
#define PLATFORM_CPP_17
#endif

#endif

/**
 *  Kind of a Windows-with-MingW-matching-Visual-Studio macro.
 */

#if defined PLATFORM_MSVC || defined PLATFORM_MINGW
#define PLATFORM_MING_OR_WINDOWS
#endif

/**
 *  A UNIX or MingW macro.
 */

#if defined PLATFORM_UNIX || defined PLATFORM_MINGW
#define PLATFORM_MING_OR_UNIX
#endif

/**
 *  Provides macros that mean "debugging enabled".
 *
 *      -  PLATFORM_DEBUG or PLATFORM_RELEASE
 *      -  DEBUG or NDEBUG for legacy usage
 *
 * Prefer the former macro.  The second is defined only for legacy
 * purposes for Windows builds, and might eventually disappear.
 */

#if ! defined PLATFORM_DEBUG
#if defined DEBUG || defined _DEBUG || defined _DEBUG_ || \
 defined __DEBUG || defined __DEBUG__
#define PLATFORM_DEBUG
#endif
#endif

#if ! defined PLATFORM_DEBUG && ! defined PLATFORM_RELEASE
#define PLATFORM_RELEASE
#endif

#endif                  /* PLATFORM_MACROS_H */

/*
 * platform_macros.h
 *
 * vim: ts=4 sw=4 wm=4 et ft=c
 */

