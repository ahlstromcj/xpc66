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
 * \file          automutex.cpp
 *
 *  This module declares/defines an automatic (exception-safe) mutex.
 *
 * \library       xpc66
 * \author        Chris Ahlstrom
 * \date          2015-07-24
 * \updates       2024-04-17
 * \license       GNU GPLv2 or above
 *
 *  Seq66 needs a recursive mutex and a condition-variable for sequencer
 *  operations.  This module defines the recursive mutex needed.
 */

#include <cstdio>                       /* std::printf()                    */

#include "xpc/automutex.hpp"            /* xpc::automutex                   */

/*
 *  Do not document a namespace; it breaks Doxygen.
 */

namespace xpc
{

/*
 * Currently, the header defines all the code needed for this class.
 */

#if defined PLATFORM_DEBUG

/**
 *  The safety mutex, passed to the automutex constructor.
 */

recmutex s_common_mutex;

/*
 *  This code merely tests linkage. It can also be used to check for leakage
 *  in valgrind.We can later allocate threads to test the locking.
 */

bool
thread_1_locking ()
{
    automutex m{s_common_mutex};
    std::printf("Thread 1 in\n");
    return true;
}

bool
thread_2_locking ()
{
    automutex m{s_common_mutex};
    std::printf("Thread 2 in\n");
    return true;
}

#endif          // PLATFORM_DEBUG

}               // namespace xpc

/*
 * automutex.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

