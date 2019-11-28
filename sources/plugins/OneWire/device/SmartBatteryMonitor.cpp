#include "stdafx.h"
#include "SmartBatteryMonitor.h"
#include "Identification.h"
#include "OneWireException.hpp"
#include <shared/Log.h>

namespace device
{
const boost::posix_time::time_duration CSmartBatteryMonitor::HistorizationPeriod(boost::posix_time::minutes(1));

CSmartBatteryMonitor::CSmartBatteryMonitor(EOneWireFamily family,
                                           const std::string &id,
                                           boost::shared_ptr<ioInterfaces::ISmartBatteryMonitor> io)
    : m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2438")),
      m_io(io)
{
   BOOST_ASSERT_MSG(m_identification->family() == kSmartBatteryMonitor, "Invalid family number");

   creatDefaultKeywordList();
}

void CSmartBatteryMonitor::creatDefaultKeywordList()
{
   // Default keyword list, without configuration
   m_kwTemperature = boost::make_shared<yApi::historization::CTemperature>("temperature");
   m_keywords.push_back(m_kwTemperature);

   m_kwVdd = boost::make_shared<yApi::historization::CVoltage>("vdd");
   m_keywords.push_back(m_kwVdd);
}

void CSmartBatteryMonitor::setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                            const shared::CDataContainer &configuration)
{
   m_keywords.clear();
   creatDefaultKeywordList();

   m_configuration = boost::make_shared<const CSmartBatteryMonitorConfiguration>(configuration);

   YADOMS_LOG(information) << "Configuration for device " << ident()->deviceName() << " :";
   YADOMS_LOG(information) << "  - vadSensor = " << m_configuration->vadSensor();
   YADOMS_LOG(information) << "  - visInput = " << m_configuration->visInput();

   switch (m_configuration->vadSensor())
   {
   case CSmartBatteryMonitorConfiguration::kNone:
      break;
   case CSmartBatteryMonitorConfiguration::kHumidity:
   case CSmartBatteryMonitorConfiguration::kHIH3600:
   case CSmartBatteryMonitorConfiguration::kHIH4000:
   case CSmartBatteryMonitorConfiguration::kHTM1735:
      m_kwHumidity = boost::make_shared<yApi::historization::CHumidity>("humidity");
      m_keywords.push_back(m_kwHumidity);
      break;
   case CSmartBatteryMonitorConfiguration::kS3_R1_A:
      m_kwLight = boost::make_shared<yApi::historization::CIllumination>("light");
      m_keywords.push_back(m_kwLight);
      break;
   case CSmartBatteryMonitorConfiguration::kB1_R1_A:
      m_kwPressure = boost::make_shared<yApi::historization::CPressure>("pressure");
      m_keywords.push_back(m_kwPressure);
      break;
   case CSmartBatteryMonitorConfiguration::kRaw:
      m_kwVad = boost::make_shared<yApi::historization::CVoltage>("vad");
      m_keywords.push_back(m_kwVad);
      break;
   default:
      YADOMS_LOG(error) << "Invalid configuration of DS2438 VAD input " << m_configuration->vadSensor();
      break;
   }

   if (m_configuration->visInput())
   {
      m_kwVis = boost::make_shared<yApi::historization::CVoltage>("vis");
      m_keywords.push_back(m_kwVis);
   }

   removeNoMoreUsedKeywords(api,
                            ident()->deviceName(),
                            m_keywords);

   api->declareKeywords(ident()->deviceName(),
                        m_keywords);
}

void CSmartBatteryMonitor::removeNoMoreUsedKeywords(boost::shared_ptr<yApi::IYPluginApi> api,
                                                    const std::string &device,
                                                    const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> &newKeywords)
{
   // Get existing keywords
   for (const auto &keyword : api->getAllKeywords(device))
   {
      bool keywordExist = false;
      for (const auto &newKeyword : newKeywords)
      {
         if (keyword == newKeyword->getKeyword())
         {
            keywordExist = true;
            break;
         }
      }
      if (!keywordExist)
      {
         YADOMS_LOG(information) << "Remove keyword " << keyword;
         api->removeKeyword(device,
                            keyword);
      }
   }
}

void CSmartBatteryMonitor::read() const
{
   m_kwTemperature->set(m_io->readTemperature());
   m_kwVdd->set(m_io->readVdd());

   try
   {
      auto vadSensor = m_configuration ? m_configuration->vadSensor() : CSmartBatteryMonitorConfiguration::kNone;
      switch (vadSensor)
      {
      case CSmartBatteryMonitorConfiguration::kNone:
         break;
      case CSmartBatteryMonitorConfiguration::kHumidity:
         if (!!m_kwHumidity)
            m_kwHumidity->set(m_io->readHumidity());
         break;
      case CSmartBatteryMonitorConfiguration::kHIH3600:
         if (!!m_kwHumidity)
            m_kwHumidity->set(m_io->readHIH3600());
         break;
      case CSmartBatteryMonitorConfiguration::kHIH4000:
         if (!!m_kwHumidity)
            m_kwHumidity->set(m_io->readHIH4000());
         break;
      case CSmartBatteryMonitorConfiguration::kHTM1735:
         if (!!m_kwHumidity)
            m_kwHumidity->set(m_io->readHTM1735());
         break;
      case CSmartBatteryMonitorConfiguration::kS3_R1_A:
         if (!!m_kwLight)
            m_kwLight->set(m_io->readS3_R1_A());
         break;
      case CSmartBatteryMonitorConfiguration::kB1_R1_A:
         if (!!m_kwPressure)
            m_kwPressure->set(m_io->readB1_R1_A());
         break;
      case CSmartBatteryMonitorConfiguration::kRaw:
         if (!!m_kwVad)
            m_kwVad->set(m_io->readVad());
         break;
      default:
         YADOMS_LOG(error) << "Invalid configuration of DS2438 VAD input " << m_configuration->vadSensor();
         break;
      }
   }
   catch (COneWireException &e)
   {
      YADOMS_LOG(warning) << "Device " << ident()->deviceName() << " : unable to read VAD sensor";
   }

   try
   {
      if (!!m_kwVis)
         m_kwVis->set(m_io->readVis());
   }
   catch (COneWireException &e)
   {
      YADOMS_LOG(warning) << "Device " << ident()->deviceName() << " : unable to read vis value";
   }
}

void CSmartBatteryMonitor::write(const std::string &keyword, const std::string &command)
{
   YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
}

bool CSmartBatteryMonitor::hasRelevantValue()
{
   if (m_keywords->empty())
      return false;

   const auto now = shared::currentTime::Provider().now();

   if (now < (m_lastHistorizationDate + HistorizationPeriod))
      return false;

   m_lastHistorizationDate = now;
   return true;
}
} // namespace device
