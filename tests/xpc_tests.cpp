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
 * \file          xpc_tests.cpp
 *
 *      A test-file for the rudimentary CLI parser class and C API.
 *
 * \library       xpc66
 * \author        Chris Ahlstrom
 * \date          2022-07-03
 * \updates       2025-11-01
 * \license       See above.
 *
 *  To do: add a help-line for each option.
 */

#include <cstdlib>                      /* EXIT_SUCCESS, EXIT_FAILURE       */
#include <iostream>                     /* std::cout, std::cerr             */
#include <string>                       /* std::string                      */

#include "xpc66.hpp"                    /* xpc66_version() function         */
#include "xpc/kbhit.hpp"                /* xpc::kbhit(), getch(), etc.      */
#include "xpc/shellexecute.hpp"         /* xpc::open_pdf() for linkage test */
#include "xpc/timing.hpp"               /* xpc66::microsleep(), etc.        */

namespace   // anonymous
{

/*
 * Explanation text.
 */

static const std::string help_intro
{
    "This test program illustrates and tests the C++ interface of the os-\n"
    "independence of the xpc66 library.  Options are as follows:\n\n"
};

/*
 * Test of shellexecute functions. It assumes the test is being run
 * from the project top-level directory.
 */

bool
test_shell_execution ()
{
    static std::string s_doc = "./doc/xpc66-library-guide.pdf";
    bool result = xpc::open_pdf(s_doc);
    if (! result)
    {
        std::cerr << "Failed to open '" << s_doc << "'" << std::endl;
    }
    return result;
}

bool
test_kbhit ()
{
    /*
     * Hmmmm, only an Enter works. Why not other characters?
     */

    printf("Waiting for an <Enter> keystroke...\n");
    for (;;)
    {
        printf(".");
        (void) ::fflush(stdout);
        if (xpc::kbhit())
        {
            printf("\n");
            break;
        }
        xpc::millisleep(500);
    }
    printf("Thank you!\n");
    return true;
}

}   // namespace anonymous

/*
 * main() routine
 */

int
main (int /*argc*/, char * /*argv*/ [])
{
    std::cout << "Test of " << xpc66_version() << ":" << std::endl;

    bool success { test_kbhit() };
    if (success)
        success = test_shell_execution();   /* this test should come last   */

    return success ? EXIT_SUCCESS : EXIT_FAILURE ;
}

/*
 * xpc_tests.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
