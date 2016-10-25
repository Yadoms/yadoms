#include "stdafx.h"
#include "FakeConfigurableDevice.h"

CFakeConfigurableDevice::CFakeConfigurableDevice(const std::string& deviceName)
   : m_deviceName(deviceName),
     m_counter(boost::make_shared<yApi::historization::CCounter>("counter")),
     m_internalCounter(0),
     m_divider(0),
     m_historizers({m_counter})
{
   m_counter->set(0);
}

CFakeConfigurableDevice::~CFakeConfigurableDevice()
{
}

void CFakeConfigurableDevice::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   // Declare device and associated keywords (= values managed by this device)
   if (!api->deviceExists(m_deviceName))
      api->declareDevice(m_deviceName, getModel(), m_historizers);
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
   static const std::string model("Fake configurable device");
   return model;
}

boost::shared_ptr<shared::CDataContainer> CFakeConfigurableDevice::deviceConfigurationSchema(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api->getInformation()->getPackage()->containsChild("supportDeviceConfiguration/configurationSchema"))
      throw shared::exception::CException("supportDeviceConfiguration node not found in package.json");

   return boost::make_shared<shared::CDataContainer>(api->getInformation()->getPackage()->get<shared::CDataContainer>("supportDeviceConfiguration/configurationSchema"));
}

void CFakeConfigurableDevice::setConfiguration(const shared::CDataContainer& newConfiguration)
{
   m_divider = newConfiguration.get<int>("CounterDivider");
}
