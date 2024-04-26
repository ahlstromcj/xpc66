#if ! defined CPP_TYPES_HPP
#define CPP_TYPES_HPP

/*
 *  This file is part of xpc66.
 *
 *  xpc66 is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  xpc66 is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with xpc66; if not, write to the Free Software Foundation, Inc., 59
 *  Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          cpp_types.hpp
 *
 *    This module provides some commonly-used and useful C++ aliases.
 *
 * \library       Any application or library
 * \author        Chris Ahlstrom
 * \date          2022-07-01
 * \updates       2024-04-26
 * \license       GNU GPL v2 or above
 *
 *  This file defines a minimal set of convenience macros for both C and C++11
 *  (or greater) code.
 */

#include "c_macros.h"

#if defined __cplusplus

/*
 * namespace xyz
 *
 *      This module currently defines no namespace as it is common (and
 *      copied) in many "66" libraries.
 */

#include <string>                       /* std::string class                */
#include <vector>                       /* std::vector class                */

/**
 *  A kind of tribool, it adds a "toggle" option. Better than sending
 *  multiple boolean parameters.
 */

enum class toggler
{
    off,                /**< A request to turn a state boolean to false.    */
    on,                 /**< A request to turn a state boolean to true.     */
    flip                /**< A request to toggle a state boolean.           */
};

/**
 *  An indicator of message level.  Used in the message functions defined
 *  in the msgfunctions C++ module, and elsewhere to specify the level.
 *
 *      -   none. Used only to revert back to no color in message functions.
 *      -   info. Messages that should appear only in verbose mode.
 *      -   warn. Message about problems or statuses that are minor.
 *      -   error. More serious problems.
 *      -   status. Messages that show the progesss of the application even
 *          when not in verbose mode.
 *      -   session. Messages that need to stand out in session management.
 *      -   debug. Message that should appear only in investigate mode.
 */

enum class msglevel
{
    none,           /* default console color    */
    info,           /* blue                     */
    warn,           /* yellow                   */
    error,          /* red                      */
    status,         /* green                    */
    session,        /* cyan                     */
    debug           /* debug                    */
};

/**
 *  Provides an easy-to-search container for strings.  It is useful when
 *  breaking a line of text into individual tokens.
 */

using tokenization = std::vector<std::string>;

#endif

#endif          // CPP_TYPES_HPP

/*
 * cpp_types.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

