# README for Library Xpc66 0.1.0

Chris Ahlstrom
2022-07-03 to 2024-04-17

__Xpc66__ is a cross-platform utilities library based on the "util" and "os"
directories of the __Seq66__ project, but that is useful in other applications.
It is a work in progress and the precursor to a potential Seq66 version 2.

Support sites (still in progress):

    *   https://ahlstromcj.github.io/
    *   https://github.com/ahlstromcj/ahlstromcj.github.io/wiki

# Major Features

    *   C++ recursive mutex and synchronization support.
    *   Utilites for daemonizing a C++ application, rerouting standard
        I/O, and generic signalling of session changes. Partly based
        on Kerrisk's "The Linux Programming Interface".
    *   A ring-buffer implementation with test code.
    *   Functions for executing command lines and displaying URLs or
        PDFs in Linux/UNIX and Windows.
    *   Functions for measuring time and "sleeping".
    *   PDF documentation built from LaTex.
    *   Basic dependencies: Meson 1.1 and above; C++14 and above.

##  Additional Features (in progress)

    *   To do. Beef up testing and the LaTeX documentation; use as a
        Meson subproject.

##  Internal

    *   Broken into modules for easier maintenance.

## Recent Changes

    *   Version 0.1.0:
        *   Usage of meson instead of autotools or cmake.

// vim: sw=4 ts=4 wm=2 et ft=markdown
