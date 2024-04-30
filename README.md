# README for Library Xpc66 0.1.1 2024-04-30

__Xpc66__ is a cross-platform utilities library based on the "util" and "os"
directories of the __Seq66__ project, but that is useful in other applications.
It is a work in progress and the precursor to a potential Seq66 version 2.

Support sites (still in progress):

    *   https://ahlstromcj.github.io/
    *   https://github.com/ahlstromcj/ahlstromcj.github.io/wiki

# Major Features

    Note that a work.sh script is provided to simplify or clarify various
    operations such as cleaning, building, making a release, and installing
    or uninstalling the library.

##  Library Features

    *   Can be built using GNU C++ or Clang C++.
    *   C++ recursive mutex and synchronization support.
    *   Utilites for daemonizing a C++ application, rerouting standard
        I/O, and generic signalling of session changes. Partly based
        on Kerrisk's "The Linux Programming Interface".
    *   A ring-buffer implementation with test code.
    *   Functions for executing command lines and displaying URLs or
        PDFs in Linux/UNIX and Windows.
    *   Functions for measuring time and "sleeping".
    *   Basic dependencies: Meson 1.1 and above; C++14 and above.
    *   The build system is Meson, and sample wrap files are provided
        for using Xpc66 as a C++ subproject.
    *   PDF documentation built from LaTeX.

##  Code

    *   The code is a mix of hard-core C++ and C-like functions.
    *   The C++ STL and advanced language features are used as much as
    *   possible
    *   C++14 is required for some of its features.
    *   The GNU and Clang C++ compilers are supported.
    *   Broken into modules for easier maintenance.

##  Fixes

    *   Improved the work.sh, added an --uninstall option.

##  Documentation

    *   A PDF developers guide is in progress.

## To Do

    *   Beef up testing; it is currently minimal.
    *   Beef up the LaTeX documentation.

## Recent Changes

    *   Version 0.1.0:
        *   Usage of meson instead of autotools or cmake.
    *   Version 0.1.1:
        *   Many fixes to Meson files and the build script.
        *   Improvements in use as a Meson subproject.
        *   Moved msglevel and tokenization into "lib66" namespace.

// vim: sw=4 ts=4 wm=2 et ft=markdown
