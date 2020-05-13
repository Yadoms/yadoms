#include "stdafx.h"
#include "FakeConfigurableDevice.h"
#include <shared/Log.h>

CFakeConfigurableDevice::CFakeConfigurableDevice(const std::string& deviceName)
   : m_deviceName(deviceName),
     m_counter(boost::make_shared<yApi::historization::CCounter>("counter")),
     m_internalCounter(0),
     m_divider(1),
     m_historizers({m_counter})
{
   m_counter->set(0);
}

CFakeConfigurableDevice::~CFakeConfigurableDevice()
{
}

void CFakeConfigurableDevice::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Declare device and associated keywords (= values managed by this device)
   if (!api->deviceExists(m_deviceName))
      api->declareDevice(m_deviceName, getType(), getModel(), m_historizers);

   // Get the divider value from the device configuration
   try
   {
      YADOMS_LOG(information) << "Configuration = " << api->getDeviceConfiguration(m_deviceName)->serialize();
      m_divider = api->getDeviceConfiguration(m_deviceName)->get<int>("CounterDivider2");
   }
   catch (std::exception&)
   {
      // Configuration may not actually exist, set the default value to divider
      m_divider = 1;
   }
}

void CFakeConfigurableDevice::read()
{
   ++m_internalCounter;
   if (m_internalCounter % m_divider)
      m_counter->set(m_counter->get() + 1);
}

void CFakeConfigurableDevice::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   // If you need to historize several data, use the vector form of historizeData for performance
   api->historizeData(m_deviceName, m_historizers);
}

const std::string& CFakeConfigurableDevice::getDeviceName() const
{
   return m_deviceName;
}

const std::string& CFakeConfigurableDevice::getModel()
{
   static const std::string model("Fake Configurable Device");
   return model;
}

const std::string& CFakeConfigurableDevice::getType()
{
   static const std::string type("fakeConfigurableDeviceType");
   return type;
}

void CFakeConfigurableDevice::setConfiguration(const shared::CDataContainerSharedPtr& newConfiguration)
{
   YADOMS_LOG(information) << "Configuration = " << newConfiguration->serialize();
   if(newConfiguration->containsValue("CounterDivider2"))
      m_divider = newConfiguration->get<int>("CounterDivider2");
}

