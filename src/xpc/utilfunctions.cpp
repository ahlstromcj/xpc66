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
 * \file          utilfunctions.cpp
 *
 *  This module defines some informative functions that are actually
 *  better off as functions.
 *
 * \library       xpc66
 * \author        Chris Ahlstrom
 * \date          2024-04-15
 * \updates       2024-04-17
 * \license       GNU GPLv2 or above
 *
 *  Near duplicates of a few functions from the cfg66 library, defined here
 *  to keep xpc66 and cfg66 independent of each other.
 *
 *  An application should consider linking to the cfg66 library and use
 *  its version of these functions (in the util namespace).
 */

#include <algorithm>                    /* std::replace() function          */
#include <cstring>                      /* std::strlen(3)                   */
#include <cstdarg>                      /* see "man stdarg(3)"              */
#include <ctime>                        /* std::strftime()                  */
#include <iostream>

#include "c_macros.h"                   /* not_nullptr() macro              */
#include "platform_macros.h"            /* detects the build platform       */
#include "xpc/utilfunctions.hpp"        /* basic message functions          */

#if defined PLATFORM_UNIX

#include <unistd.h>                     /* C::write(2), STDOUT_FILENO, etc  */
#define S_WRITE         write           /* POSIX write(2)                   */
#define S_CHDIR         chdir           /* ISO/POSIX/BSD chdir()            */
#define ENV_HOME        "HOME"

#else

#if defined PLATFORM_WINDOWS            /* Microsoft platform               */

#include <io.h>                         /* C::_write()                      */
#include <windows.h>                    /* ::MultiByteToWideChar()          */

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2
#define ENV_HOMEDRIVE   "HOMEDRIVE"
#define ENV_HOMEPATH    "HOMEPATH"

#if defined PLATFORM_MSVC               /* Microsoft compiler vs MingW      */
#define S_WRITE     _write              /* Microsoft's write()              */
#define S_CHDIR     _chdir              /* Microsoft's chdir()              */
#else
#define S_CHDIR     chdir
#endif
#endif
#endif

/*
 *  Do not document a namespace; it breaks Doxygen.
 */

namespace xpc
{

static bool
file_name_good (const std:: string & path)
{
    return ! path.empty();
}

/**
 *  Gets the user's $HOME (Linux) or $LOCALAPPDAT (Windows) directory from the
 *  current environment.
 *
 * getenv("HOME"):
 *
 *      -   Linux returns "/home/ahlstrom".  Append "/.config/seq66".
 *      -   Windows returns "\Users\ahlstrom".  A better value than HOMEPATH
 *          is LOCALAPPDATA, which gives us most of what we want:
 *          "C:\Users\ahlstrom\AppData\Local", and then we append simply
 *          "seq66".  However, this inconsistency is annoying. So now
 *          we provide separate functions for home versus the standard
 *          configuration directory for a Windows or Linux user.
 *
 * \param appfolder
 *      If not empty (the default) then the parameter is appended to the
 *      path that is returned.
 *
 * \return
 *      Returns the value of $HOME, such as "/home/user" or "C:\Users\user".
 *      Notice the lack of a terminating path-slash.  If std::getenv() fails,
 *      an empty string is returned. The value is normalized to use the
 *      UNIX path separator.
 */

static std::string
user_home (const std::string & appfolder)
{
    std::string result;
#if defined PLATFORM_WINDOWS
    char * env = std::getenv(ENV_HOMEDRIVE);
    if (not_nullptr(env))
    {
        char * env2 = std::getenv(ENV_HOMEPATH);
        if (not_nullptr(env2))
        {
            result += env;              /* "C:"                             */
            result += env2;             /* "\Users\username"                */
        }
    }
#else
    char * env = std::getenv(ENV_HOME);
    if (not_nullptr(env))
        result = std::string(env);      /* "/home/username"                 */
#endif
    if (result.empty())
    {
        file_error("std::getenv() failed", "HOME");
    }
    else
    {
        result = normalize_path(result);
        if (! appfolder.empty())
            result = result + "/" + appfolder;
    }
    return result;
}

static bool
is_a_tty (int fd)
{
#if defined PLATFORM_WINDOWS
    int fileno;
    switch (fd)
    {
        case STDIN_FILENO:  fileno = _fileno(stdin);    break;
        case STDOUT_FILENO: fileno = _fileno(stdout);   break;
        case STDERR_FILENO: fileno = _fileno(stderr);   break;
        default:            fileno = (-1);              break;
    }
    int rc = (fileno >= 0) ? _isatty(fileno) : 90 ;
    return rc == 1;                             /* fd refers to a terminal  */
#else
    int rc = isatty(fd);
    return rc == 1;                             /* fd refers to a terminal  */
#endif
}

/**
 *  Distilled from cfg66/src/cfg/appinfo.cpp to indicate xpc66 messages.
 *  No checking the "message level", all messages are colored the same.
 */

static std::string
get_client_tag ()
{
    static const char * s_color = "\033[1;30m";             /* black    */
    static const char * s_normal_color = "\033[0m";
    std::string result = "[";
    bool showcolor = is_a_tty(STDERR_FILENO);
    if (showcolor)
        result += s_color;

    result += "xpc66";
    if (showcolor)
        result += s_normal_color;

    result += "]";
    return result;
}

/**
 *  Common-code for console warning messages.  Adds markers and a newline.
 *
 * \param msg
 *      The message to print, sans the newline.
 *
 * \param data
 *      Additional information about the message. Optional.
 *
 * \return
 *      Returns true, so that the caller can show the message and return the
 *      status at the same time.
 */

bool
warn_message (const std::string & msg, const std::string & data)
{
    std::cerr << get_client_tag() << " " << msg;
    if (! data.empty())
        std::cerr << ": " << data;

    if (! msg.empty())
        std::cerr << std::endl;

    return true;
}

/**
 *  Common-code for error messages.  Adds markers, and returns false.
 *
 * \param msg
 *      The message to print, sans the newline.
 *
 * \param data
 *      Additional information about the message. Optional.
 *
 * \return
 *      Returns false for convenience/brevity in setting function return
 *      values.
 */

bool
error_message (const std::string & msg, const std::string & data)
{
    std::cerr << get_client_tag() << " " << msg;
    if (! data.empty())
        std::cerr << ": " << data;

    if (! msg.empty())
        std::cerr << std::endl;

    return false;
}

/**
 *  Common-code for error messages involving file issues, a very common use
 *  case in error-reporting.  Adds markers, and returns false.
 *
 * \param tag
 *      The message to print, sans the newline.
 *
 * \param path
 *      The name of the file to be shown.
 *
 * \return
 *      Returns false for convenience/brevity in setting function return
 *      values.
 */

bool
file_error (const std::string & tag, const std::string & path)
{
    std::cerr << get_client_tag() << " "
        << tag << ": '" << path << "'" << std::endl;

    return false;
}

/**
 *  Shows a path-name (or other C++ string) as an info message.  This output
 *  is not contingent upon debugging or verbosity.
 *
 * \param tag
 *      Provides the text to precede the name of the path.
 *
 * \param path
 *      Provides the path-name to print.  This message can be something other
 *      than a path-name, by the way.
 */

void
file_message (const std::string & tag, const std::string & path)
{
    std::cout << get_client_tag() << " "
        << tag << ": '" << path << "'" << std::endl;
}

/**
 *  Given a path, relative or not, this function returns the full path.
 *  It uses the Linux function realpath(3), which returns the canonicalized
 *  absolute path-name.  For Windows, the function _fullpath() is used.
 *
 * \param path
 *      Provides the path, which may be relative.
 *
 * \return
 *      Returns the full path.  If a problem occurs, the result is empty.
 */

std::string
get_full_path (const std::string & path)
{
    std::string result;                         /* default empty result     */
    if (file_name_good(path))
    {
#if defined PLATFORM_WINDOWS              /* _MSVC not defined in Qt  */
        char * resolved_path = NULL;            /* what a relic!            */
        char temp[256];
        resolved_path = _fullpath(temp, path.c_str(), 256);
        if (not_NULL(resolved_path))
            result = resolved_path;
#else
        char * resolved_path = NULL;            /* what a relic!            */
        resolved_path = realpath(path.c_str(), NULL);
        if (not_NULL(resolved_path))
        {
            result = resolved_path;
            free(resolved_path);
        }
#endif
    }
    return result;
}

/**
 *  Sets the current directory for the application.  A wrapper replacement for
 *  chdir() or _chdir().  It sets the current working directory in the
 *  application.  This function is necessary in order to make sure the current
 *  working directory is a safe place to work.
 *
 * \return
 *      Returns true if the path name is good, and the chdir() call succeeded.
 *      Otherwise, false is returned.
 *
 * \param path
 *      The full or relative name of the directory.
 */

bool
set_current_directory (const std::string & path)
{
    bool result = false;
    if (! path.empty())
    {
        int rcode = S_CHDIR(path.c_str());
        result = is_posix_success(rcode);
        if (! result)
            file_error("chdir() failed", path);
    }
    return result;
}

/**
 *  Makes sure that the path-name is a UNIX path, separated by forward slashes
 *  (the solidus), or a Windows path, separated by back slashes.  It also
 *  converts "~" to the user's HOME or LOCALAPPDATA directory.
 *
 * \param path
 *      Provides the path, which should be a full path-name.
 *
 * \param to_unix
 *      Defaults to true, which converts "\" to "/".  False converts in the
 *      opposite direction.
 *
 * \param terminate
 *      If true, tack on a final separator, if necessary. Defaults to false.
 *      Do not set to true if you know the path ends with a file-name.
 *
 * \return
 *      The possibly modified path is returned.  If path is not valid (e.g.
 *      the name of a console file handle), then an empty string is returned.
 */

std::string
normalize_path (const std::string & path, bool to_unix, bool terminate)
{
    std::string result;
    if (file_name_good(path))
    {
        result = path;

        auto circumpos = result.find_first_of("~");
        if (circumpos != std::string::npos)
        {
            result.replace(circumpos, 1, user_home("xpc66"));    /* Hmmm */
        }
        if (to_unix)
        {
            auto pos = path.find_first_of("\\");
            if (pos != std::string::npos)
                std::replace(result.begin(), result.end(), '\\', '/');

            if (terminate && result.back() != '/')
                result += "/";
        }
        else
        {
            auto pos = path.find_first_of("/");
            if (pos != std::string::npos)
                std::replace(result.begin(), result.end(), '/', '\\');

            if (terminate && result.back() != '\\')
                result += "\\";
        }
    }
    return result;
}

/**
 *  Gets the current date/time.
 *
 * \return
 *      Returns the current date and time as a string.
 */

std::string
current_date_time ()
{
    static char s_temp[64];
    static const char * const s_format = "%Y-%m-%d %H:%M:%S";
    time_t t;
    std::memset(s_temp, 0, sizeof s_temp);
    time(&t);

    struct tm * tm = localtime(&t);
    std::strftime(s_temp, sizeof s_temp - 1, s_format, tm);
    return std::string(s_temp);
}

/**
 *
 *  This is a simplistic string-conversion function; it requires that
 *  the source string be ASCII-encoded.
 *
 * Windows:
 *
 *  -#  Handle the trivial case of an empty string.
 *  -#  Determine the required length of the new string.
 *  -#  Construct a new string of required length.
 *  -#  Convert the old string to the new string.
 *
 * Linux:
 *
 *  Just do a simple assign.
 *
 */

std::wstring
widen_string (const std::string & source)
{
    if (source.empty())
        return std::wstring();          /* trivial case of empty string     */

#if defined SEQ66_PLATFORM_WINDOWS
    size_t required_length = ::MultiByteToWideChar
    (
        CP_UTF8, 0, source.c_str(), int(source.length()), 0, 0
    );
    std::wstring result(required_length, L'\0');
    ::MultiByteToWideChar
    (
        CP_UTF8, 0, source.c_str(), int(source.length()), &result[0], int(result.length())
    );
    return result;
#else
    std::wstring result;
    result.assign(source.begin(), source.end());
    return result;
#endif
}

}           // namespace xpc

/*
 * utilfunctions.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

