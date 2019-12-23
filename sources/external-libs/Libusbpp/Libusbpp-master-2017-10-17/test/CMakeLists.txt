#
# test/CMakeLists.txt
#

# Setup CMake to run tests
enable_testing()

# Require Boost
find_package (Boost REQUIRED COMPONENTS system filesystem
                                        unit_test_framework)
if (Boost_FOUND)
    message (STATUS "Found Boost in ${Boost_LIBRARY_DIRS}")
else (Boost_FOUND)
    message (FATAL_ERROR "Boost was not found.")
endif (Boost_FOUND)

# Preappend the local include search path
include_directories (${CMAKE_CURRENT_SOURCE_DIR})

# Prep ourselves for compiling boost
include_directories (${Boost_INCLUDE_DIRS})
add_definitions (-DBOOST_TEST_DYN_LINK) 

# Collect all test source files
set (testusbppsrc
    main.cpp
)

# Check if all sources were catched
message (STATUS "Searching for uncatched test sources...")
file (GLOB allsrc RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.hpp
    *.cpp
)
foreach (_src ${allsrc})
    list (FIND testusbppsrc ${_src} matchidx)
    if (${matchidx} EQUAL -1)
        message (AUTHOR_WARNING "  ${_src}")
    endif ()
endforeach ()

# Create the test suite
add_executable (testusbpp ${testusbppsrc})
target_link_libraries (testusbpp sharedlibusbpp
                                 ${Boost_SYSTEM_LIBRARY}
                                 ${Boost_FILESYSTEM_LIBRARY}
                                 ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

# Provide the test suite by ctest
add_dependencies (check testusbpp)
add_dependencies (build_and_test testusbpp)
# OFF: add_test (NAME TestSuit
# OFF:           COMMAND testusbpp -i -l test_suite -t TestSuit)
# OFF: add_test (NAME TestSuit/TestCase
# OFF:           COMMAND testusbpp -i -l test_suite -t TestSuit/TestCase)
