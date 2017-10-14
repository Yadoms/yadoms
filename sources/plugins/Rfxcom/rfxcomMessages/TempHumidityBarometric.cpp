#include "stdafx.h"
#include "TempHumidityBarometric.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CTempHumidityBarometric::CTempHumidityBarometric(boost::shared_ptr<yApi::IYPluginApi> api,
                                                    const RBUF& rbuf,
                                                    size_t rbufSize)
      : m_temperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
      m_humidity(boost::make_shared<yApi::historization::CHumidity>("humidity")),
      m_pressure(boost::make_shared<yApi::historization::CPressure>("pressure")),
      m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_temperature , m_humidity, m_pressure, m_batteryLevel , m_signalPower })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeTEMP_HUM_BARO,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(TEMP_HUM_BARO),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.TEMP_HUM_BARO.subtype;

      m_id = rbuf.TEMP_HUM_BARO.id1 | (rbuf.TEMP_HUM_BARO.id2 << 8);

      m_temperature->set(NormalizeTemperature(rbuf.TEMP_HUM_BARO.temperatureh, rbuf.TEMP_HUM_BARO.temperaturel, rbuf.TEMP_HUM_BARO.tempsign == 1));
      m_humidity->set(rbuf.TEMP_HUM_BARO.humidity);

      m_pressure->set(rbuf.TEMP_HUM_BARO.baroh << 8 | (rbuf.TEMP_HUM_BARO.barol));

      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.TEMP_HUM_BARO.battery_level));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.TEMP_HUM_BARO.rssi));

      Init(api);
   }

   CTempHumidityBarometric::~CTempHumidityBarometric()
   {
   }

   void CTempHumidityBarometric::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeTEMP_HUM_BARO);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
         YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
         details.printToLog(YADOMS_LOG(information));
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CTempHumidityBarometric::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("TempHumidityBarometric is a read-only message, can not be encoded");
   }

   void CTempHumidityBarometric::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CTempHumidityBarometric::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CTempHumidityBarometric::keywords()
   {
      return m_keywords;
   }

   void CTempHumidityBarometric::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CTempHumidityBarometric::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeTHB1: ssModel << "BTHR918, BTHGN129";
         break;
      case sTypeTHB2: ssModel << "BTHR918N, BTHR968";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages


