#*****************************************************************************
# meson.build (xpc66)
#-----------------------------------------------------------------------------
##
# \file        meson.build
# \library     xpc66
# \author      Chris Ahlstrom
# \date        2022-07-03
# \updates     2023-04-17
# \license     $XPC_SUITE_GPL_LICENSE$
#
#  This file is part of the "xpc66" library. It was part of the libs66
#  collection of libraries, but is now separate and stands alone.
#
#  The "xpc66" library is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  The "xpc66" library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the "xpc66" library. If not, see <https://www.gnu.org/licenses/>.
#
#-----------------------------------------------------------------------------

project('xpc66',
   [ 'c', 'cpp' ],
   version: '0.1.0',
   license: 'GPLv3',
   meson_version: '>=1.1.0',
   default_options: [
      'c_std=c11', 'cpp_std=c++14', 'warning_level=3',
      'default_library=both'
      ]
   )

#-----------------------------------------------------------------------------
# Information for this sub-project.
#-----------------------------------------------------------------------------

xpc66_description = 'Refactoring the base Seq66 OS-dependent modules.'
xpc66_date = '2024-04-17'
xpc66_version = meson.project_version()
xpc66_api_version = '0.1'
xpc66_build_type = get_option('buildtype')
xpc66_debug = get_option('buildtype').contains('debug')
xpc66_project_base = 'xpc66-@0@'.format(xpc66_api_version)
xpc66_project_root = meson.project_source_root()
xpc66_libtype = get_option('default_library')
xpc66_subproj = meson.is_subproject()

if xpc66_debug
   if not meson.is_subproject()
      add_global_arguments('-DDEBUG', language : 'cpp')
   endif
endif

#-----------------------------------------------------------------------------
# Easy access to directory options.  Interim until meson version 0.64.0
# We want to be able to install the header files to subdirectories of,
# for example, /usr/local/include/xpc66-0.1, just as they appear in this
# project.
#-----------------------------------------------------------------------------

xpc66_dir = 'xpc66-@0@'.format(xpc66_api_version)
xpc66_prefix = get_option('prefix')
xpc66_bindir = join_paths(xpc66_prefix, get_option('bindir'))
xpc66_includedir = join_paths(xpc66_prefix, get_option('includedir'), xpc66_dir)
xpc66_libdir = join_paths(xpc66_prefix, get_option('libdir'), xpc66_dir)
xpc66_datadir = join_paths(xpc66_prefix, get_option('datadir'), xpc66_dir)
xpc66_localedir = join_paths(xpc66_prefix, get_option('localedir'))
xpc66_metainfodir = join_paths(xpc66_datadir, 'metainfo')
xpc66_docdir = join_paths(xpc66_prefix, xpc66_datadir, 'doc', xpc66_dir)
xpc66_project_root = meson.project_source_root()
xpc66_libtype = get_option('default_library')
libraries_include_top = '.'
libxpc66_include_top = join_paths(libraries_include_top, 'include')

#-----------------------------------------------------------------------------
# The *.pc file goes into $libdir/xpc66-0.1/pkgconfig instead of
# $libdir/pkgconfig as the meson documentation asserts.  Not sure why, but
# wiring the expected directory here works.
#-----------------------------------------------------------------------------

alt_pkgconfig_libdir = join_paths(
   xpc66_prefix, get_option('libdir'), 'pkgconfig'
   )

#-----------------------------------------------------------------------------
# libxpc66_headers provides the list of all headers in the project. These are
# currently all installable, public headers. They are set in
# include/meson.build.
#
# libxpc66_sources provides the source-code files. They are set in
# src/meson.build.
#-----------------------------------------------------------------------------

libxpc66_headers = []
libxpc66_sources = []
build_args = []

#-----------------------------------------------------------------------------
# Unlike the cfg66 library, this library will have platform-dependent
# characteristics.
#-----------------------------------------------------------------------------

os_unix   = false
os_linux  = false
os_win32  = false
os_darwin = false

if host_machine.system() == 'darwin'
   os_darwin = true
elif host_machine.system() == 'windows'
   os_win32 = true
elif host_machine.system() == 'linux'
   os_linux = true
endif

os_unix = not os_win32

#-----------------------------------------------------------------------------
# Each include file directory is added to the list passed to the compiler.
# We prefer to use the style '#include "cfg/header.hpp".
#-----------------------------------------------------------------------------

libxpc66_includedirs = include_directories(libxpc66_include_top,
   'include',
   'include/xpc'
   )

build_args += [
   '-DXPC66_NAME=' + meson.project_name(),
   '-DXPC66_VERSION=' + meson.project_version()
   ]

