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
 * \updates       2022-11-16
 * \license       See above.
 *
 *  To do: add a help-line for each option.
 */

#include <string>
#include <cstdlib>                   /* EXIT_SUCCESS, EXIT_FAILURE       */

/*
 * Explanation text.
 */

static const std::string help_intro
{
    "This test program illustrates and tests the C++ interface of the os-\n"
    "independence of the xpc66 library.  Options are as follows:\n\n"
};

/*
 * main() routine
 */

int
main (int /*argc*/, char * /*argv*/ [])
{
    int rcode = EXIT_FAILURE;
#if 0
    xpc::cliparser clip(test_options);
    bool success = clip.parse(argc, argv);
    if (success)
    {
        rcode = EXIT_SUCCESS;
        if (clip.help_request())
        {
            std::cout
                << help_intro
                << clip.help_text()
                ;
        }
        else if (clip.version_request())
        {
            printf("Version 0.0.0\n");  /* TODO! */
        }
    }
    if (success)
        std::cout << "cliparser C++ test succeeded" << std::endl;
    else
        std::cout << "cliparser C++ test failed" << std::endl;

#endif
    return rcode;
}

/*
 * xpc_tests.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=c
 */

