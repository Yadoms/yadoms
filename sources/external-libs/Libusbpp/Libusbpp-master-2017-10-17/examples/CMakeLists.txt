#
# LibusbTest/CMakeLists.txt
#

# Collect PIC example source files
set (appusbpppicsrc
    LibusbTest.cpp
)

# Join and sort all known example source files
set (appusbppsrc ${appusbpppicsrc})
list (SORT appusbppsrc)

# Check if all sources were catched
message (STATUS "Searching for uncatched example sources...")
file (GLOB allsrc RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp
)
foreach (_src ${allsrc})
    list (FIND appusbppsrc ${_src} matchidx)
    if (${matchidx} EQUAL -1)
        message (AUTHOR_WARNING "  ${_src}")
    endif ()
endforeach ()

add_executable (${PROJECT_NAME}-pic-example ${appusbpppicsrc})
target_link_libraries (${PROJECT_NAME}-pic-example sharedlibusbpp)

# Define headers for this example. PUBLIC headers are used for
# compiling the example, and will be added to consumers' build
# paths.
target_include_directories (${PROJECT_NAME}-pic-example
    PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../headers>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Install executable
install (TARGETS ${PROJECT_NAME}-pic-example EXPORT ${PACKAGE_NAME}Config
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows

# This makes the project importable from the build directory
export (TARGETS ${PROJECT_NAME}-pic-example FILE ${PACKAGE_NAME}Config.cmake)
