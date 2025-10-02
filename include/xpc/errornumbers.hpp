#if ! defined XPC66_XPC_ERRORNUMBERS_HPP
#define XPC66_XPC_ERRORNUMBERS_HPP

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
 * \file          errornumbers.hpp
 *
 *    This module provides functions for translating error numbers
 *    for trouble-shooting and clarity.
 *
 * \library       xpc66
 * \author        Chris Ahlstrom and other authors; see documentation
 * \date          2025-10-02
 * \updates       2025-10-02
 * \version       $Revision$
 * \license       GNU GPL v2 or above
 *
 */

#include "cpp_types.hpp"                /* string, vector, msglevel         */

namespace xpc
{

/*
 *  Free functions in the xpc namespace.
 */

extern const std::string & errno_name (int errnum);
extern int error_number (const std::string & errname);

}               // namespace xpc

#endif          // XPC66_XPC_ERRORNUMBERS_HPP

/*
 * errornumbers.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

