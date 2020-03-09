#include "stdafx.h"
#include "FakeAnotherConfigurableDevice.h"
#include <shared/Log.h>

CFakeAnotherConfigurableDevice::CFakeAnotherConfigurableDevice(const std::string& deviceName,
                                                               const shared::CDataContainerSharedPtr& configuration)
   : m_deviceName(deviceName),
     m_counter(boost::make_shared<yApi::historization::CCounter>("counter")),
     m_internalCounter(0),
     m_divider(readDividerConfiguration(configuration)),
     m_historizers({m_counter})
{
   m_counter->set(0);
}

CFakeAnotherConfigurableDevice::~CFakeAnotherConfigurableDevice()
{
}

void CFakeAnotherConfigurableDevice::read()
{
   ++m_internalCounter;
   if (m_internalCounter % m_divider)
      m_counter->set(m_counter->get() + 1);
}

void CFakeAnotherConfigurableDevice::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   // If you need to historize several data, use the vector form of historizeData for performance
   api->historizeData(m_deviceName, m_historizers);
}

const std::string& CFakeAnotherConfigurableDevice::getDeviceName() const
{
   return m_deviceName;
}

const std::string& CFakeAnotherConfigurableDevice::getType()
{
   static const std::string type("anotherFakeConfigurableDeviceType");
   return type;
}

const std::string& CFakeAnotherConfigurableDevice::getModel()
{
   static const std::string model("Another Fake Configurable Device");
   return model;
}

void CFakeAnotherConfigurableDevice::setConfiguration(const shared::CDataContainerSharedPtr& newConfiguration)
{
   m_divider = readDividerConfiguration(newConfiguration);
}

std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> CFakeAnotherConfigurableDevice::historizers() const
{
   return m_historizers;
}

int CFakeAnotherConfigurableDevice::readDividerConfiguration(const shared::CDataContainerSharedPtr& configuration)
{
   try
   {
      YADOMS_LOG(information) << "Configuration = " << configuration->serialize();
      if (configuration->containsValue("CounterDivider2"))
         return configuration->get<int>("CounterDivider2");
   }
   catch (std::exception&)
   {
      // Configuration may not actually exist
   }
   // Return default value
   return 1;
}

