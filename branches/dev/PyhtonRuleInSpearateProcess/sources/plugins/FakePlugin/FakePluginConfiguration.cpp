#include "stdafx.h"
#include "FakePluginConfiguration.h"
#include <shared/Log.h>


CFakePluginConfiguration::~CFakePluginConfiguration()
{
}

EEnumType CFakePluginConfiguration::getEnumParameter() const
{
   // Enum type, declare keys labels
   static const EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
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
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'StringParameter' is " << get<std::string>("StringParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is " << get<bool>("BoolParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'DecimalParameter' is " << get<double>("DecimalParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'IntParameter' is " << get<int>("IntParameter");

      // Enum
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'EnumParameter' is " << getEnumParameter();

      // Parameters in sections
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'MySection.SubIntParameter' is " << get<int>("MySection.content.SubIntParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'MySection.SubStringParameter' is " << get<std::string>("MySection.content.SubStringParameter");
   }
   catch (const shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Parameter not found : " << e.what();
      BOOST_ASSERT(false);  // Parameter is not found in configuration
   }
   catch (const shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Parameter value out of range : " << e.what();
      BOOST_ASSERT(false);  // Parameter value is out of range
   }
}
