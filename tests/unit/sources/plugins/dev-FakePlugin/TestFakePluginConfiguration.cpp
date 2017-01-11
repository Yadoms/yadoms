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
#ifdef _WIN32
   boost::filesystem::copy_file("../../../../sources/plugins/dev-FakePlugin/package.json", "package.json", boost::filesystem::copy_option::overwrite_if_exists);
#else 
   boost::filesystem::copy_file("../../../sources/plugins/dev-FakePlugin/package.json", "package.json", boost::filesystem::copy_option::overwrite_if_exists);
#endif

   CFakePluginConfiguration cfg;
   cfg.initializeWith(conf);

   BOOST_CHECK_EQUAL(cfg.getEnumParameter(), static_cast<EEnumType>(kEnumValue1));
}

BOOST_AUTO_TEST_SUITE_END()