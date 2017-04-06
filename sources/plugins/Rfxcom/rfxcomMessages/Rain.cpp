#include "stdafx.h"
#include "Rain.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CRain::CRain(boost::shared_ptr<yApi::IYPluginApi> api,
                const RBUF& rbuf,
                size_t rbufSize)
      : m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
      m_rssi(boost::make_shared<yApi::historization::CRssi>("rssi")),
      m_keywords({ m_batteryLevel, m_rssi })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeRAIN,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(RAIN),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.RAIN.subtype;

      m_id = rbuf.RAIN.id1 | (rbuf.RAIN.id2 << 8);

      m_rain = boost::make_shared<yApi::historization::CRain>("rain", (m_subType == sTypeRAIN6) ? (yApi::historization::EMeasureType::kIncrement) : (yApi::historization::EMeasureType::kCumulative));
      m_keywords.push_back(m_rain);

      switch (m_subType)
      {
      case sTypeRAIN1:
      case sTypeRAIN2:
      case sTypeRAIN3:
      case sTypeRAIN4:
      case sTypeRAIN5:
      case sTypeRAIN7:
         m_rain->set(((rbuf.RAIN.raintotal1 << 16) | (rbuf.RAIN.raintotal2 << 8) | rbuf.RAIN.raintotal3) / 10.0);
         break;
      case sTypeRAIN6:
         m_rain->set(rbuf.RAIN.raintotal3 * 0.266);
         break;
      default:
         YADOMS_LOG(information) << "Rain subtype is not supported : " << m_subType ;
         break;
      }

      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.RAIN.battery_level));
      m_rssi->set(NormalizeRssiLevel(rbuf.RAIN.rssi));

      Init(api);
   }

   CRain::~CRain()
   {
   }

   void CRain::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeRAIN);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName, m_deviceModel, m_keywords, details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CRain::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Rain is a read-only message, can not be encoded");
   }

   void CRain::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CRain::getDeviceName() const
   {
      return m_deviceName;
   }

   void CRain::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CRain::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeRAIN1: ssModel << "RGR126/682/918";
         break;
      case sTypeRAIN2: ssModel << "PCR800";
         break;
      case sTypeRAIN3: ssModel << "TFA";
         break;
      case sTypeRAIN4: ssModel << "UPM";
         break;
      case sTypeRAIN5: ssModel << "La Crosse WS2300";
         break;
      case sTypeRAIN6: ssModel << "TX5";
         break;
      case sTypeRAIN7: ssModel << "Alecto WS4500, Auriol H13726, Hama EWS1500, Meteoscan W155/W160, Ventus WS155";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages


