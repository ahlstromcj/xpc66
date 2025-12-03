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
 * \file          fifo.cpp
 *
 *  This module defines our own fifo that support objects.
 *
 * \library       xpc66 application
 * \author        Chris Ahlstrom
 * \date          2025-12-02
 * \updates       2025-12-03
 * \license       GNU GPLv2 or above
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU Library General Public License version 2 as
 *   published by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *   for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  This implementation:
 *
 *      -   Encodes whole objects, not characters.
 *      -   Provides insertion to the back of the container and direct
 *          access to the back.
 *      -   Provides access to the front of the container to get that
 *          object.
 *      -   Provides a front() function to inspect the object.  If worried
 *          about the usability of the result, then use the read() function
 *          and test the result for a value greater than 0.
 *
 *  Recommendations for TYPE:
 *
 *      -   Provide a boolean or integer that indicates the retrieved item
 *          is not usable. For example, a count() of -1 or a boolean called
 *          usable(). The fifo template does not enforce this.
 */

#include <string>                       /* std::string                      */

#include "cpp_types.hpp"                /* CSTR() inline functions          */
#include "xpc/fifo.hpp"                 /* xpc::fifo                        */

#if defined PLATFORM_DEBUG
#include <iostream>
#endif

namespace xpc
{

#if defined PLATFORM_DEBUG_TMI          // TO DO

class fifo_test
{

public:

    using cref = fifo<fifo_test>::const_reference;

private:

    int m_test_counter;
    std::string m_test_text;

public:

    fifo_test (int counter = (-1), const std::string & text = "");
    fifo_test (const fifo_test & rhs) = default;
    fifo_test & operator = (const fifo_test & rhs) = default;
    ~fifo_test () = default;

    void increment ()
    {
        ++m_test_counter;
    }

    int test_counter () const
    {
        return m_test_counter;
    }

    void set_test_text (const std::string & t)
    {
        m_test_text = t;
    }

    const std::string & test_text () const
    {
        return m_test_text;
    }

    std::string to_string () const;

};              // class fifo_test

static void
show_message (const std::string & msg)
{
    std::cout << msg << std::endl;
}

static void
show_error (const std::string & msg)
{
    std::cerr << msg << std::endl;
}

static bool
item_test (fifo_test::cref item, const std::string & tag, int counter)
{
    std::string values { item.to_string() };
    int c { item.test_counter() };
    bool result { c == counter };
    std::cout << "Item test " << tag << " " << values << std::endl;
    if (! result)
        std::cerr << "'" << tag << "' test failed" << std::endl;

    return result;
}

fifo_test::fifo_test (int counter, const std::string & text) :
    m_test_counter  (counter),
    m_test_text     (text)
{
    // No code
}

std::string
fifo_test::to_string () const
{
    std::string result { "counter " + std::to_string(test_counter()) + "; " };
    result += "text '" + test_text() + "'.";
    return result;
}

bool
run_fifo_test ()
{
    bool result { true };
    fifo_test rt_a(1, "rt_a");
    fifo_test rt_b(2, "rt_b");
    fifo_test rt_c(3, "rt_c");
    fifo_test rt_d(4, "rt_d");
    fifo_test rt_e(5, "rt_e");
    fifo_test rt_f(6, "rt_f");
    fifo_test rt_g(7, "rt_g");
    fifo_test rt_h(8, "rt_h");
    fifo_test rt_i(9, "rt_i");
    fifo_test rt_j(10, "rt_j");

    /*
     * Smoke test
     */

    fifo<fifo_test> rb(7);           /* should become 8 (power of 2) */
    std::size_t sz { fife.write(rt_a) };
    if (sz != 1)
    {
        show_error("fifo::write() failed");
        result = false;
    }
    else
    {
        fifo_test rt;
        sz = fife.read(rt);
        if (sz > 0)
        {
            show_error("fifo::read() failed");
            result = false;
        }
        else
        {
            std::string ss { rt.to_string() };
            std::cout << "Read test object '" << ss << "'" << std::endl;
            if (fife.count() > 0)
            {
                show_error("read() failed to pop the object");
                result = false;
            }
        }
    }

    /*
     * Full buffer test. Ultimately 10 items entered. Only 8 should remain.
     * Then we pop all items in the fifo and show them.
     * (Should compare counters at some point.)
     */

    if (result)
    {
        fife.clear();
        if (! fife.empty())
        {
            show_error("fifo not empty");
            result = false;
        }
    }

    if (result)
    {
        std::size_t space { fife.read_space() };
        if (space == 0)
        {
            fife.push(rt_a);
            fife.push(rt_b);
            fife.push(rt_c);
            fife.push(rt_d);
            fife.push(rt_e);
            fife.push(rt_f);
            fife.push(rt_g);
            fife.push(rt_h);
            space = fife.read_space();
            if (fife.count() != 8 || space != 8)
            {
                show_error("fifo count mismatch");
                result = false;
            }
            if (result)
            {
                space = fife.write_space();
                if (space > 0)
                {
                    show_error("write space > 0");
                    result = false;
                }
            }
        }
        else
        {
            show_error("empty read-space error");
            result = false;
        }
    }
    if (result)
    {
        /*
         * Here, rt_a and rt_b should be dropped.
         */

        fife.push(rt_i);
        fife.push(rt_j);

        std::size_t rspace { fife.read_space() };
        std::size_t wspace { fife.write_space() };
        if (fife.count() != 8 || rspace != 8 || wspace != 0)
        {
            show_error("objects not overwritten");
            result = false;
        }
        if (fife.dropped() != 2)
        {
            show_error("unexpected number of dropped items");
            result = false;
        }
        if (result)
        {
            int imax { fife.count() };            /* can't use count() in loop */
            for (int i = 0; i < imax; ++i)
            {
                fifo_test::cref item { fife.front() };
                std::string values { item.to_string() };
                printf("[%d] %s\n", i, CSTR(values));
                if (item.test_counter() != (i + 3))
                    result = false;

                fife.pop();
            }
            if (! result)
                show_message("Item-counter mismatch detected");

            if (fife.empty())
            {
                show_message("Should see rt_c through rt_j values");
            }
            else
            {
                show_error("ringbuffer still has items!");
                result = false;
            }
        }
    }

    /*
     *  Alternative push/pops with access via front(). Note that back()
     *  goes back one step from the tail in order to (hopefully) get a valid
     *  object.
     */

    if (result)
    {
        fife.clear();

        fifo_test::cref fitem { fife.front() };
        fifo_test::cref bitem { fife.back() };
        result = item_test(fitem, "front", (-1));   /* usability test   */
        result = item_test(bitem, "back", (-1));    /* usability test   */

        fife.push(rt_a);                            /* front (1)        */
        fife.push(rt_b);
        fife.push(rt_c);
        fife.push(rt_d);                            /* back (4)         */
        result = fife.count() == 4;
        if (result)
        {
            fifo_test::cref frontitem { fife.front() };
            result = item_test(frontitem, "[front]", 1);
            if (result)
            {
                fifo_test::cref backitem { fife.back() };
                result = item_test(backitem, "[back] ", 4);
            }
            if (result)
            {
                fife.pop();
            }
            else
                show_error("First front call failed");
        }
        else
            show_error("Bad fifo count");
    }

    /*
     * End of tests.
     */

    return result;
}

#endif

}           // namespace xpc

/*
 * fifo.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
