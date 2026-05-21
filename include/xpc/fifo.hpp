#if ! defined XPC66_XPC_FIFO_HPP
#define XPC66_XPC_FIFO_HPP

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
 * \file          fifo.hpp
 *
 *  This module defines our own FIFO queue that supports objects, and is
 *  thread safe (optionally).
 *
 * \library       xpc66 application
 * \author        Chris Ahlstrom
 * \date          2025-12-02
 * \updates       2025-12-03
 * \license       GNU GPLv2 or above
 *
 *  One could use std::queue by itself, but we want to avoid undefined
 *  behavior and exception.
 *
 *  The object of the template should provide a default constructor that
 *  indicates no object could be retrieved. Thus, this template is more
 *  appropriate for user-controlled objects.
 */

#include <cstddef>
#include <sys/types.h>
#include <queue>

#include "lib_build_macros.h"           /* PLATFORM_DEBUG macro, etc.       */
#include "xpc/automutex.hpp"            /* xpc::automutex and xpc::recmutex */

namespace xpc
{

/**
 *  Provides a FIFO queue with some safety features.
 *  Eventually we will provide thread safety.
 */

template <typename TYPE>
class fifo
{

public:

    using value_type = TYPE;
    using reference = TYPE &;
    using const_reference = const TYPE &;
    using size_type = std::size_t;
    using container = std::queue<value_type>;

private:

    /**
     *  The container for all push/popped items in the queue.
     */

    container m_queue { };

    /**
     *  Constant container size.
     */

    size_type m_queue_size { 32 };

    /**
     *  The locking mutex.  This object is passed to an automutex object that
     *  lends exception-safety to the mutex locking.
     */

    mutable xpc::recmutex m_mutex { };

    /**
     *  Indicates whether the size of the queue is limited or not,
     *  If the size parameter is 0, there is no limit to the size
     *  of the queue.
     */

    bool m_infinite { true };

    /**
     *  Is memory locked or the queue thread-safe? NOT YET SUPPORTED.
     */

    bool m_locked { false };

    /**
     *  Number of items not pushed in run due to overflow, if applicable.
     */

    int m_dropped { 0 };

public:

    fifo (size_type sz = 0);
    fifo (const fifo &) = delete;
    fifo (fifo &&) = delete;
    fifo & operator = (const fifo &) = delete;
    fifo & operator = (fifo &&) = delete;
    ~fifo () = default;

    bool mlock ();

    /**
     *  Reset the read and write pointers to zero. This is not thread safe.
     *  Neither is the clear() function.
     */

    void clear ()
    {
        m_dropped = 0;
        initialize();
    }

    int buffer_size () const
    {
        return int(m_queue_size);
    }

    /**
     *  The number of active entries in the queue. Are these next few
     *  functions worth locking?
     */

    int count () const
    {
        return int(m_queue.size());
    }

    bool empty () const
    {
        return count() == 0;
    }

    int dropped () const
    {
        return m_dropped;
    }

    bool push (const value_type & value);
    value_type pop ();

#if defined USE_FRONT_BACK_REFERENCE_RETURN

    /*
     *  Returns reference to the first element in the queue. This element will
     *  be the first element to be removed on a call to pop().  The return
     *  value might not be valid, either a default-constructed object or an
     *  old and "overwritten" value. An alternative is to call the read()
     *  function and check the return value.
     *
     *  Note that there's no real way to lock the front()/back() functions
     *  safely for the caller.
     *
     *  We might want to return an object instead of a reference.
     */

    reference front ()
    {
        static value_type s_dummy;
        return count() > 0 ? m_queue.front() : s_dummy ;
    }

    const_reference front () const
    {
        static value_type s_dummy;
        return count() > 0 ? m_queue.front() : s_dummy ;
    }

    /**
     *  Currently there's no way to be sure that the back item is a
     *  valid item, except by checking the number of items in the queue.
     */

    reference back ()
    {
        static value_type s_dummy;
        return count() > 0 ? m_queue.back() : s_dummy ;
    }

    const_reference back () const
    {
        static value_type s_dummy;
        return count() > 0 ? m_queue.back() : s_dummy ;
    }

#else

    value_type front () const
    {
        xpc::automutex locker(m_mutex);
        static value_type s_dummy;
        return count() > 0 ? m_queue.front() : s_dummy ;
    }

    value_type back () const
    {
        xpc::automutex locker(m_mutex);
        static value_type s_dummy;
        return count() > 0 ? m_queue.back() : s_dummy ;
    }

#endif      // defined USE_FRONT_BACK_REFERENCE_RETURN

private:    // helper functions

    void initialize ();

};          // class fifo<TYPE>

/**
 *  Create a new queue to hold at least `sz' elements (TYPE) of data.
 *
 *  Note that the members are initialize "in-class".
 *
 * \param sz
 *      The size limit of the queue. If 0, then the queue can expand
 *      indefinitely, but the initial size is set to a default value
 *      of 32.
 */

template<typename TYPE>
fifo<TYPE>::fifo (size_type sz)
{
    if (sz > 0)
    {
        m_queue_size = sz;             /* otherwise the queue size is 32   */
        m_infinite = false;             /* otherwise the growth is endless  */
    }
    initialize();
}

/**
 *  The std::queue container does not have a clear() function.
 *  There are three ways to empty it:
 *
 *      -   While the queue is not empty, pop() in a loop.
 *      -   Swap it with an empty queue.
 *      -   Assign an empty queue to it.
 *
 *  The first option is slow, but straightforward.
 *
 *  The std::queue container does not have a reserve() function.
 *
 *      m_queue.reserve(m_queue_size);
 */

template<typename TYPE>
void
fifo<TYPE>::initialize ()
{
    while (! m_queue.empty())
        m_queue.pop();

    m_dropped = 0;
}

/**
 *  Lock the data block of `rb' using the system call 'mlock'.
 *  Not nearly ready for prime time!
 */

template<typename TYPE>
bool
fifo<TYPE>::mlock ()
{
#if defined XPC66_USE_MEMORY_LOCK
    if (::mlock(m_queue, m_queue_size) != 0)
        return false;

    m_locked = true;
    return true;
#else
    return false;
#endif
}

/**
 *  Puts an item on the back of the queue, for later retrieval via pop().
 */

template<typename TYPE>
bool
fifo<TYPE>::push (const value_type & item)
{
    xpc::automutex locker(m_mutex);
    if (m_infinite || m_queue.size() < m_queue_size)
    {
        m_queue.push(item);
        return true;
    }
    else                                    /* accept item and drop front() */
    {
        ++m_dropped;                        /* for future use in expansion  */
        return false;
    }
}

/**
 *  Pops an item from the front of the queue. If the queue is empty,
 *  then a default item is popped. If this worries you, then call
 *  count() first.
 */

template<typename TYPE>
typename fifo<TYPE>::value_type
fifo<TYPE>::pop ()
{
    xpc::automutex locker(m_mutex);
    static value_type s_dummy;
    if (m_queue.size() > 0)
    {
        value_type result { front() };
        m_queue.pop();
        return result;
    }
    else
        return s_dummy;
}

/*
 *  Free function for testing the fifo.
 */

#if defined PLATFORM_DEBUG

extern bool run_fifo_test ();

#endif

}           // namespace xpc

#endif      // XPC66_XPC_FIFO_HPP

/*
 * fifo.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

