#
# To build out-of-source do (example):
#
#	mkdir build
#	cd build
#	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
#
# CMAKE_BUILD_TYPE can be: empty, Debug, Release, RelWithDebInfo or MinSizeRel
#

# Define minimum CMake version required
cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)

# Define the project name belong their language.
# This top-leve project name can be refered by refered exactly by
# ${CMAKE_PROJECT_NAME} -- any overwritten project name can be refered by
# ${PROJECT_NAME}.
project (libusbpp CXX)

# The project package version.
set (PROJECT_VERSION_MAJOR 1)
set (PROJECT_VERSION_MINOR 0)
set (PROJECT_VERSION_PATCH 0)

# The project package version.
set (PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# The package configuration name.
set (PACKAGE_NAME "${PROJECT_NAME}")

# Define a short description of the project (only a few words).
set (PACKAGE_DESCRIPTION "C++ wrapper API of C API for USB device access (libusb-1.0)")

# The package configuration version.
set (PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

# The package full name with version
set (PACKAGE "${PACKAGE_NAME}-${PACKAGE_VERSION}")

# The API version (SOVERSION) of the USBPP libraries.
# Increment this when more or fewer functions are included in the
# library, the function prototype changes, or data type changes.
#
# The SOVERSION is not necessarily the same as the major version.
# The rule is that any breakage of the ABI must be indicated by
# incrementing the SOVERSION. So, adding e.g. functions is no
# problem, modifying argument lists or removing functions would
# required the SOVERSION to be incremented. Similar rules hold of
# course for non-opaque data-structures. For C++ the whole issue
# is infinitely more complicated, as determining whether the ABI
# was changed in an incompatible way is AFAIK nearly impossible.
#
# Refer to:
# cmake --help-command set_target_properties
# cmake --help-property SOVERSION
# https://community.kde.org/Policies/Binary_Compatibility_Issues_With_C++
#
set (LIBUSBPP_LIB_SOVERSION 0)

# The build version (VERSION) of the USBPP libraries.
set (LIBUSBPP_LIB_VERSION "${LIBUSBPP_LIB_SOVERSION}.${PACKAGE_VERSION}")

# The build name (OUTPUT_NAME) of the USBPP libraries.
set (LIBUSBPP_LIB_NAME "usbpp-${PACKAGE_VERSION}")

# Make sure the user doesn't play dirty with symlinks
get_filename_component (srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component (bindir "${CMAKE_BINARY_DIR}" REALPATH)

# Disallow in-source builds
if (${srcdir} STREQUAL ${bindir})
	message(FATAL_ERROR "In-source builds are not allowed. "
	"Please create a directory and run cmake from there, passing the path "
	"to this source directory as the last argument. This process created "
	"the file `CMakeCache.txt' and the directory `CMakeFiles' in ${srcdir}. "
	"Please remove them.")
endif (${srcdir} STREQUAL ${bindir})

# Use NEW behavior with newer CMake releases
foreach (p
		CMP0025 # CMake 3.0: Compiler id for Apple Clang is now AppleClang
		)
	if (POLICY ${p})
		cmake_policy(SET ${p} NEW)
	endif()
endforeach()

# Use OLD behavior with newer CMake releases
foreach (p
		CMP0026 # CMake 3.0: Disallow use of the LOCATION target property
		)
	if (POLICY ${p})
		cmake_policy(SET ${p} OLD)
	endif()
endforeach()

# Make it possible to locate CMake modules for finding libraries
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Use GNUInstallDirst to get canonical paths
include(GNUInstallDirs)

# Setup CMake to run tests
include (CTest)
if (BUILD_TESTING)
    # Setup CMake to provide a customized CTest -- see:
    # https://cmake.org/Wiki/CMake/Testing_With_CTest#Customizing_CTest
    enable_testing ()
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake"
        IMMEDIATE @ONLY)
    # Emulate GNU Autotools 'make check'
    if (NOT TARGET check)
        add_custom_target (check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
    endif (NOT TARGET check)
    # Combine the fancy 'make ARGS=--verbose all test'
    if (NOT TARGET build_and_test)
        add_custom_target (build_and_test COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
    endif (NOT TARGET build_and_test)
    add_subdirectory (test)
endif (BUILD_TESTING)

# Setup CMake to provide an uninstall target -- see:
# https://cmake.org/Wiki/CMake_FAQ#Can_I_do_.22make_uninstall.22_with_CMake.3F
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

if (NOT TARGET uninstall)
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif (NOT TARGET uninstall)

# This makes the project importable from the install directory
# Put config file in per-project dir (name MUST match), can also
# just go into <prefix>/cmake.
install(EXPORT ${PACKAGE_NAME}Config DESTINATION ${CMAKE_INSTALL_DATADIR}/${PACKAGE}/cmake)

# Exporting dependencies towards external packages
set (PKG_CONFIG_REQUIRES libusb-1.0)
set (PKG_CONFIG_LIBDIR
    "\${prefix}/lib"
)
set (PKG_CONFIG_INCLUDEDIR
    "\${prefix}/include/${PACKAGE}"
)
set (PKG_CONFIG_LIBS
    "-L\${libdir} -l${LIBUSBPP_LIB_NAME}"
)
set (PKG_CONFIG_CFLAGS
    "-I\${includedir}"
)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkg-config.pc.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE}.pc
)

# Install package configuration file
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE}.pc
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

## Add subdirectories
add_subdirectory (headers)
add_subdirectory (src)
add_subdirectory (examples)
