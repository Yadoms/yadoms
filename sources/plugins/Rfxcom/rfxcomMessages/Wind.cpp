#include "stdafx.h"
#include "Wind.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CWind::CWind(boost::shared_ptr<yApi::IYPluginApi> api,
                const RBUF& rbuf,
                size_t rbufSize)
      : m_windDirection(boost::make_shared<yApi::historization::CDirection>("windDirection")),
      m_windMaxSpeed(boost::make_shared<yApi::historization::CSpeed>("windMaxSpeed")),
      m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_windDirection , m_windMaxSpeed , m_batteryLevel , m_signalPower })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeWIND,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(WIND),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.WIND.subtype;

      m_id = rbuf.WIND.id1 | (rbuf.WIND.id2 << 8);

      m_windDirection->set(rbuf.WIND.directionl | (rbuf.WIND.directionh << 8));

      if (m_subType != sTypeWIND5)
      {
         m_windAverageSpeed = boost::make_shared<yApi::historization::CSpeed>("windAverageSpeed");
         m_keywords.push_back(m_windAverageSpeed);

         m_windAverageSpeed->set((rbuf.WIND.av_speedl | (rbuf.WIND.av_speedh << 8)) / 10.0);
      }

      m_windMaxSpeed->set((rbuf.WIND.gustl | (rbuf.WIND.gusth << 8)) / 10.0);

      if (m_subType == sTypeWIND4)
      {
         m_temperature = boost::make_shared<yApi::historization::CTemperature>("temperature");
         m_temperature->set(NormalizeTemperature(rbuf.WIND.temperatureh, rbuf.WIND.temperaturel, rbuf.WIND.tempsign == 1));
         m_keywords.push_back(m_temperature);

         m_chillTemperature = boost::make_shared<yApi::historization::CTemperature>("chillTemperature");
         m_chillTemperature->set(NormalizeTemperature(rbuf.WIND.chillh, rbuf.WIND.chilll, rbuf.WIND.chillsign == 1));
         m_keywords.push_back(m_chillTemperature);
      }

      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.WIND.battery_level));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.WIND.signalPower));

      Init(api);
   }

   CWind::~CWind()
   {
   }

   void CWind::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeWIND);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CWind::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Wind is a read-only message, can not be encoded");
   }

   void CWind::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CWind::getDeviceName() const
   {
      return m_deviceName;
   }

   void CWind::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CWind::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeWIND1: ssModel << "WTGR800";
         break;
      case sTypeWIND2: ssModel << "WGR800";
         break;
      case sTypeWIND3: ssModel << "STR918,WGR918";
         break;
      case sTypeWIND4: ssModel << "TFA";
         break;
      case sTypeWIND5: ssModel << "UPM";
         break;
      case sTypeWIND6: ssModel << "La Crosse WS2300";
         break;
      case sTypeWIND7: ssModel << "Alecto WS4500, Auriol H13726, Hama EWS1500, Meteoscan W155/W160, Ventus WS155";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages


