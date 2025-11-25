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
 * \file          kbhit.cpp
 * \library       xpc66
 * \author        Chris Ahlstrom
 * \date          2025-11-01
 * \updates       2025-11-04
 * \license       GNU GPLv2 or above
 *
 *  Kbhit/getch module of the POSIX C Wrapper (PSXC) library
 *  Copyright (C) 2005-2025 by Chris Ahlstrom
 *
 *  The xpc::kbhit() and xpc::getch() functions are provided, as well
 *  as a couple of convenience functions. Our kbhit() returns a boolean.
 *
 *  Parts of this code are basically copped from the project at
 *
 *      https://github.com/thradams/conio
 */

#include <iostream>                     /* std::cout, std::cin, getchar/EOF */

#include "platform_macros.h"            /* detects the build platform       */
#include "kbhit.hpp"                    /* xpc::kbhit(), getch(), etc.      */

#if defined PLATFORM_WINDOWS

#include <conio.h>                      /* a Microsoft Windows header       */

namespace xpc
{

bool
kbhit ()
{
    return ::_kbhit();
}

int
getch ()
{
    return ::_getch();
}

bool
kbhit_ex ()
{
    return xpc::kbhit();

}           // namespace xpc

#else       // if LINUX

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>

namespace xpc
{

bool
kbhit ()
{
    struct timeval tv { 0, 0 };             /* tv.tv_sec = tv.tv_usec = 0;  */
    fd_set read_fd;
    FD_ZERO(&read_fd);
    FD_SET(0,&read_fd);
    if (::select(1, &read_fd, NULL, NULL, &tv) == -1)
        return false;

    if (FD_ISSET(0, &read_fd))
        return true;

    return false;
}

/**
 *  An alternative implementation the changes the terminal attributes
 *  to exit the canonical/cooked mode and enter the raw mode, get
 *  the character, and restore the previous terminal mode.
 */

bool
kbhit_ex ()
{
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);

    struct termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return true;
    }
    return false;
}

/* Read 1 character without echo */

int
getch ()
{
    struct termios old;
    tcgetattr(0, &old);

    struct termios znew;
    znew = old;
    znew.c_lflag &= ~ICANON;
    znew.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &znew);

    int ch = getchar();
    tcsetattr(0, TCSANOW, &old);
    return ch;
}

#endif

void
clear_kb ()
{
    while (kbhit())
        (void) getch();
}

void
clear_kb_ex ()
{
    while (kbhit_ex())
        (void) getch();
}

bool
kbcheck ()
{
    bool result = kbhit();
    if (result)
        (void) getch();

    return result;
}

bool
kbcheck_ex ()
{
    bool result = kbhit_ex();
    if (result)
        (void) getch();

    return result;
}

/**
 *  This function waits for input of <Enter> or <Ctrl-D>. That is all.
 *  The character is returned.
 */

char
kbget ()
{
    return std::cin.get();
}

}           // namespace xpc

/*
 * kbhit.cpp
 *
 * vim: ts=4 sw=4 et ft=cpp
 */

