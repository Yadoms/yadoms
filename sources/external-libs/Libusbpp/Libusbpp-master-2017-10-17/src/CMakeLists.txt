#
# src/CMakeLists.txt
#

# Require libusb-1.0
find_package (LibUSB-1.0 REQUIRED)
if (LIBUSB1_FOUND)
    include_directories (${LIBUSB1_INCLUDE_DIR})
else (LIBUSB1_FOUND)
    message (FATAL_ERROR "LibUSB-1.0 was not found.")
endif (LIBUSB1_FOUND)

# On Windows do not prefix the DLLs with the 'lib' prefix
if (WIN32)
    set (LIB_PREFIX "")
else (WIN32)
    set (LIB_PREFIX "lib")
endif (WIN32)

# Create position independent code on all targets [auto]
# (needed for static build on non-x86):
set (CMAKE_POSITION_INDEPENDENT_CODE on)

# Collect all library source files
set (libusbppsrc
    Configuration.cpp
    ConfigurationImpl.cpp
    ConfigurationImpl.hpp
    Device.cpp
    DeviceImpl.cpp
    DeviceImpl.hpp
    Endpoint.cpp
    EndpointImpl.cpp
    EndpointImpl.hpp
    Exception.cpp
    Interface.cpp
    InterfaceImpl.cpp
    InterfaceImpl.hpp
    LibusbImpl.cpp
    LibusbImpl.hpp
    libusbpp.cpp
    Transfer.cpp
    TransferImpl.cpp
    TransferImpl.hpp
    Wideconvert.cpp
    Wideconvert.hpp
)

# Check if all sources were catched
message (STATUS "Searching for uncatched library sources...")
file (GLOB allsrc RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.hpp
    *.cpp
)
foreach (_src ${allsrc})
    list (FIND libusbppsrc ${_src} matchidx)
    if (${matchidx} EQUAL -1)
        message (AUTHOR_WARNING "  ${_src}")
    endif ()
endforeach ()

# This is the "object library" target:
# - compiles the sources only once
# - shared libraries need PIC
add_library (objlibusbpp OBJECT ${libusbppsrc})
set_property (TARGET ${objlibusbpp} PROPERTY POSITION_INDEPENDENT_CODE on)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories (objlibusbpp
    PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../headers>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Actual shared and static libraries built from the same object files
add_library (sharedlibusbpp SHARED $<TARGET_OBJECTS:objlibusbpp>)
target_link_libraries (sharedlibusbpp ${LIBUSB1_LIBRARIES})
set_target_properties (sharedlibusbpp
                       PROPERTIES OUTPUT_NAME "${LIBUSBPP_LIB_NAME}"
                                  PREFIX "${LIB_PREFIX}"
                                  IMPORT_PREFIX "${LIB_PREFIX}"
                                  VERSION "${LIBUSBPP_LIB_VERSION}"
                                  SOVERSION "${LIBUSBPP_LIB_SOVERSION}")

add_library (staticlibusbpp STATIC $<TARGET_OBJECTS:objlibusbpp>)
set_target_properties (staticlibusbpp
                       PROPERTIES OUTPUT_NAME "${LIBUSBPP_LIB_NAME}"
                                  PREFIX "${LIB_PREFIX}"
                                  IMPORT_PREFIX "${LIB_PREFIX}")

# Install shared and static libraries
install (TARGETS sharedlibusbpp staticlibusbpp EXPORT ${PACKAGE_NAME}Config
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows

# This makes the project importable from the build directory
export (TARGETS sharedlibusbpp staticlibusbpp FILE ${PACKAGE_NAME}Config.cmake)
