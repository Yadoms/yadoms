#include "stdafx.h"
#include "FakeDynamicallyConfigurableDevice.h"
#include <shared/Log.h>

CFakeDynamicallyConfigurableDevice::CFakeDynamicallyConfigurableDevice(const std::string& deviceName)
   : m_deviceName(deviceName),
     m_counter(boost::make_shared<yApi::historization::CCounter>("counter")),
     m_internalCounter(0),
     m_divider(1.00),
     m_historizers({m_counter})
{
   m_counter->set(0);
}

CFakeDynamicallyConfigurableDevice::~CFakeDynamicallyConfigurableDevice()
{
}

void CFakeDynamicallyConfigurableDevice::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Declare device and associated keywords (= values managed by this device)
   if (!api->deviceExists(m_deviceName))
      api->declareDevice(m_deviceName, getType(), getModel(), m_historizers);

   // Get the divider value from the device configuration
   try
   {
      YADOMS_LOG(information) << "Configuration = " << api->getDeviceConfiguration(m_deviceName).serialize();
      m_divider = api->getDeviceConfiguration(m_deviceName).get<int>("DynamicDivider");
   }
   catch (std::exception&)
   {
      // Configuration may not actually exist, set the default value to divider
      m_divider = 1.0;
   }
}

void CFakeDynamicallyConfigurableDevice::read()
{
   ++m_internalCounter;
   if (m_internalCounter / m_divider)
      m_counter->set(m_counter->get() + 1);
}

void CFakeDynamicallyConfigurableDevice::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   // If you need to historize several data, use the vector form of historizeData for performance
   api->historizeData(m_deviceName, m_historizers);
}

const std::string& CFakeDynamicallyConfigurableDevice::getDeviceName() const
{
   return m_deviceName;
}

const std::string& CFakeDynamicallyConfigurableDevice::getModel()
{
   static const std::string model("Fake dynamically configurable device");
   return model;
}

const std::string& CFakeDynamicallyConfigurableDevice::getType()
{
   static const std::string type("fakeDynamicallyConfigurableDeviceType");
   return type;
}

shared::CDataContainer CFakeDynamicallyConfigurableDevice::getDynamicConfiguration()
{
   //this code must be runtime dynamic.
   //in case of static configration, define the configuration schema in package.json
   shared::CDataContainer results;

   shared::CDataContainer options;
   options.set("type", "decimal");
   options.set("name", "Dynamic divider");
   options.set("minimumValue", "0.01");
   options.set("maximumValue", "20.0");
   options.set("precision", "2");

   results.set("DynamicDivider", options);

   return results;
}

void CFakeDynamicallyConfigurableDevice::setConfiguration(const shared::CDataContainer& newConfiguration)
{
   try
   {
      YADOMS_LOG(information) << "Configuration = " << newConfiguration.serialize();
      m_divider = newConfiguration.get<double>("DynamicDivider");
   }
   catch (std::exception&)
   {
      // Configuration may not actually exist, set the default value to divider
      m_divider = 1.0;
   }
}

