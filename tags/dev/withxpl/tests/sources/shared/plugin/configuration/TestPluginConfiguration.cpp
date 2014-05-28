#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/plugin/configuration/Configuration.h"

BOOST_AUTO_TEST_SUITE(TestPluginConfiguration)

enum EEnumType
{
   kEnumValue1 = 7,
   kEnumValue2 = 12,
   kEnumValue3
};

static const shared::plugin::configuration::CConfiguration::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
   ("EnumValue1", kEnumValue1)
   ("EnumValue2", kEnumValue2)
   ("EnumValue3", kEnumValue3);

const std::string defaultConf("{"
   "\"BoolParameter\": \"true\","
   "\"DecimalParameter\": \"18.4\","
   "\"EnumParameter\": \"EnumValue1\","
   "\"IntParameter\": \"42\","
   "\"Serial port\": \"tty0\","
   "\"StringParameter\": \"Yadoms is so powerful !\","
   "\"MySection\": {"
   "\"SubIntParameter\": \"123\","
   "\"SubStringParameter\": \"Just a string parameter in the sub-section\""
   "}"
   "}");


BOOST_AUTO_TEST_CASE(ReadDefaultConfiguration)
{
   shared::plugin::configuration::CConfiguration cfg;
   cfg.set(defaultConf);

   BOOST_CHECK_EQUAL(cfg.getValue<bool>("BoolParameter"), true);
   BOOST_CHECK_EQUAL(cfg.getValue<double>("DecimalParameter"), 18.4);
   BOOST_CHECK_EQUAL(cfg.getEnumValue<EEnumType>("EnumParameter", EEnumTypeNames), kEnumValue1);
   BOOST_CHECK_EQUAL(cfg.getValue<int>("IntParameter"), 42);
   BOOST_CHECK_EQUAL(cfg.getValue<std::string>("Serial port"), "tty0");
   BOOST_CHECK_EQUAL(cfg.getValue<std::string>("StringParameter"), "Yadoms is so powerful !");
   BOOST_CHECK_EQUAL(cfg.getValue<int>("MySection.SubIntParameter"), 123);
   BOOST_CHECK_EQUAL(cfg.getValue<std::string>("MySection.SubStringParameter"), "Just a string parameter in the sub-section");
}

BOOST_AUTO_TEST_SUITE_END()