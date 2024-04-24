#*****************************************************************************
# meson.build (xpc66/include)
#-----------------------------------------------------------------------------
##
# \file        include/meson.build
# \library     xpc66
# \author      Chris Ahlstrom
# \date        2022-07-03
# \updates     2024-04-24
# \license     $XPC_SUITE_GPL_LICENSE$
#
#  This file is part of the "xpc66" library. See the top-level meson.build
#  file for license information.
#
#  Here we locate the headers.  They reside in:
#
#     -  include. The main include directory, for general definitions.
#     -  include/xpc. Contains headers refactored from the RtMidi project.
#
#-----------------------------------------------------------------------------

libxpc66_headers += files(
   'c_macros.h',
   'cpp_types.hpp',
   'platform_macros.h',
   'xpc66.hpp',
   'xpc_build_macros.h',
   'xpc_build_macros.h',
   'xpc/automutex.hpp',
   'xpc/condition.hpp',
   'xpc/daemonize.hpp',
   'xpc/recmutex.hpp',
   'xpc/ring_buffer.hpp',
   'xpc/shellexecute.hpp',
   'xpc/timing.hpp',
   'xpc/utilfunctions.hpp'
   )

configure_file(
   input: 'xpc66-config.h.in',
   output: 'xpc66-config.h',
   configuration: cdata
   )

#-----------------------------------------------------------------------------
# Headers are installed via the main meson.build file.
#
#     install_headers(libxpc66_headers)
#
#-----------------------------------------------------------------------------

#****************************************************************************
# meson.build (xpc66/include)
#----------------------------------------------------------------------------
# vim: ts=3 sw=3 ft=meson
#----------------------------------------------------------------------------
