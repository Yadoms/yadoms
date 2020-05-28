#include "stdafx.h"
#include "Weather.h"
#include "Weather1.h"
#include "Weather2.h"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CWeather::CWeather(boost::shared_ptr<yApi::IYPluginApi> api,
      const RBUF& rbuf,
      size_t rbufSize)
      : m_deviceDetails(shared::CDataContainer::make()),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("batteryLevel")),
        m_keywords({m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeWEATHER,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(WEATHER),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.WEATHER.subtype;
      m_id = rbuf.WEATHER.id2 << 8 | rbuf.WEATHER.id1;
      m_subTypeManager->setFromProtocolState(rbuf.WEATHER.directionhigh << 8 | rbuf.WEATHER.directionlow,
                                             rbuf.WEATHER.av_speedhigh << 8 | rbuf.WEATHER.av_speedlow,
                                             rbuf.WEATHER.gusthigh << 8 | rbuf.WEATHER.gustlow,
                                             (rbuf.WEATHER.temperaturesign == 1 ? -1 : 1) * (rbuf.WEATHER.temperaturehigh << 8 | rbuf
                                                                                                                                 .WEATHER.
                                                                                                                                 temperaturelow),
                                             (rbuf.WEATHER.chillsign == 1 ? -1 : 1) * (rbuf.WEATHER.chillhigh << 8 | rbuf.WEATHER.chilllow),
                                             rbuf.WEATHER.humidity,
                                             rbuf.WEATHER.rainratehigh << 8 | rbuf.WEATHER.rainratelow,
                                             rbuf.WEATHER.raintotal3 << 16 | rbuf.WEATHER.raintotal2 << 8 | rbuf.WEATHER.raintotal3,
                                             rbuf.WEATHER.uv,
                                             rbuf.WEATHER.solarhigh << 8 | rbuf.WEATHER.solarlow,
                                             rbuf.WEATHER.barohigh << 8 | rbuf.WEATHER.barolow,
                                             static_cast<IWeatherSubtype::EWeatherForecast>(rbuf.WEATHER.forecast));
      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.WEATHER.battery_level));
      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.WEATHER.rssi));

      init(api);
   }

   void CWeather::buildDeviceDetails()
   {
      if (m_deviceDetails->empty())
      {
         m_deviceDetails->set("type", pTypeWEATHER);
         m_deviceDetails->set("subType", m_subType);
         m_deviceDetails->set("id", m_id);
      }
   }

   void CWeather::init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      switch (m_subType)
      {
      case sTypeWEATHER1:
         m_subTypeManager = boost::make_shared<CWeather1>();
         break;
      case sTypeWEATHER2:
         m_subTypeManager = boost::make_shared<CWeather2>();
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      m_keywords.insert(m_keywords.end(), m_subTypeManager->keywords().begin(), m_subTypeManager->keywords().end());

      // Build device description
      buildDeviceName();
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CWeather::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Weather is a read-only message, can not be encoded");
   }

   void CWeather::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CWeather::filter() const
   {
   }

   void CWeather::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      const auto model = m_subTypeManager->getModel();
      api->declareDevice(m_deviceName, model, model, m_keywords, m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << model << ")";
      m_deviceDetails->printToLog(YADOMS_LOG(information));
   }

   const std::string& CWeather::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CWeather::keywords()
   {
      return m_keywords;
   }

   void CWeather::buildDeviceName()
   {
      std::ostringstream deviceName;
      deviceName << static_cast<unsigned int>(m_subType) << "." << m_id;
      m_deviceName = deviceName.str();
   }
} // namespace rfxcomMessages
