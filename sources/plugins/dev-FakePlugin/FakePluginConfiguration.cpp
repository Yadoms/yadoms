#include "stdafx.h"
#include "FakePluginConfiguration.h"
#include <shared/Log.h>


void CFakePluginConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer> & data)
{
   m_configuration.initializeWith(data);
}

void CFakePluginConfiguration::initializeWith(boost::shared_ptr<shared::CDataContainer> data,
                                              const boost::filesystem::path& packageJsonPath)
{
   m_configuration.initializeWith(data, packageJsonPath);
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
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'StringParameter' is "
         << (m_configuration.get<std::string>("StringParameter").empty() ? "empty" : m_configuration.get<std::string>("StringParameter"));
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'BoolParameter' is " << m_configuration.get<bool>("BoolParameter");
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'DecimalParameter' is " << m_configuration.get<double>("DecimalParameter");
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'IntParameter' is " << m_configuration.get<int>("IntParameter");

      // Enum
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'EnumParameter' is " << getEnumParameter();

      // Parameters in sections
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'MySection.SubIntParameter' is " << m_configuration.get<int>(
            "MySection.content.SubIntParameter");
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'MySection.SubStringParameter' is "
         << (m_configuration.get<std::string>("MySection.content.SubStringParameter").empty()
                ? "empty"
                : m_configuration.get<std::string>("MySection.content.SubStringParameter"));

      // SystemData sections
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'SystemData.SerialPort' is " << m_configuration.get<std::string>(
         "SystemData.content.SerialPort");
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'SystemData.UsbDevices' is " << m_configuration.get<std::string>(
         "SystemData.content.UsbDevices");
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'SystemData.NetworkInterfaces' is " << m_configuration.get<std::string>(
         "SystemData.content.NetworkInterfaces");
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'SystemData.NetworkInterfacesWithoutLoopback' is " << m_configuration.get<std::string>(
         "SystemData.content.NetworkInterfacesWithoutLoopback");
      YADOMS_LOG(information) << "CFakePluginConfiguration::trace, parameter 'SystemData.SupportedTimezones' is " << m_configuration.get<std::string>(
         "SystemData.content.SupportedTimezones");
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
