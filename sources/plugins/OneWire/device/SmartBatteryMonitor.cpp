#include "stdafx.h"
#include "SmartBatteryMonitor.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device
{
   CSmartBatteryMonitor::CSmartBatteryMonitor(EOneWireFamily family,
                                              const std::string& id,
                                              boost::shared_ptr<ioInterfaces::ISmartBatteryMonitor> io)
      :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2438")),
      m_io(io)
   {
      BOOST_ASSERT_MSG(m_identification->family() == kSmartBatteryMonitor, "Invalid family number");

      m_kwTemperature = boost::make_shared<yApi::historization::CTemperature>("temperature");
      m_keywords.push_back(m_kwTemperature);

      switch (m_io->readSensorType())
      {
      case ioInterfaces::ISmartBatteryMonitor::kMultisensorTemperature: break; // Nothing to do, all sensors have temperature (sensor is inside DS2438), temperature is already added
      case ioInterfaces::ISmartBatteryMonitor::kMultisensorTemperatureHumidity:
         m_kwHumidity = boost::make_shared<yApi::historization::CHumidity>("humidity");
         m_keywords.push_back(m_kwHumidity);
         break;
      case ioInterfaces::ISmartBatteryMonitor::kMultisensorTemperatureLight:
         m_kwLight = boost::make_shared<yApi::historization::CIllumination>("light");
         m_keywords.push_back(m_kwLight);
         break;
      default: // Unable to determine sensor connected to DS2438, so add all values
         m_kwHumidity = boost::make_shared<yApi::historization::CHumidity>("humidity");
         m_keywords.push_back(m_kwHumidity);
         m_kwPressure = boost::make_shared<yApi::historization::CPressure>("pressure");
         m_keywords.push_back(m_kwPressure);
         m_kwLight = boost::make_shared<yApi::historization::CIllumination>("light");
         m_keywords.push_back(m_kwLight);
         m_kwVad = boost::make_shared<yApi::historization::CVoltage>("vad");
         m_keywords.push_back(m_kwVad);
         m_kwVdd = boost::make_shared<yApi::historization::CVoltage>("vdd");
         m_keywords.push_back(m_kwVdd);
         m_kwVis = boost::make_shared<yApi::historization::CVoltage>("vis");
         m_keywords.push_back(m_kwVis);
         break;
      }
   }

   CSmartBatteryMonitor::~CSmartBatteryMonitor()
   {
   }

   void CSmartBatteryMonitor::setConfiguration(const shared::CDataContainer& configuration)
   {
      m_configuration = boost::make_shared<const CSmartBatteryMonitorConfiguration>(configuration);
      // TODO
      YADOMS_LOG(information) << "Configuration was changed for device " << ident()->deviceName();
      YADOMS_LOG(information) << "humidity = " << m_configuration->historizeHumidity();
      YADOMS_LOG(information) << "light = " << m_configuration->historizeLight();
   }

   void CSmartBatteryMonitor::read() const
   {
      m_kwTemperature->set(m_io->readTemperature());
      if (!!m_kwHumidity)
         m_kwHumidity->set(m_io->readHumidity());
      if (!!m_kwPressure)
         m_kwPressure->set(m_io->readPressure());
      if (!!m_kwLight)
         m_kwLight->set(m_io->readLight());
      if (!!m_kwVad)
         m_kwVad->set(m_io->readVad());
      if (!!m_kwVdd)
         m_kwVdd->set(m_io->readVdd());
      if (!!m_kwVis)
         m_kwVis->set(m_io->readVis());
   }

   void CSmartBatteryMonitor::write(const std::string& keyword, const std::string& command)
   {
      YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
   }
} // namespace device
