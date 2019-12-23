# - Try to find the LibUSB 1.0 library
# Once done this will define
#
#  LIBUSB1_FOUND - system has LibUSB 1.0
#  LIBUSB1_INCLUDE_DIR - the LibUSB 1.0 include directory
#  LIBUSB1_LIBRARIES - Link these to LibUSB 1.0
#  LIBUSB1_DEFINITIONS - Compiler switches required for using LibUSB 1.0
#  LIBUSB1_VERSION_STRING - version of LibUSB 1.0 found (since CMake 2.8.8)

#=============================================================================
# Copyright 2016 Stephan Linz <linz@li-pro.net>
#
# Copied from FindLibXslt.cmake
#
# Copyright 2006-2009 Kitware, Inc.
# Copyright 2006 Alexander Neundorf <neundorf@kde.org>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# use pkg-config to get the directories and then use these values
# in the find_path() and find_library() calls
find_package(PkgConfig QUIET)
PKG_CHECK_MODULES(PC_LIBUSB1 QUIET libusb-1.0)
set(LIBUSB1_DEFINITIONS ${PC_LIBUSB1_CFLAGS_OTHER})

find_path(LIBUSB1_INCLUDE_DIR NAMES libusb.h
    HINTS ${PC_LIBUSB1_INCLUDEDIR}
          ${PC_LIBUSB1_INCLUDE_DIRS}
    PATH_SUFFIXES libusb-1.0)

find_library(LIBUSB1_LIBRARIES NAMES usb-1.0 libusb-1.0
    HINTS ${PC_LIBUSB1_LIBDIR}
          ${PC_LIBUSB1_LIBRARY_DIRS})

if(PC_LIBUSB1_VERSION)
    set(LIBUSB1_VERSION_STRING ${PC_LIBUSB1_VERSION})
elseif(LIBUSB1_INCLUDE_DIR AND EXISTS "${LIBUSB1_INCLUDE_DIR}/libusb.h")
    file(STRINGS "${LIBUSB1_INCLUDE_DIR}/libusb.h" libusb1_version_str
         REGEX "^#define[\t ]+LIBUSB._API_VERSION[\t ]+0x0.*")

    string(REGEX REPLACE "^#define[\t ]+LIBUSB._API_VERSION[\t ]+(0x.*).*" "\\1"
           LIBUSB1_VERSION_STRING "${libusb1_version_str}")
    unset(libusb1_version_str)
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibUSB1
        REQUIRED_VARS LIBUSB1_LIBRARIES LIBUSB1_INCLUDE_DIR
        VERSION_VAR LIBUSB1_VERSION_STRING)

mark_as_advanced(LIBUSB1_INCLUDE_DIR
                 LIBUSB1_LIBRARIES)
