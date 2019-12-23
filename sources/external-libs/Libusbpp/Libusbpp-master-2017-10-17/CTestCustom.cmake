#
# CTestCustom.cmake
#
# CTest can be customized by providing CTestCustom.cmake -- see:
# https://cmake.org/Wiki/CMake/Testing_With_CTest#Customizing_CTest
#

# disable unit test cases that will fail on non-WIN32 systems
if (NOT WIN32)
    set (
        CTEST_CUSTOM_TESTS_IGNORE ${CTEST_CUSTOM_TESTS_IGNORE}
        # OFF: TestSuit
        # OFF: TestSuit/TestCase
    )
endif (NOT WIN32)

# Check if all tests will run
message ("[****] Searching for ignored test cases...")
foreach (_ct_ignored ${CTEST_CUSTOM_TESTS_IGNORE})
    message (AUTHOR_WARNING "**** IGNORED: ${_ct_ignored}")
endforeach ()
