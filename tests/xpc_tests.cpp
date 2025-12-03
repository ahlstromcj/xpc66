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
 * \updates       2025-11-25
 * \license       See above.
 *
 *  To do: add a help-line for each option.
 */

#include <cstdlib>                      /* EXIT_SUCCESS, EXIT_FAILURE       */
#include <iostream>                     /* std::cout, std::cerr             */
#include <string>                       /* std::string                      */

#include "xpc66.hpp"                    /* xpc66_version() function         */
#include "xpc/kbhit.hpp"                /* xpc::kbhit(), getch(), etc.      */
#include "xpc/ring_buffer.hpp"          /* xpc::run_ring_test()             */
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

bool
test_kbhit ()
{
    /*
     * Hmmmm, only an Enter works. Why not other characters?
     * Oh, we need to use the extended kbhit(), kbhit_ex() that
     * temporarily puts the terminal in raw mode.
     */

    printf("Waiting for a keystroke...\n");
    for (;;)
    {
        printf(".");
        (void) ::fflush(stdout);
        if (xpc::kbhit_ex())
        {
            printf("\n");
            break;
        }
        xpc::millisleep(500);
    }
    printf("Thank you!\n");
    return true;
}

bool
test_getch ()
{
    std::cout << "Press any key again..." << std::endl;
    xpc::clear_kb_ex();

    int c { xpc::getch() };
    char character { char(c) };
    std::cout << "'" << character << "' pressed" << std::endl;
    return true;
}

bool
test_kbget ()
{
    std::cout << "Press any key yet again, then <Enter>..." << std::endl;
    xpc::clear_kb_ex();

    char c { xpc::kbget() };
    std::cout << "'" << c << "' entered" << std::endl;
    return true;
}

/*
 * Test of shellexecute functions. It assumes the test is being run
 * from the project top-level directory.
 *
 * Note: On one laptop, this appears:
 *
 *  [25687:25706:1104/072528.126313:
 *      ERROR:google_apis/gcm/engine/registration_request.cc:291]
 *  Registration response error message: DEPRECATED_ENDPOINT
 *
 * One workaround is to suppress this error (and others) by tweaking
 * the chrome options tag like this.
 *
 *      chrome_options = Options()
 *      chrome_options.add_argument("--log-level=3")
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
        success = test_getch();

    if (success)
        success = test_kbget();

    if (success)
        success = xpc::run_ring_test();

    if (success)
        success = test_shell_execution();   /* this test should come last   */

    return success ? EXIT_SUCCESS : EXIT_FAILURE ;
}

/*
 * xpc_tests.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
