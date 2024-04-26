#!/bin/sh
#
#******************************************************************************
# work.sh (xpc66)
#------------------------------------------------------------------------------
##
# \file           work.sh
# \library        xpc66
# \author         Chris Ahlstrom
# \date           2024-02-06
# \update         2024-04-26
# \version        $Revision$
# \license        $XPC_SUITE_GPL_LICENSE$
#
#     The above is modified by the following to remove even the mild GPL
#     restrictions:
#
#     Use this script in any manner whatsoever.  You don't even need to give
#     me any credit.  However, keep in mind the value of the GPL in keeping
#     software and its descendant modifications available to the community for
#     all time.  Runs the configure script by default.
#
#     This script encapsulates some common commands.
#
#     Also see:  https://mesonbuild.com/Creating-releases.html
#
#------------------------------------------------------------------------------

LANG=C
export LANG
CYGWIN=binmode
export CYGWIN
export XPC66_SCRIPT_EDIT_DATE="2024-04-26"
export XPC66_LIBRARY_API_VERSION="0.1"
export XPC66_LIBRARY_VERSION="$XPC66_LIBRARY_API_VERSION.0"
export XPC66="xpc66"
export XPC66_LIBRARY="$XPC66-0.1"

PLATFORM="UNIX"
INSTALL_PREFIX="/usr/local"

DOCLANG="no"         # --clang. Default is the native compiler.
DOGNU="no"           # --gnu. Default is the native compiler.
DOCLEAN="no"         # --clean
DODEBUG="yes"        # --debug. This is the default Meson build.
DODIST="no"          # --dist. Use Meson "dist" to create a package.
DOHELP="no"          # --help. Duh!
DOINSTALL="no"       # --install. Requires the release be built already.
DOUNINSTALL="no"     # --uninstall. Like --install, requires sudo/root.
DOMAKE="yes"         # Default action after creating the build directory.
DOREMAKE="no"        # currently UNUSED
DOMAKEPDF="no"       # --pdf. Make the manual, a separate step, NOT READY.
DOPACK="no"          # --pack. Clean and create a tar-file.
DORELEASE="no"       # --release. as opposed to debug; also PDF made
DOSTATIC="yes"       # --static
DOVERSION="no"       # --version. Duouble duh!
EXTRAFLAGS=""
MAKEFILE="./build/build.ninja"
TAGSTRING="pack"

#******************************************************************************
#  Brute-force options loop
#------------------------------------------------------------------------------

if test $# -ge 1 ; then

   while [ "$1" != "" ] ; do

      case "$1" in

         --clang)
            DOCLANG="yes"
            ;;

         --gnu)
            DOGNU="yes"
            ;;

         --clean)
            DOCLEAN="yes"
            DOMAKE="no"
            ;;

         --help)
            DOHELP="yes"
            DOMAKE="no"
            ;;

         --build | --make)
            DOMAKE="yes"
            ;;

         --install)
            DOINSTALL="yes"
            DOMAKE="no"
            ;;

         --uninstall)
            DOUNINSTALL="yes"
            DOMAKE="no"
            ;;

         --dist)
            DODIST="yes"
            DOMAKE="no"
            ;;

         --pdf)
            DOMAKEPDF="yes"
            DOMAKE="no"
            ;;

         --pack)
            DOCLEAN="yes"
            DOMAKE="no"
            DOPACK="yes"
            ;;

         --debug)
            DOMAKE="yes"
            DODEBUG="yes"
            DORELEASE="no"
            ;;

         --release)
            DOMAKE="yes"
            DORELEASE="yes"
            DODEBUG="no"
            DOMAKEPDF="yes"
            ;;

         --static)
            DORELEASE="yes"
            DODEBUG="no"
            DOSTATIC="yes"
            ;;

         --version)
            DOVERSION="yes"
            DOBOOTSTRAP="no"
            ;;

         *)
            if test "$DOPACK" = "no" ; then
               echo "? Unsupported work option; --help for more information"
               exit 1
            else
               TAGSTRING="$1"
            fi
            ;;

      esac
      shift
   done
fi

