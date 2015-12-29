#include "stdafx.h"
#include "FakePluginConfiguration.h"
#include <shared/Log.h>


CFakePluginConfiguration::~CFakePluginConfiguration()
{
}

void CFakePluginConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_configuration.initializeWith(data);
}

EEnumType CFakePluginConfiguration::getEnumParameter() const
{
   // Enum type, declare keys labels
   static const shared::CDataContainer::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
      ("EnumValue1", kEnumValue1)
      ("EnumValue2", kEnumValue2)
      ("EnumValue3", kEnumValue3);

   return m_configuration.getEnumValue<EEnumType>("EnumParameter", EEnumTypeNames);
}

void CFakePluginConfiguration::trace()
{
   try
   {
      // Get simple parameters
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'StringParameter' is " << m_configuration.get<std::string>("StringParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is " << m_configuration.get<bool>("BoolParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'DecimalParameter' is " << m_configuration.get<double>("DecimalParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'IntParameter' is " << m_configuration.get<int>("IntParameter");

      // Enum
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'EnumParameter' is " << getEnumParameter();

      // Parameters in sections
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'MySection.SubIntParameter' is " << m_configuration.get<int>("MySection.content.SubIntParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'MySection.SubStringParameter' is " << m_configuration.get<std::string>("MySection.content.SubStringParameter");
   }
   catch (const shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Parameter not found : " << e.what();
   }
   catch (const shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Parameter value out of range : " << e.what();
   }
}
