#include "stdafx.h"

#define BOOST_TEST_MODULE yadomsTests

#if defined (__GNUC__) && defined(__unix__)
#include <boost/test/included/unit_test.hpp>
#elif defined (WIN32)
#include <boost/test/unit_test.hpp>
#endif

