#include "stdafx.h"
#include "FakePluginConfiguration.h"


CFakePluginConfiguration::~CFakePluginConfiguration()
{
}

void CFakePluginConfiguration::initializeWith(const shared::CDataContainer& data)
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

void CFakePluginConfiguration::trace() const
{
   try
   {
      // Get simple parameters
      std::cout << "CFakePlugin::doWork, parameter 'StringParameter' is "
         << (m_configuration.get<std::string>("StringParameter").empty() ? "empty" : m_configuration.get<std::string>("StringParameter"))
         << std::endl;
      std::cout << "CFakePlugin::doWork, parameter 'BoolParameter' is " << m_configuration.get<bool>("BoolParameter") << std::endl;
      std::cout << "CFakePlugin::doWork, parameter 'DecimalParameter' is " << m_configuration.get<double>("DecimalParameter") << std::endl;
      std::cout << "CFakePlugin::doWork, parameter 'IntParameter' is " << m_configuration.get<int>("IntParameter") << std::endl;

      // Enum
      std::cout << "CFakePlugin::doWork, parameter 'EnumParameter' is " << getEnumParameter() << std::endl;

      // Parameters in sections
      std::cout << "CFakePlugin::doWork, parameter 'MySection.SubIntParameter' is " << m_configuration.get<int>("MySection.content.SubIntParameter") << std::endl;
      std::cout << "CFakePlugin::doWork, parameter 'MySection.SubStringParameter' is "
         << (m_configuration.get<std::string>("MySection.content.SubStringParameter").empty() ? "empty" : m_configuration.get<std::string>("MySection.content.SubStringParameter"))
         << std::endl;
   }
   catch (const shared::exception::CInvalidParameter& e)
   {
      std::cerr << "Parameter not found : " << e.what() << std::endl;
   }
   catch (const shared::exception::COutOfRange& e)
   {
      std::cerr << "Parameter value out of range : " << e.what() << std::endl;
   }
}

