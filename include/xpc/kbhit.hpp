#if ! defined XPC66_XPC_KBHIT_HPP
#define XPC66_XPC_KBHIT_HPP

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
 * \file          kbhit.hpp
 * \author        Chris Ahlstrom
 * \date          2025-11-01
 * \updates       2025-11-01
 * \license       GNU GPLv2 or above
 *
 *    Kbhit of POSIX C Wrapper (PSXC) library
 *    Copyright (C) 2005-2025 by Chris Ahlstrom
 *
 *    This module provides functions to supply the oft-needed kbhit() and
 *    getch() function for C++, in the xpc namespace.
 */

namespace xpc
{

extern int getch ();
extern bool kbhit ();
extern bool kbhit_ex ();
extern void clear_kb ();
extern void clear_kb_ex ();
extern bool kbcheck ();
extern bool kbcheck_ex ();

}        // namespace xpc

#endif   // XPC66_XPC_KBHIT_HPP

/*
 * kbhit.hpp
 *
 * vim: ts=4 sw=4 et ft=cpp
 */

