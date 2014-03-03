#include "stdafx.h"
#include "FakePluginConfiguration.h"
#include <shared/Log.h>

namespace cfg = shared::plugin::configuration;


CFakePluginConfiguration::~CFakePluginConfiguration()
{
}

std::string CFakePluginConfiguration::getSerialPort() const
{
   return getValue<std::string>("Serial port");
}

EEnumType CFakePluginConfiguration::getEnumParameter() const
{
   // Enum type, declare keys labels
   static const CConfiguration::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
      ("EnumValue1", kEnumValue1)
      ("EnumValue2", kEnumValue2)
      ("EnumValue3", kEnumValue3);

   return getEnumValue<EEnumType>("EnumParameter", EEnumTypeNames);
}

void CFakePluginConfiguration::trace()
{
   try
   {
      // Get simple parameters
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'Serial port' is " << getSerialPort();
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'StringParameter' is " << getValue<std::string>("StringParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is " << getValue<bool>("BoolParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'DecimalParameter' is " << getValue<double>("DecimalParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'IntParameter' is " << getValue<int>("IntParameter");

      // Enum
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'EnumParameter' is " << getEnumParameter();

      // Parameters in sections
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'MySection.SubIntParameter' is " << getValue<int>("MySection.SubIntParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'MySection.SubStringParameter' is " << getValue<std::string>("MySection.SubStringParameter");
   }
   catch (const shared::exception::CInvalidParameter& e)
   {
      BOOST_ASSERT(false);  // Parameter is not found in configuration
      YADOMS_LOG(error) << "Parameter not found : " << e.what();
   }
   catch (const shared::exception::COutOfRange& e)
   {
      BOOST_ASSERT(false);  // Parameter value is out of range
      YADOMS_LOG(error) << "Parameter value out of range : " << e.what();
   }
}
