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
 * \file          xpc66.cpp
 *
 *  This module defines an informational function.
 *
 * \library       xpc66
 * \author        Chris Ahlstrom
 * \date          2024-04-15
 * \updates       2024-04-15
 * \license       GNU GPLv2 or above
 *
 */

#include "xpc66.hpp"                    /* no-namespace function library    */

std::string
xpc66_version ()
{
    static std::string s_info = "xpc66-0.1 library 2024-04-15";
    return s_info;
}

/*
 * xpc66.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

