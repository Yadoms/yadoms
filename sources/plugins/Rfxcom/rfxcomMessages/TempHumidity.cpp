#include "stdafx.h"
#include "TempHumidity.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CTempHumidity::CTempHumidity(boost::shared_ptr<yApi::IYPluginApi> api,
                                const RBUF& rbuf,
                                size_t rbufSize)
      : m_temperature(boost::make_shared<yApi::historization::CTemperature>("temperature")),
      m_humidity(boost::make_shared<yApi::historization::CHumidity>("humidity")),
      m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
      m_signalStrength(boost::make_shared<yApi::historization::CSignalStrength>("signalStrength")),
      m_keywords({ m_temperature , m_humidity, m_batteryLevel , m_signalStrength })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeTEMP_HUM,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(TEMP_HUM),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.TEMP_HUM.subtype;

      m_id = rbuf.TEMP_HUM.id1 | (rbuf.TEMP_HUM.id2 << 8);

      m_temperature->set(NormalizeTemperature(rbuf.TEMP_HUM.temperatureh, rbuf.TEMP_HUM.temperaturel, rbuf.TEMP_HUM.tempsign == 1));
      m_humidity->set(rbuf.TEMP_HUM.humidity);

      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.TEMP_HUM.battery_level));
      m_signalStrength->set(NormalizesignalStrengthLevel(rbuf.TEMP_HUM.signalStrength));

      Init(api);
   }

   CTempHumidity::~CTempHumidity()
   {
   }

   void CTempHumidity::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeTEMP_HUM);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName, m_deviceModel, m_keywords, details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CTempHumidity::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("TempHumidity is a read-only message, can not be encoded");
   }

   void CTempHumidity::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CTempHumidity::getDeviceName() const
   {
      return m_deviceName;
   }

   void CTempHumidity::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CTempHumidity::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeTH1: ssModel << "THGN122/123, THGN132, THGR122/228/238/268";
         break;
      case sTypeTH2: ssModel << "THGR810, THGN800";
         break;
      case sTypeTH3: ssModel << "RTGR328";
         break;
      case sTypeTH4: ssModel << "THGR328";
         break;
      case sTypeTH5: ssModel << "WTGR800";
         break;
      case sTypeTH6: ssModel << "THGR918, THGRN228, THGN500";
         break;
      case sTypeTH7: ssModel << "TFA TS34C, Cresta";
         break;
      case sTypeTH8: ssModel << "WT450H";
         break;
      case sTypeTH9: ssModel << "Viking 02035, 02038 (02035 has no humidity), TSS320";
         break;
      case sTypeTH10: ssModel << "Rubicson";
         break;
      case sTypeTH11: ssModel << "EW109";
         break;
      case sTypeTH12: ssModel << "Imagintronix";
         break;
      case sTypeTH13: ssModel << "Alecto WS1700 and compatibles";
         break;
      case sTypeTH14: ssModel << "Alecto WS3500, WS4500, Auriol H13726, Hama EWS1500, Meteoscan W155/W160, Ventus WS155";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages


