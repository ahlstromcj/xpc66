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
 * \file          timing.cpp
 * \library       xpc66
 * \author        Chris Ahlstrom
 * \date          2005-07-03 to 2007-08-21 (pre-Sequencer24/64/Seq66)
 * \updates       2024-01-17
 * \license       GNU GPLv2 or above
 *
 *  Provides support for cross-platform time-related functions.
 */

#include "platform_macros.h"            /* detects the build platform       */
#include "xpc/timing.hpp"               /* xpc66::microsleep(), etc.        */

#if defined PLATFORM_UNIX               // PLATFORM_LINUX

#include <errno.h>                      /* error numbers                    */
#include <pthread.h>                    /* pthread_setschedparam()          */
#include <sched.h>                      /* sched_yield(), _get_priority()   */
#include <stdio.h>                      /* snprintf()                       */
#include <string.h>                     /* memset()                         */
#include <time.h>                       /* C::nanosleep(2)                  */
#include <unistd.h>                     /* exit(), setsid()                 */

#elif defined PLATFORM_WINDOWS

/*
 * For Windows, only the reroute_stdio() and microsleep() functions are
 * defined, currently.
 */

#include <windows.h>                    /* WaitForSingleObject(), INFINITE  */
#include <mmsystem.h>                   /* Win32 timeGetTime() [!timeapi.h] */
#include <synchapi.h>                   /* recent Windows "wait" functions  */

#endif

/*
 *  Do not document a namespace; it breaks Doxygen.
 */

namespace xpc
{

/**
 *  Provides a standard wait time in microsecond to use, in an explicit
 *  function. This microsleep() time is useful in avoiding general
 *  thread contention.
 */

int
std_sleep_us ()
{
    return 10;                              /* was c_default_sleep_time_us  */
}

/*
 * --------------------------------------------------------------------------
 *  microsleep() and millisleep()
 * --------------------------------------------------------------------------
 */

/*
 *  This free-function in the xpc namespace provides a way to suspend a
 *  thread for a small amount of time, or to yield the processor.
 *
 * \linux
 *      We can use the usleep(3) function.
 *
 * \unix
 *    In POSIX, select() can return early if any signal occurs.  We don't
 *    correct for that here at this time.  Actually, it is a convenient
 *    feature, and we wish that Sleep() would provide it.
 *
 * \win32
 *    In Windows, the Sleep(0) function does not sleep, but it does cede
 *    control of the thread to the operating system, which then schedules
 *    another thread to run.
 *
 * \warning
 *    Please note that this function isn't all that accurate for small
 *    sleep values, due to the time taken to set up the operation, and
 *    resolution issues in many operating systems.
 *
 * \param ms
 *    The number of milliseconds to "sleep".  For Linux and Windows, the
 *    microsleep() function handles this, including the case of ms == 0.
 *
 * \return
 *    Returns true if the ms parameter was 0 or greater.
 */

bool
millisleep (int ms)
{
    bool result = ms >= 0;
    if (result)
    {
#if defined PLATFORM_LINUX
        result = microsleep(ms * 1000);
#elif defined PLATFORM_UNIX
        struct timeval tv;
        struct timeval * tvptr = &tv;
        tv.tv_usec = long(ms % 1000) * 1000;
        tv.tv_sec = long(ms / 1000);
        result = select(0, 0, 0, 0, tvptr) != (-1);
#else
        result = microsleep(ms * 1000);
#endif
    }
    return result;
}

#if defined PLATFORM_UNIX               // PLATFORM_LINUX

/**
 *  Sleeps for the given number of microseconds. nanosleep() is a Linux
 *  function which has some advantage over sleep(3) and usleep(3), such as not
 *  interacting with signals.  It seems that it supports a non-busy wait.
 *
 *  The typical stack call is around 10 x 5 us, or 50 us.  We've been using 100
 *  us for calls to microsleep(), which does division, modulo, and
 *  multiplication calls.
 *
 * \param us
 *      Provides the desired number of microseconds to wait. Must be greater
 *      than 0.  Use thread_yield() if you just want to yield the CPU.
 *      Use std_sleep_us() if you want the normal sleep time.
 *
 * \return
 *      Returns true if the full sleep occurred, or if interruped by a signal.
 */

bool
microsleep (int us)
{
    bool result = us > 0;
    if (result)
    {
        int rc;
        if (us == std_sleep_us())                   /* an optimization      */
        {
            static bool s_uninitialized = true;
            static timespec s_ts;
            if (s_uninitialized)
            {
                s_uninitialized = false;
                s_ts.tv_sec = 0;
                s_ts.tv_nsec = us * 1000;
            }
            rc = nanosleep(&s_ts, NULL);
        }
        else
        {
            struct timespec ts;
            ts.tv_sec = us / 1000000;
            ts.tv_nsec = (us % 1000000) * 1000;     /* 1000 ns granularity  */
            rc = nanosleep(&ts, NULL);
        }
        result = rc == 0 || rc == EINTR;
    }
    return result;
}

#elif defined PLATFORM_WINDOWS

/**
 *  This implementation comes from https://gist.github.com/ngryman/6482577 and
 *  performs busy-waiting, meaning it will NOT relinquish the processor.
 *
 * \param us
 *      Provides the desired number of microseconds to wait. Must be greater
 *      than 0.
 *
 * \return
 *      Returns true only if all calls succeeded.  It doesn't matter if the
 *      wait completed, at this point.
 */

bool
microsleep (int us)
{
    bool result = us > 0;
    if (result)
    {
        HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
        bool result = timer != NULL;
        if (result)
        {
            LARGE_INTEGER ft;
            ft.QuadPart = -(10 * (__int64) us);
            result = SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0) != 0;
            if (result)
                result = WaitForSingleObject(timer, INFINITE) != WAIT_FAILED;

            CloseHandle(timer);
        }
    }
    return result;
}

#endif

/*
 * --------------------------------------------------------------------------
 *  thread_yield()
 * --------------------------------------------------------------------------
 */

#if defined PLATFORM_UNIX               // PLATFORM_LINUX

void
thread_yield ()
{
    (void) sched_yield();               /* always succeeds in Linux */
}

#elif defined PLATFORM_WINDOWS

void
thread_yield ()
{
    Sleep(0);
}

#endif

/*
 * --------------------------------------------------------------------------
 *  microtime() and millitime()
 * --------------------------------------------------------------------------
 */

#if defined PLATFORM_UNIX               // PLATFORM_LINUX

/**
 *  Gets the current system time in microseconds.
 *
 *  Should we try rounding off the nanoseconds here and in millitime()?
 */

long
microtime ()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (t.tv_sec * 1000000) + (t.tv_nsec * 0.0010);
}

/**
 *  Gets the current system time in milliseconds.
 *
 * Better?
 *
 *  return (t.tv_sec * 1000) + ((t.tv_nsec + 500000) / 1000000);
 */

long
millitime ()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (t.tv_sec * 1000) + (t.tv_nsec * 1.0e-6);
}

#elif defined PLATFORM_WINDOWS

/**
 *  Gets the current system time in microseconds.  Currently, we use
 *  millitime() and multiply by 1000, in effect what Seq32 does.
 *
 * GetSystemTimeAsFileTime:
 *
 *      It gives you accuracy in 0.1 microseconds or 100 nanoseconds.
 *
 *      Note that it's Epoch different from POSIX Epoch.
 *      So to get POSIX time in microseconds you need:
 *
\verbatim
            FILETIME ft;
            GetSystemTimeAsFileTime(&ft);
            unsigned long long tt = ft.dwHighDateTime;
            tt <<=32;
            tt |= ft.dwLowDateTime;
            tt /=10;
            tt -= 11644473600000000ULL;
\endverbatim
 *
 */

long
microtime ()
{
#if defined PLATFORM_WINDOWS__THIS_CODE_IS_READY  // NOT!
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    unsigned long long tt = ft.dwHighDateTime;
    tt <<=32;
    tt |= ft.dwLowDateTime;
    tt /=10;
    tt -= 11644473600000000ULL;
    return tt;
#else
    return millitime() * 1000;
#endif
}

/**
 *  Gets the current system time in milliseconds. Uses the Win32 function
 *  timeGetTime().
 *
 *  The default precision of timeGetTime() can be five milliseconds or more.
 *  One can use the timeBeginPeriod() and timeEndPeriod() functions to increase
 *  the precision of timeGetTime(). If done, the minimum difference between
 *  successive values returned by timeGetTime() can be as large as the minimum
 *  period value set using timeBeginPeriod() and timeEndPeriod(). Use
 *  QueryPerformanceCounter() and QueryPerformanceFrequency() to measure
 *  short time intervals at a high resolution.
 */

long
millitime ()
{
    return long(timeGetTime());
}

#endif

/*
 * --------------------------------------------------------------------------
 *  set_thread_priority() and set_timer_services()
 * --------------------------------------------------------------------------
 */

#if defined PLATFORM_UNIX               // PLATFORM_LINUX

/**
 * In Linux, sets the thread priority for the calling thread, either the
 * performer input thread or output thread.
 *
 * \param p
 *      This is the desired priority of the thread, ranging from 1 (low), to
 *      99 (high).  The default value is 1.
 *
 * \return
 *      Returns true if the scheduling call succeeded.
 */

bool
set_thread_priority (std::thread & t, int p)
{
    int minp = sched_get_priority_min(SCHED_FIFO);
    int maxp = sched_get_priority_max(SCHED_FIFO);
    if (p >= minp && p <= maxp)
    {
        struct sched_param schp;
        memset(&schp, 0, sizeof(sched_param));
        schp.sched_priority = p;                /* Linux range: 1 to 99 */
#if defined PLATFORM_PTHREADS
        int rc = pthread_setschedparam(t.native_handle(), SCHED_FIFO, &schp);
#else
        int rc = sched_setscheduler(t.native_handle(), SCHED_FIFO, &schp);
#endif
        return rc == 0;
    }
    else
    {
        char temp[80];
        snprintf
        (
            temp, sizeof temp,
            "Priority %d outside of range %d-%d", p, minp, maxp
        );
        //// error_message(temp);
        return false;
    }
}

/**
 *  Linux doesn't need this, so we just act like it works.
 */

bool
set_timer_services (bool /*on*/)
{
    return true;
}

#elif defined PLATFORM_WINDOWS

/**
 *  In Windows, currently does nothing.  An upgrade for the future.
 *  The handle must have the THREAD_SET_INFORMATION or
 *  THREAD_SET_LIMITED_INFORMATION access right.  The main priority values are
 *
 *      0:  THREAD_PRIORITY_NORMAL
 *      1:  THREAD_PRIORITY_ABOVE_NORMAL
 *      2:  THREAD_PRIORITY_HIGHEST
 *     15:  THREAD_PRIORITY_TIME_CRITICAL
 *
 *  See https://docs.microsoft.com/en-us/windows/win32/api/
 *          processthreadsapi/nf-processthreadsapi-setthreadpriority
 *
 * \param p
 *      This is the desired priority of the thread. For Windows we restrict it
 *      to the first three values shown above.
 *
 * \return
 *      Returns true if p > 0; no functionality at present.
 */

bool
set_thread_priority (std::thread & t, int p)
{
#if defined PLATFORM_WINDOWS__THIS_CODE_IS_READY
    bool result = false;
    if (p >= THREAD_PRIORITY_NORMAL && p <= THREAD_PRIORITY_HIGHEST)
    {
        HANDLE hthread = t.native_handle();
        BOOL ok = SetThreadPriority(hthread, p);
        result = ok != 0;
        if (! result)
        {
            //// error_message("Windows set-priority error... access rights?");
        }
    }
    return result;
#else
    return p > 0 || t.joinable();
#endif
}

/**
 *  Necessary for proper input and output timing using our portmidi
 *  implementation under windows.
 */

bool
set_timer_services (bool on)
{
    MMRESULT mmr = on ? timeBeginPeriod(1) : timeEndPeriod(1) ;
    return mmr == TIMERR_NOERROR;
}

#endif      // PLATFORM_LINUX, PLATFORM_WINDOWS

}           // namespace xpc

/*
 * vim: ts=4 sw=4 et ft=cpp
 */

