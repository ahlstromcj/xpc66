#if ! defined XPC66_BUILD_MACROS_H
#define XPC66_BUILD_MACROS_H

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
 * \file          xpc_build_macros.h
 *
 *  Macros that depend upon the build platform.
 *
 * \library       xpc66
 * \author        Gary P. Scavone; refactoring by Chris Ahlstrom
 * \date          2024-04-15
 * \updates       2024-04-15
 * \license       See above.
 *
 * Introduction:
 *
 *      We have two ways of configuring the build for the xpc66 library.  This
 *      file presents features that depend upon the build platform, and build
 *      options that might be considered permanent, such as the option to pick
 *      from various JACK-processing callbacks.
 */

#include "platform_macros.h"            /* generic detecting of OS platform */

/**
 *  To do: improve the naming
 */

#if defined XPC66_EXPORT

#if defined PLATFORM_WINDOWS || defined PLATFORM_CYGWIN

#define XPC66_DLL_PUBLIC   __declspec(dllexport)
#define XPC66_API          __declspec(dllexport)

#else

#if PLATFORM_GNUC >= 4
#define XPC66_DLL_PUBLIC   __attribute__((visibility("default")))
#define XPC66_API          __attribute__((visibility("default")))
#else
#define XPC66_API
#define XPC66_DLL_PUBLIC
#endif

#endif

#else                       /* not exporting    */

#define XPC66_API
#define XPC66_DLL_PUBLIC

#endif

#endif          // XPC66_BUILD_MACROS_H

/*
 * xpc_build_macros.h
 *
 * vim: sw=4 ts=4 wm=4 et ft=c
 */

