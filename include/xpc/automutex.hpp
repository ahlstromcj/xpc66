#if ! defined XPC66_XPC_AUTOMUTEX_HPP
#define XPC66_XPC_AUTOMUTEX_HPP

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
 * \file          automutex.hpp
 *
 *  This module declares/defines the base class for mutexes.
 *
 * \library       xpc66
 * \author        Chris Ahlstrom
 * \date          2015-07-24
 * \updates       2024-04-15
 * \license       GNU GPLv2 or above
 *
 *  This module defines the following classes:
 *
 *      -   xpc::mutex.  An alias for std::recursive_mutex.
 *      -   xpc::automutex. A way to lock a function exception-safely and
 *          easily.
 */

#include "xpc/recmutex.hpp"             /* xpc::recmutex wrapper class      */

/*
 *  Do not document a namespace; it breaks Doxygen.
 */

namespace xpc
{

/**
 *  Provides a mutex that locks automatically when created, and unlocks
 *  when destroyed.  This has a couple of benefits.  First, it is threadsafe
 *  in the face of exception handling.  Secondly, it can be done with just one
 *  line of code.
 *
 *  It could potentially be replaced by std::lock_guard<std::recursive_mutex>
 *  However, it provides lock() and unlock() functions for extra flexibility
 *  and danger.  :-)
 */

class automutex
{

private:

    /**
     *  Provides the mutex reference to be used for locking.
     */

    recmutex & m_safety_mutex;

private:                        /* do not allow these functions to be used  */

    automutex () = delete;
    automutex (const automutex &) = delete;
    automutex & operator = (const automutex &) = delete;

public:

    /**
     *  Principal constructor gets a reference to a mutex parameter, and
     *  then locks the mutex.
     *
     * \param my_mutex
     *      The caller's mutex to be used for locking.
     */

    automutex (recmutex & my_mutex) : m_safety_mutex (my_mutex)
    {
        lock();
    }

    /**
     *  The destructor unlocks the mutex.
     */

    ~automutex ()
    {
        unlock();
    }

    /**
     *  The lock() and unlock() functions are provided for additional
     *  flexibility in usage.
     */

    void lock ()
    {
        m_safety_mutex.lock();
    }

    void unlock ()
    {
        m_safety_mutex.unlock();
    }

};          // class automutex

#if defined PLATFORM_DEBUG
extern bool thread_1_locking ();
extern bool thread_2_locking ();
#endif

}           // namespace xpc

#endif      // XPC66_XPC_AUTOMUTEX_HPP

/*
 * automutex.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

