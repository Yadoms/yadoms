#include "stdafx.h"
#include "Security1.h"
#include "Security1X10.h"
#include "Security1X10M.h"
#include "Security1X10R.h"
#include "Security1KD101_SA30.h"
#include "Security1PowerCodeSensor.h"
#include "Security1PowerCodeMotion.h"
#include "Security1Meiantech.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CSecurity1::CSecurity1(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& keyword,
                          const std::string& command,
                          const shared::CDataContainer& deviceDetails)
      : m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_rssi(boost::make_shared<yApi::historization::CRssi>("rssi")),
        m_keywords({m_batteryLevel, m_rssi})
   {
      m_batteryLevel->set(100);
      m_rssi->set(0);

      createSubType(deviceDetails.get<unsigned char>("subType"));
      m_id = deviceDetails.get<unsigned int>("id");

      declare(api);
      m_subTypeManager->set(keyword, command);
   }

   CSecurity1::CSecurity1(boost::shared_ptr<yApi::IYPluginApi> api,
                          unsigned int subType,
                          const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_rssi(boost::make_shared<yApi::historization::CRssi>("rssi")),
        m_keywords({m_batteryLevel, m_rssi})
   {
      m_batteryLevel->set(100);
      m_rssi->set(0);

      createSubType(static_cast<unsigned char>(subType));
      m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");

      declare(api);
      m_subTypeManager->reset();
   }

   CSecurity1::CSecurity1(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize)
      : m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_rssi(boost::make_shared<yApi::historization::CRssi>("rssi")),
        m_keywords({m_batteryLevel, m_rssi})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeSecurity1,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(SECURITY1),
                           DONT_CHECK_SEQUENCE_NUMBER);

      createSubType(rbuf.SECURITY1.subtype);
      m_id = m_subTypeManager->idFromProtocol(rbuf);
      m_subTypeManager->setFromProtocolState(rbuf.SECURITY1.status);
      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.SECURITY1.battery_level));
      m_rssi->set(NormalizeRssiLevel(rbuf.SECURITY1.rssi));

      declare(api);
   }

   CSecurity1::~CSecurity1()
   {
   }

   void CSecurity1::createSubType(unsigned char subType)
   {
      m_subType = subType;
      switch (m_subType)
      {
      case CSecurity1X10::rfxValue: m_subTypeManager = boost::make_shared<CSecurity1X10>();
         break;
      case CSecurity1X10M::rfxValue: m_subTypeManager = boost::make_shared<CSecurity1X10M>();
         break;
      case CSecurity1X10R::rfxValue: m_subTypeManager = boost::make_shared<CSecurity1X10R>();
         break;
      case CSecurity1KD101_SA30::rfxValueKD101: m_subTypeManager = boost::make_shared<CSecurity1KD101_SA30>(CSecurity1KD101_SA30::rfxValueKD101,
                                                                                                            "KD101");
         break;
      case CSecurity1PowerCodeSensor::rfxValuePowercodeSensor: m_subTypeManager = boost::make_shared<CSecurity1PowerCodeSensor>(false);
         break;
      case CSecurity1PowerCodeSensor::rfxValuePowercodeSensorAux: m_subTypeManager = boost::make_shared<CSecurity1PowerCodeSensor>(true);
         break;
      case CSecurity1PowerCodeMotion::rfxValue: m_subTypeManager = boost::make_shared<CSecurity1PowerCodeMotion>();
         break;
      case CSecurity1Meiantech::rfxValue: m_subTypeManager = boost::make_shared<CSecurity1Meiantech>();
         break;
      case CSecurity1KD101_SA30::rfxValueSA30: m_subTypeManager = boost::make_shared<CSecurity1KD101_SA30>(CSecurity1KD101_SA30::rfxValueSA30,
                                                                                                           "SA30");
         break;
      case CSecurity1KD101_SA30::rfxValueRM174RF: m_subTypeManager = boost::make_shared<CSecurity1KD101_SA30>(CSecurity1KD101_SA30::rfxValueRM174RF,
                                                                                                              "RM174RF");
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      m_keywords.insert(m_keywords.end(), m_subTypeManager->keywords().begin(), m_subTypeManager->keywords().end());
   }

   void CSecurity1::declare(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      if (!m_subTypeManager)
         throw shared::exception::CException("m_subTypeManager must be initialized");

      // Build device description
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeSecurity1);
         details.set("subType", m_subType);
         details.set("id", m_id);

         api->declareDevice(m_deviceName, m_subTypeManager->getModel(), m_keywords, details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CSecurity1::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.SECURITY1);

      rbuf.SECURITY1.packetlength = ENCODE_PACKET_LENGTH(SECURITY1);
      rbuf.SECURITY1.packettype = pTypeSecurity1;
      rbuf.SECURITY1.subtype = m_subType;
      rbuf.SECURITY1.seqnbr = seqNumberProvider->next();
      rbuf.SECURITY1.id1 = static_cast<unsigned char>((m_id & 0xFF00) >> 8);
      rbuf.SECURITY1.id2 = static_cast<unsigned char>(m_id & 0xFF);
      rbuf.SECURITY1.status = m_subTypeManager->toProtocolState();
      rbuf.SECURITY1.rssi = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(SECURITY1));
   }

   void CSecurity1::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CSecurity1::getDeviceName() const
   {
      return m_deviceName;
   }

   void CSecurity1::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << m_id;
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages
