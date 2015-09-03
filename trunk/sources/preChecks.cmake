# Check for minimum versions
#

# Check for minimum GCC version
MESSAGE(STATUS "Compiler = ${CMAKE_CXX_COMPILER_ID}")
MESSAGE(STATUS "Compiler Version = ${CMAKE_CXX_COMPILER_VERSION}")

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    # require at least gcc 4.5 (for boost::log)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.5)
        message(FATAL_ERROR "GCC version must be at least 4.5!")
    endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    # require at least clang 3.2
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.2)
        message(FATAL_ERROR "Clang version must be at least 3.2!")
    endif()
endif()