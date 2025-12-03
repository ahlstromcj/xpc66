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

#undef  XPC66_USE_MEMORY_LOCK           /* TODO: needs a lot of work !      */

#if defined XPC66_USE_MEMORY_LOCK
#include <sys/mman.h>
#endif

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

    container m_buffer { };

    /**
     *  Constant container size.
     */

    size_type m_buffer_size { 32 };

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
    ~fifo ();

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
        return int(m_buffer_size);
    }

    /**
     *  The number of active entries in the queue.
     */

    int count () const
    {
        return int(m_buffer.size());
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

    /*
     * Returns reference to the first element in the queue. This element will
     * be the first element to be removed on a call to pop().  The return
     * value might not be valid, either a default-constructed object or an
     * old and "overwritten" value. An alternative is to call the read()
     * function and check the return value.
     */

    reference front ()
    {
        return m_buffer.front();
    }

    const_reference front () const
    {
        return m_buffer.front();
    }

    /**
     *  Currently there's no way to be sure that the back item is a
     *  valid item, except by checking the number of items in the queue.
     */

    reference back ()
    {
        return m_buffer.back();
    }

    const_reference back () const
    {
        return m_buffer.back();
    }

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
        m_buffer_size = sz;             /* otherwise the queue size is 32   */
        m_infinite = false;             /* otherwise the growth is endless  */
    }
    initialize();
}

/**
 *  Free all data associated with the ringbuffer `m_rb'.
 */

template<typename TYPE>
fifo<TYPE>::~fifo ()
{
#if defined XPC66_USE_MEMORY_LOCK
    if (m_locked)
        ::munlock(m_buffer, m_buffer_size);
#endif
}

template<typename TYPE>
void
fifo<TYPE>::initialize ()
{
    TYPE empty_value;
    m_buffer.clear();
    m_buffer.reserve(m_buffer_size);
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
    if (::mlock(m_buffer, m_buffer_size) != 0)
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
    if (m_infinite || m_buffer.size() < m_buffer_size)
    {
        m_buffer.push_back(item);
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
    static value_type s_dummy;
    if (m_buffer.size() > 0)
    {
        value_type result { front() };
        m_buffer.pop_front();
        return result;
    }
    else
        return s_dummy;
}

/*
 *  Free functions (for testing the fifo).
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

