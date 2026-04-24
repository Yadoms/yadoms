#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/StringExtension.h"
#include <shared/Log.h>


bool checkExceptionValue(std::invalid_argument const& e)
{
	return std::string(e.what()) == std::string(R"(Key "first" not defined in "Replace {{first}} and {{second}} in my string")");
}

BOOST_AUTO_TEST_SUITE(TestStringExtension)


	BOOST_AUTO_TEST_CASE(ReplaceValues)
	{
		BOOST_CHECK_EQUAL(shared::CStringExtension::replaceValues("Replace {{first}} and {{second}} in my string",
			                  std::map<std::string, std::string>{ { "first", "value1" }, { "second", "value2" } }),
		                  "Replace value1 and value2 in my string");

		BOOST_CHECK_THROW(shared::CStringExtension::replaceValues("Replace {{first}} and {{second}} in my string",
			                  std::map<std::string, std::string>{ { "second", "value2" } }),
		                  std::invalid_argument);
		BOOST_CHECK_EXCEPTION(shared::CStringExtension::replaceValues("Replace {{first}} and {{second}} in my string",
			                      std::map<std::string, std::string>{ { "second", "value2" } }),
		                      std::invalid_argument,
		                      checkExceptionValue);

		BOOST_CHECK_EQUAL(shared::CStringExtension::replaceValues("Replace {{first}} and {{second}} in my string",
			                  std::map<std::string, std::string>{ { "first", "value1" }, { "second", "value2" }, { "third", "value3" } }),
		                  "Replace value1 and value2 in my string");

		BOOST_CHECK_EQUAL(shared::CStringExtension::replaceValues("Replace {{first}} and {{first}} in my string",
			                  std::map<std::string, std::string>{ { "first", "value1" }, { "second", "value2" } }),
		                  "Replace value1 and value1 in my string");
	}

BOOST_AUTO_TEST_SUITE_END()