#******************************************************************************
#  Version info
#------------------------------------------------------------------------------

if test "$DOVERSION" = "yes" ; then
   echo "Potext version $XPC66_LIBRARY_VERSION $XPC66_SCRIPT_EDIT_DATE"
   exit 0
fi

#******************************************************************************
#  Help
#------------------------------------------------------------------------------

if test "$DOHELP" = "yes" ; then

   cat << E_O_F
Usage: ./work [options]    ($XPC66_LIBRARY_VERSION-$XPC66_SCRIPT_EDIT_DATE)

'work' encapsulates some common operations involving Meson, builds, packing,
and version information.  Only implemented options are shown here; there will
be more to come. Some options might not work on Windows.

 --make or --build   Build the code in 'build'. The default operation.
 --release           Build release version (Meson defaults to a debug version).
                     Also builds the PDF documentation.
 --install           Run 'meson install' to install the library and PDF.
 --uninstall         Run 'ninja uninstall' to uninstall the library.
                     Some directories might remain; there is a error about
                     one header file not being found... strange.
 --dist              Make a Meson dist package and exit.
 --clang             Rebuild the code using the Clang compilers.
 --pdf               Build just the PDF documentation and exit. In progress.
 --clean             Delete the usual derived files from the project. Also
                     do "git checkout doc/xpc66-developer-guide.pdf"
 --pack [ tag ]      A simple quick packaging of the code; the tag goes
                     into the tarball name.
 --help              Show this help text.
 --version           Show only the version information.

E_O_F
   exit 0

fi

if test "$DODIST" = "yes" ; then

   meson dist
   exit 0

fi

# Make the PDF, then exist if not creating a release.

if test "$DOMAKEPDF" = "yes" ; then

   cd doc/latex
   ./make_pdf.sh
   cd ../..
   if test "$DORELEASE" = "no" ; then
      exit 0
   fi
fi

# This removes the work products, but leaves the README intact.

PROJECTSDIR="libraries"

if test $DOCLEAN = "yes" ; then
   rm -rf build/doc/
   rm -rf build/doxlatex/
   rm -rf build/include/
   rm -rf build/latex/
   rm -rf build/src/
   rm -rf build/tests/
   rm -rf build/meson*
   rm -rf build/lib*
   rm -f build/.ninja_deps
   rm -f build/.ninja_log
   rm -f build/*.h
   rm -f build/*.log
   rm -f build/*.so
   rm -f $MAKEFILE
   rm -f build/compile_commands.json
   rm -rf wipe/
   rm -f doc/dox/*.log
   rm -f doc/latex/*.log
   echo "Build products removed from the xpc66/build directory."
#  git checkout doc/xpc66-developer-guide.pdf
   echo "Previous version of developer guide restored."
fi

# This is just a quick pack, with date and branch information added.

if test $DOPACK = "yes" ; then

   CURRENTDATE=$(date +%Y-%m-%d)
   CURRENTDIR=$(pwd)
   WORKINGDIR=${CURRENTDIR##/*/}       # strip all but last part of path
   PACKAGENAME="bogus"
   BRANCH=`git symbolic-ref -q HEAD 2> /dev/null`

   if test $? = 0 ; then
      ISGITBRANCH="yes"
      GITBRANCH=${BRANCH##*/}
   else
      ISGITBRANCH="no"
      GITBRANCH=""
   fi

   echo "Working directory: $WORKINGDIR"
   if test "$ISGITBRANCH" = "yes" ; then
         TARBALLNAME="$WORKINGDIR-$GITBRANCH-$CURRENTDATE-$TAGSTRING.tar.xz"
         echo "Git branch detected."
   else
         TARBALLNAME="$WORKINGDIR-$CURRENTDATE-$TAGSTRING.tar.xz"
   fi

   echo "The tar-ball to be generated is '../$TARBALLNAME'"

   cd ..
   if test -d $WORKINGDIR ; then

      tar cJf $TARBALLNAME \
 --exclude-vcs \
 --exclude=".git" \
 --exclude="moc" \
 --exclude="html" \
 --exclude="*stamp*" \
 --exclude="*.bz" \
 --exclude="*.bz2" \
 --exclude="*.gz" \
 --exclude=".*.swp" \
 --exclude="*.t" \
 --exclude="*.xz" \
 --exclude="*.zip" \
 $WORKINGDIR
      echo "Packed!"
   else
      echo "? Working directory $WORKINGDIR does not exist."
      echo "  Are you running pack from the proper directory?"
      echo "  That is what you must do."
   fi
   cd $XPC66

fi

if test "$DOMAKE" = "yes" ; then

# TODO: use a separate build directory for Clang.
#
# $ CC=clang CXX=clang++ meson setup buildclang
#
# https://mesonbuild.com/Running-Meson.html

   if test "$DOCLANG" = "yes" ; then
      echo "Using the Clang C/C++ compilers..."
      export CC=clang
      export CXX=clang++
   elif test "$DOGNU" = "yes" ; then
      echo "Using the GNU C/C++ compilers..."
      export CC=gcc
      export CXX=g++
   else
      echo "Using the default C/C++ compilers..."
   fi

   echo "Making all xpc66 libraries..."

   NINJA_EXISTS="no"
   if test -f "$MAKEFILE" ; then
      NINJA_EXISTS="yes"
   fi

   if test "$DOREMAKE" = "yes" ; then
      if test "$NINJA_EXISTS" = "yes" ; then
         echo "$MAKEFILE exists, reconfiguring..."
         meson --reconfigure . build
      fi
   fi

   if test "$NINJA_EXISTS" = "no" ; then
      echo "New configuration, creating $MAKEFILE, etc...."
      if test "$DODEBUG" = "yes" ; then
         meson setup --buildtype=debug --default-library=static build/
         echo "... for debugging"
      else
         meson setup --buildtype=release build/
         echo "... for release"
      fi
   fi

   # Could also run "meson compile" here.  The --verbose option is not
   # present on older ninjas, so we use -v here.

   cd build
   ninja -v > make.log
   if test $? = 0 ; then
      if test "$DODEBUG" = "yes" ; then
         echo "Debug build succeeded."
      else
         echo "Release build succeeded."
      fi
   else
      if test "$DODEBUG" = "yes" ; then
         echo "Debug build failed, check build/make.log for errors."
      else
         echo "Release build failed, check build/make.log for errors."
      fi
   fi
   cd ..

fi

# Check for root, then install. We could let meson prompt the user
# to automatically become root. Note that there are two possible locations
# for the library and pkgconfig to be installed on Linux:
#
#     -  /usr/local/lib/
#     -  /usr/local/lib/x86_64-linux-gnu/

if test "$DOINSTALL" = "yes" ; then

   USERID=$(id -u)
   if test "$USERID" = 0 ; then
      cd build
      echo "Installing the xpc66 library..."
      meson install
      cd ..
   else
      echo "We want you to be root to install the xpc66 library..."
   fi

fi

# Uninstallation is odd with Meson. The following does not work. And
# the call to ninja does not remove some directories.
#
#     cd build
#     meson --internal uninstall
#     cd ..
#
# Note that there are no man pages yet.
#
# We could 


if test "$DOUNINSTALL" = "yes" ; then

   USERID=$(id -u)
   if test "$USERID" = 0 ; then
      echo "Uninstalling the cfg66 library..."
      ninja -C build uninstall
      if test "$PLATFORM" = "UNIX" ; then
         rm -rf "$INSTALL_PREFIX/include/$XPC66_LIBRARY"
         rm -rf "$INSTALL_PREFIX/lib/$XPC66_LIBRARY"
         rm -rf "$INSTALL_PREFIX/share/doc/$XPC66"
#        rm -rf "$INSTALL_PREFIX/man/man1/$XPC66.1"
      fi
   else
      echo "UID $USERID. We want you as root to uninstall the cfg66 library..."
   fi

fi

#******************************************************************************
# work.sh (xpc66)
#------------------------------------------------------------------------------
# vim: ts=3 sw=3 wm=4 et ft=sh
#------------------------------------------------------------------------------
