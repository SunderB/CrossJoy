# CMake Module made by SunderB - licensed unser the MIT Licence.
# (c) SunderB 2021. All rights reserved.
#[=======================================================================[.rst:
FindHIDAPI
-------

Finds the HIDAPI library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

Windows, macOS or BSD:
``HIDAPI::hidapi``
  The HIDAPI library

Linux:
``HIDAPI::hidapi-hidraw``
  The HIDAPI library implementation using hidraw

``HIDAPI::hidapi-libusb``
  The HIDAPI library implementation using libusb

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``HIDAPI_FOUND``
  True if the system has the HIDAPI library.
``HIDAPI_VERSION``
  The version of the HIDAPI library which was found.
``HIDAPI_INCLUDE_DIRS``
  Include directories needed to use HIDAPI.
``HIDAPI_LIBRARIES``
  Libraries needed to link to HIDAPI.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``HIDAPI_INCLUDE_DIR``
  The directory containing ``hidraw.h``.
``HIDAPI_LIBRARY``
  The path to the HIDRAW library.
``HIDAPI_HIDRAW_LIBRARY``
  The path to the HIDRAW library implementation using hidraw.
``HIDAPI_LIBUSB_LIBRARY``
  The path to the HIDRAW library implementation using libusb.


#]=======================================================================]

find_package(PkgConfig)
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # HIDAPI has two libraries on Linux: hidapi-hidraw and hidapi-libusb
  pkg_check_modules(PC_HIDAPI_HIDRAW QUIET hidapi-hidraw)
  pkg_check_modules(PC_HIDAPI_LIBUSB QUIET hidapi-libusb)

  find_path(HIDAPI_INCLUDE_DIR
    NAMES hidapi.h
    PATHS ${PC_HIDAPI_HIDRAW_INCLUDE_DIRS}
    PATH_SUFFIXES hidapi
  )
  set(HIDAPI_VERSION ${PC_HIDAPI_HIDRAW_VERSION})

  find_library(HIDAPI_HIDRAW_LIBRARY
    NAMES hidapi-hidraw
    PATHS ${PC_HIDAPI_HIDRAW_LIBRARY_DIRS}
  )
  find_library(HIDAPI_LIBUSB_LIBRARY
    NAMES hidapi-hidraw
    PATHS ${PC_HIDAPI_LIBUSB_LIBRARY_DIRS}
  )

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(HIDAPI
    FOUND_VAR HIDAPI_FOUND
    REQUIRED_VARS
      HIDAPI_HIDRAW_LIBRARY
      HIDAPI_LIBUSB_LIBRARY
      HIDAPI_INCLUDE_DIR
    VERSION_VAR HIDAPI_VERSION
  )

  if(HIDAPI_FOUND)
    set(HIDAPI_LIBRARIES ${HIDAPI_HIDRAW_LIBRARY} ${HIDAPI_LIBUSB_LIBRARY})
    set(HIDAPI_INCLUDE_DIRS ${HIDAPI_INCLUDE_DIR})
    set(HIDAPI_DEFINITIONS ${PC_HIDAPI_HIDRAW_CFLAGS_OTHER})
  endif()

  if(HIDAPI_FOUND AND NOT TARGET HIDAPI::hidapi-hidraw AND NOT TARGET HIDAPI::hidapi-libusb)
    add_library(HIDAPI::hidapi-hidraw UNKNOWN IMPORTED)
    set_target_properties(HIDAPI::hidapi-hidraw PROPERTIES
      IMPORTED_LOCATION "${HIDAPI_HIDRAW_LIBRARY}"
      INTERFACE_COMPILE_OPTIONS "${PC_HIDAPI_HIDRAW_CFLAGS_OTHER}"
      INTERFACE_INCLUDE_DIRECTORIES "${HIDAPI_INCLUDE_DIR}"
    )

    add_library(HIDAPI::hidapi-libusb UNKNOWN IMPORTED)
    set_target_properties(HIDAPI::hidapi-libusb PROPERTIES
      IMPORTED_LOCATION "${HIDAPI_LIBUSB_LIBRARY}"
      INTERFACE_COMPILE_OPTIONS "${PC_HIDAPI_LIBUSB_CFLAGS_OTHER}"
      INTERFACE_INCLUDE_DIRECTORIES "${HIDAPI_INCLUDE_DIR}"
    )
  endif()
else()
  pkg_check_modules(PC_HIDAPI QUIET hidapi)

  find_path(HIDAPI_INCLUDE_DIR
    NAMES hidapi.h
    PATHS ${PC_HIDAPI_INCLUDE_DIRS}
    PATH_SUFFIXES hidapi
  )
  find_library(HIDAPI_LIBRARY
    NAMES hidapi
    PATHS ${PC_HIDAPI_LIBRARY_DIRS}
  )
  set(HIDAPI_VERSION ${PC_HIDAPI_HIDRAW_VERSION})

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(HIDAPI
    FOUND_VAR HIDAPI_FOUND
    REQUIRED_VARS
      HIDAPI_LIBRARY
      HIDAPI_INCLUDE_DIR
    VERSION_VAR HIDAPI_VERSION
  )

  if(HIDAPI_FOUND)
    set(HIDAPI_LIBRARIES ${HIDAPI_LIBRARY})
    set(HIDAPI_INCLUDE_DIRS ${HIDAPI_INCLUDE_DIR})
    set(HIDAPI_DEFINITIONS ${PC_HIDAPI_CFLAGS_OTHER})
  endif()

  if(HIDAPI_FOUND AND NOT TARGET HIDAPI::hidapi)
    add_library(HIDAPI::hidapi UNKNOWN IMPORTED)
    set_target_properties(HIDAPI::hidapi PROPERTIES
      IMPORTED_LOCATION "${HIDAPI_LIBRARY}"
      INTERFACE_COMPILE_OPTIONS "${PC_HIDAPI_CFLAGS_OTHER}"
      INTERFACE_INCLUDE_DIRECTORIES "${HIDAPI_INCLUDE_DIR}"
    )
  endif()
endif()