#-----------------------------------------------------------------------------
# Set up some "HAVE" macros.  By default the header file is generated in the
# "build" directory. If configure_file() is moved to include/meson.build,
# the the header goes into "build/include", which is what we want.
#-----------------------------------------------------------------------------

cc = meson.get_compiler('cpp')
cdata = configuration_data()
cdata.set('fcntl_h', cc.has_header('fcntl.h'))
cdata.set('limits_h', cc.has_header('limits.h'))

#-----------------------------------------------------------------------------
# Sub-directories. 'tests' has to be descended into later.
#-----------------------------------------------------------------------------

subdir('include')
subdir('src')

#-----------------------------------------------------------------------------
# Dependencies on Linux
#-----------------------------------------------------------------------------
#
# Later:
#
#  if host_machine.system() == 'linux'
#     . . .
#  endif
#
#-----------------------------------------------------------------------------

empty_depends = [ ]

#-----------------------------------------------------------------------------
# Only make public interfaces visible
#-----------------------------------------------------------------------------

if target_machine.system() == 'windows' or target_machine.system() == 'cygwin'
  build_args += '-DLIBPUBLIC="__declspec(dllexport)"'
else
  build_args += '-DLIBPUBLIC=__attribute__((visibility("default")))'
endif

#-----------------------------------------------------------------------------
# We recommemd using a recent version of meson by installing it outside the
# Linux distro's repository.  For example, meson on Ubuntu is pretty damn
# old.
#
# The choice of library type is of some import for debugging:
#
#     library:          Defaults to a shared library. The User setting is
#                       'default_library'.  See project() above.
#     shared_library:   Cannot see source files until stepped into.
#     static_library:   Can see the source files when debugging.
#
# This one causes the linker to not see the library functions, wotta pain:
#
#     gnu_symbol_visibility: 'hidden'
#
# Also see the https://gcc.gnu.org/wiki/Visibility site.
#
#-----------------------------------------------------------------------------

xpc66_library = static_library(
   xpc66_project_base,
   libxpc66_sources,
   install: true,
   install_dir: xpc66_libdir,
   c_args: build_args,
   dependencies: empty_depends,
   include_directories: [ libxpc66_includedirs ]
   )

#-----------------------------------------------------------------------------
# Make this library usable as a Meson subproject.
#-----------------------------------------------------------------------------

xpc66_dep = declare_dependency(
   include_directories: libxpc66_includedirs,
   link_with: xpc66_library
   )

#-----------------------------------------------------------------------------
# Make this library usable from the system's package manager.
#-----------------------------------------------------------------------------

install_headers(libxpc66_headers, subdir: xpc66_dir, preserve_path: true)

#-----------------------------------------------------------------------------
# Package config
#-----------------------------------------------------------------------------

pkg_mod = import('pkgconfig')

pkg_mod.generate(
   xpc66_library,
   filebase: xpc66_project_base,
   version: xpc66_version,
   name: xpc66_project_base,
   description: xpc66_description,
   install_dir: alt_pkgconfig_libdir,
   subdirs: xpc66_dir,                  # not xpc66_project_base
   libraries: xpc66_library
   )

if get_option('enable-tests')
   subdir('tests')
endif

if not meson.is_subproject()

# TODO: figure out where to put and install the logo

   install_data(
      files(
         'ChangeLog',
         'LICENSE.md',
         'README.md',
         'doc/xpc66-library-guide.pdf'
         ),
      install_dir : get_option('docdir')
      )

endif

#-----------------------------------------------------------------------------
# Info
#-----------------------------------------------------------------------------

summary(
   {
      'Date': xpc66_date,
      'Filebase': xpc66_project_base,
      'Version': xpc66_version,
      'Base': xpc66_project_base,
      'Description': xpc66_description,
      'Root': xpc66_project_root,
      'Sub Dir': xpc66_project_base,
      'Build Type': xpc66_build_type,
      'Library Type': xpc66_libtype,
      'Subproject?': xpc66_subproj
   },
   section: 'Xpc66 Project'
   )

summary(
   {
      'prefix': xpc66_prefix,
      'includedir': xpc66_includedir,
      'bindir': xpc66_bindir,
      'libdir': xpc66_libdir,
      'datadir': xpc66_datadir,
      'appdatadir': xpc66_datadir,
      'docdir': xpc66_docdir
   },
   section: 'Xpc66 Install Directories'
   )

#****************************************************************************
# meson.build (xpc66)
#----------------------------------------------------------------------------
# vim: ts=3 sw=3 ft=meson
#----------------------------------------------------------------------------
