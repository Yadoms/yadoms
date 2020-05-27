#include "stdafx.h"
#include "TempRain.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CTempRain::CTempRain(boost::shared_ptr<yApi::IYPluginApi> api,
                        const RBUF& rbuf,
                        size_t rbufSize)
      : m_temperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
        m_rain(boost::make_shared<yApi::historization::CRain>("rain",
                                                              yApi::EKeywordAccessMode::kGet,
                                                              yApi::EMeasureType::kCumulative)),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_temperature , m_rain, m_batteryLevel , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeTEMP_RAIN,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(TEMP_RAIN),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.TEMP_RAIN.subtype;

      m_id = rbuf.TEMP_RAIN.id1 | (rbuf.TEMP_RAIN.id2 << 8);

      m_temperature->set(NormalizeTemperature(rbuf.TEMP_RAIN.temperatureh, rbuf.TEMP_RAIN.temperaturel, rbuf.TEMP_RAIN.tempsign == 1));
      m_rain->set(static_cast<double>((rbuf.TEMP_RAIN.raintotal1 << 8) | rbuf.TEMP_RAIN.raintotal2) / 10.0);
      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.TEMP_RAIN.battery_level));
      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.TEMP_RAIN.rssi));

      buildDeviceModel();
      buildDeviceName();
   }

   CTempRain::~CTempRain()
   {
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CTempRain::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Temp is a read-only message, can not be encoded");
   }

   void CTempRain::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CTempRain::filter() const
   {
   }

   void CTempRain::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      boost::shared_ptr<shared::CDataContainer> details = shared::CDataContainer::make();
      details->set("type", pTypeTEMP_RAIN);
      details->set("subType", m_subType);
      details->set("id", m_id);
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      details->printToLog(YADOMS_LOG(information));
   }

   const std::string& CTempRain::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CTempRain::keywords()
   {
      return m_keywords;
   }

   void CTempRain::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CTempRain::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeTR1: ssModel << "Alecto WS1200";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages
