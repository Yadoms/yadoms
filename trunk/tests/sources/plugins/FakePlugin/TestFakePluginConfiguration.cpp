#include "stdafx.h"
// Includes needed to compile tested classes
#include "../../../../sources/plugins/FakePlugin/FakePluginConfiguration.h"

#define BOOST_TEST_MODULE TestFakePluginConfiguration
#include <boost/test/unit_test.hpp>

const std::string defaultConf("{"
   "\"EnumParameter\": \"EnumValue1\","
   "\"Serial port\": \"tty0\""
   "}");

BOOST_AUTO_TEST_CASE(ReadFakePluginDefaultConfiguration)
{
   CFakePluginConfiguration cfg;
   cfg.set(defaultConf);

   BOOST_CHECK_EQUAL(cfg.getSerialPort(), "tty0");
   BOOST_CHECK_EQUAL(cfg.getEnumParameter(), (EEnumType)kEnumValue1);
}

