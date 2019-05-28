#include "stdafx.h"
#include "Humidity.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CHumidity::CHumidity(boost::shared_ptr<yApi::IYPluginApi> api,
                        const RBUF& rbuf,
                        size_t rbufSize)
      : m_humidity(boost::make_shared<yApi::historization::CHumidity>("humidity")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_humidity , m_batteryLevel , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeHUM,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(HUM),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.HUM.subtype;

      m_id = rbuf.HUM.id1 | (rbuf.HUM.id2 << 8);

      m_humidity->set(rbuf.HUM.humidity);

      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.HUM.battery_level));
      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.HUM.rssi));

      buildDeviceModel();
      buildDeviceName();
   }

   CHumidity::~CHumidity()
   {
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CHumidity::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Humidity is a read-only message, can not be encoded");
   }

   void CHumidity::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CHumidity::filter() const
   {
   }

   void CHumidity::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      shared::CDataContainer details;
      details.set("type", pTypeHUM);
      details.set("subType", m_subType);
      details.set("id", m_id);
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      details.printToLog(YADOMS_LOG(information));
   }

   const std::string& CHumidity::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CHumidity::keywords()
   {
      return m_keywords;
   }

   void CHumidity::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CHumidity::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeHUM1: ssModel << "La Crosse TX3";
         break;
      case sTypeHUM2: ssModel << "La Crosse WS2300";
         break;
      case sTypeHUM3: ssModel << "Inovalley S80 plant humidity sensor";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages
