#*****************************************************************************
# meson.build (xpc66/include)
#-----------------------------------------------------------------------------
##
# \file        include/meson.build
# \library     xpc66
# \author      Chris Ahlstrom
# \date        2022-07-03
# \updates     2024-04-15
# \license     $XPC_SUITE_GPL_LICENSE$
#
#  This file is part of the "xpc66" library. See the top-level meson.build
#  file for license information.
#
#  Here we locate the headers.  They reside in:
#
#     -  include. The main include directory, for general definitions.
#     -  include/midi. Contains headers adapted from the Seq66 project.
#     -  include/xpc. Contains headers refactored from the RtMidi project.
#     -  include/transport. Will contain headers related to JACK transport
#        (and perhaps other transport like Pipewire).
#
#-----------------------------------------------------------------------------

libxpc66_headers += files(
   'xpc66.hpp',
   'xpc_build_macros.h',
   'c_macros.h',
   'cpp_types.hpp',
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

#****************************************************************************
# meson.build (xpc66/include)
#----------------------------------------------------------------------------
# vim: ts=3 sw=3 ft=meson
#----------------------------------------------------------------------------
