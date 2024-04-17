#if ! defined XPC66_XPC_UTILFUNCTIONS_HPP
#define XPC66_XPC_UTILFUNCTIONS_HPP

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
 * \file          utilfuncttions.hpp
 *
 *    This module provides macros for generating simple messages, MIDI
 *    parameters, and more.
 *
 * \library       xpc66
 * \author        Chris Ahlstrom and other authors; see documentation
 * \date          2024-04-15
 * \updates       2024-04-17
 * \version       $Revision$
 * \license       GNU GPL v2 or above
 *
 *  This file defines functions for C++11 (or greater) code.  Near duplicates
 *  of a few functions from the cfg66 library, defined here to keep xpc66 and
 *  cfg66 independent.  These items were extracted from Seq66's C++
 *  basic_macros module.
 */

#include "cpp_types.hpp"                /* string, vector, msglevel         */

/*
 *  Do not document a namespace; it breaks Doxygen.
 */

namespace xpc
{

/*
 *  Free functions in the xpd namespace.
 */

extern bool warn_message
(
    const std::string & msg,
    const std::string & data = ""
);
extern bool error_message
(
    const std::string & msg,
    const std::string & data = ""
);
extern void file_message (const std::string & tag, const std::string & path);
extern bool file_error (const std::string & tag, const std::string & filename);
extern std::string get_full_path (const std::string & path);
extern bool set_current_directory (const std::string & path);
extern std::string normalize_path
(
    const std::string & path,
    bool to_unix = true,
    bool terminate = false
);
extern std::string current_date_time ();
extern std::wstring widen_string (const std::string & source);

}               // namespace xpc

#endif          // XPC66_XPC_UTILFUNCTIONS_HPP

/*
 * utilfuncttions.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

