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
 * \file          errnames.cpp
 *
 *      A small application to show errno values, errno names, and
 *      the perror() string.
 *
 * \library       xpc66
 * \author        Chris Ahlstrom
 * \date          2025-10-05
 * \updates       2025-10-05
 * \license       See above.
 *
 */

#include <cctype>                   /* std::is_digit()                  */
#include <cstdlib>                  /* EXIT_SUCCESS, EXIT_FAILURE       */
#include <iostream>                 /* std::cout, std::cerr             */
#include <string>                   /* std::string                      */

#include "xpc/errornumbers.hpp"     /* xpc::open_pdf() for linkage test */

namespace
{

/*
 * Explanation text.
 */

const std::string s_help_intro
{
    "This program shows errno values and names. The usage is simple.\n\n"
    "   $ errnames N\n\n"
    "   Given an number from 0 to 134, show the 'E' name of the number,\n"
    "   such as 'EAGAIN'.\n\n"
    "   $ errnames Exxxx\n\n"
    "   Given an errno name starting with 'E', show the number.\n\n"
    "   $ errnames all\n\n"
    "   Show all of the error names and numbers.\n\n"
};


}           // namespace anonymous

/*
 * main() routine
 */

int
main (int argc, char * argv [])
{
    int rcode { EXIT_FAILURE };
    if (argc > 1)
    {
        std::string arg { argv[1] };
        int count { 0 };
        int digitcount { 0 };
        for (auto c : arg )
        {
            if (std::isdigit(c))
                ++digitcount;
            else
                arg[count] = std::toupper(arg[count]);

            ++count;
        }
        if (digitcount > 0)
        {
            int e { std::stoi(arg) };
            std::string errline { xpc::errno_line(e) };
            std::cout << errline << std::endl;
            rcode = EXIT_SUCCESS;
        }
        else if (arg == "ALL")
        {
            std::string chart { xpc::errno_chart() };
            std::cout
                << "Chart of Errno Numbers" << std::endl
                << chart << std::endl
                ;
            rcode = EXIT_SUCCESS;
        }
        else if (arg[0] == 'E')
        {
            int e { xpc::errno_number(arg) };
            std::string errline = xpc::errno_line(e);
            std::cout << errline << std::endl;
            rcode = EXIT_SUCCESS;
        }
        else
            std::cout << s_help_intro;

    }
    else
        std::cout << s_help_intro;

    return rcode;
}

/*
 * errnames.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=c
 */

