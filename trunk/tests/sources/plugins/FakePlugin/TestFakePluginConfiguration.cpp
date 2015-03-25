#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/plugins/FakePlugin/FakePluginConfiguration.h"

BOOST_AUTO_TEST_SUITE(TestFakePluginConfiguration)


const std::string defaultConf("{"
   "\"EnumParameter\": \"EnumValue1\""
   "}");

BOOST_AUTO_TEST_CASE(ReadFakePluginDefaultConfiguration)
{
   CFakePluginConfiguration cfg;
   cfg.initializeWith(defaultConf);

   BOOST_CHECK_EQUAL(cfg.getEnumParameter(), static_cast<EEnumType>(kEnumValue1));
}

BOOST_AUTO_TEST_SUITE_END()