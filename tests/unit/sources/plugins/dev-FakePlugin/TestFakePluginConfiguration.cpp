#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/dev-FakePlugin/FakePluginConfiguration.h"

BOOST_AUTO_TEST_SUITE(TestFakePluginConfiguration)


const shared::CDataContainer conf("{"
   "\"EnumParameter\": \"EnumValue1\""
   "}");

BOOST_AUTO_TEST_CASE(ReadFakePluginDefaultConfiguration)
{
   // Need to copy the package.json file from fakePlugin, needed by initializeWith method to get default configuration
	if (boost::filesystem::exists("../../../../sources/plugins/dev-FakePlugin/package.in.json"))
		boost::filesystem::copy_file("../../../../sources/plugins/dev-FakePlugin/package.in.json", "package.json", boost::filesystem::copy_option::overwrite_if_exists);
	else if (boost::filesystem::exists("../../../sources/plugins/dev-FakePlugin/package.in.json"))
		boost::filesystem::copy_file("../../../sources/plugins/dev-FakePlugin/package.in.json", "package.json", boost::filesystem::copy_option::overwrite_if_exists);
	else
		BOOST_ERROR("Fail to copy fakePlugin package.in.json to package.json");

   CFakePluginConfiguration cfg;
   cfg.initializeWith(conf);

   BOOST_CHECK_EQUAL(cfg.getEnumParameter(), static_cast<EEnumType>(kEnumValue1));
}

BOOST_AUTO_TEST_SUITE_END()