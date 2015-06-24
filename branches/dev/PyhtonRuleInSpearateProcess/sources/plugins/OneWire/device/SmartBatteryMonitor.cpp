#include "stdafx.h"
#include "SmartBatteryMonitor.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device {

CSmartBatteryMonitor::CSmartBatteryMonitor(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::ISmartBatteryMonitor> io)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2438")), m_context(context), m_io(io)
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

boost::shared_ptr<const IIdentification> CSmartBatteryMonitor::ident() const
{
   return m_identification;
}

void CSmartBatteryMonitor::declare()
{
   m_identification->declare(m_context);

   for (std::vector<boost::shared_ptr<yApi::historization::IHistorizable> >::const_iterator keyword = m_keywords.begin(); keyword != m_keywords.end(); ++keyword)
      if (!m_context->keywordExists(m_identification->deviceName(), **keyword))
         m_context->declareKeyword(m_identification->deviceName(), **keyword);
}

void CSmartBatteryMonitor::historize()
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

   m_context->historizeData(ident()->deviceName(), m_keywords);
}

void CSmartBatteryMonitor::set(const std::string& keyword, const std::string& command)
{
   YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
}

} // namespace device