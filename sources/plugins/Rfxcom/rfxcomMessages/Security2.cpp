#include "stdafx.h"
#include "Security2.h"
#include "Security2ClassicKeyLoq.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CSecurity2::CSecurity2(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& keyword,
                          const std::string& command,
                          const shared::CDataContainer& deviceDetails)
      : m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_batteryLevel, m_signalPower})
   {
      m_batteryLevel->set(100);
      m_signalPower->set(0);

      createSubType(deviceDetails.get<unsigned char>("subType"));
      m_subTypeManager->setId(deviceDetails.get<unsigned int>("id"));

      declare(api);
      m_subTypeManager->set(keyword, command);
   }

   CSecurity2::CSecurity2(boost::shared_ptr<yApi::IYPluginApi> api,
                          unsigned int subType,
                          const std::string& name,
                          const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_batteryLevel, m_signalPower})
   {
      m_batteryLevel->set(100);
      m_signalPower->set(0);

      createSubType(static_cast<unsigned char>(subType));
      m_subTypeManager->setId(manuallyDeviceCreationConfiguration.get<unsigned int>("id"));

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);

      m_subTypeManager->resetState();
   }

   CSecurity2::CSecurity2(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize)
      : m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_batteryLevel, m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeSecurity2,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(SECURITY2),
                           DONT_CHECK_SEQUENCE_NUMBER);

      createSubType(rbuf.SECURITY2.subtype);
      m_subTypeManager->setFromProtocolState(rbuf);
      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.SECURITY2.battery_level));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.SECURITY2.rssi));

      declare(api);
   }

   CSecurity2::~CSecurity2()
   {
   }

   void CSecurity2::buildDeviceDetails()
   {
      if (m_deviceDetails.empty())
      {
         m_deviceDetails.set("type", pTypeSecurity2);
         m_deviceDetails.set("subType", m_subType);
         m_deviceDetails.set("id", m_subTypeManager->getId());
      }
   }

   void CSecurity2::createSubType(unsigned char subType)
   {
      m_subType = subType;
      switch (m_subType)
      {
      case CSecurity2ClassicKeyLoq::rfxValue: m_subTypeManager = boost::make_shared<CSecurity2ClassicKeyLoq>();
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      m_keywords.insert(m_keywords.end(), m_subTypeManager->keywords().begin(), m_subTypeManager->keywords().end());
   }

   void CSecurity2::declare(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      if (!m_subTypeManager)
         throw shared::exception::CException("m_subTypeManager must be initialized");

      // Build device description
      buildDeviceName();
      auto model = m_subTypeManager->getModel();
      buildDeviceDetails();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
         api->declareDevice(m_deviceName, model, model, m_keywords, m_deviceDetails);
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CSecurity2::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.SECURITY2);

      rbuf.SECURITY2.packetlength = ENCODE_PACKET_LENGTH(SECURITY2);
      rbuf.SECURITY2.packettype = pTypeSecurity2;
      rbuf.SECURITY2.subtype = m_subType;
      rbuf.SECURITY2.seqnbr = seqNumberProvider->next();
      m_subTypeManager->toProtocolState(rbuf);
      rbuf.SECURITY2.rssi = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(SECURITY2));
   }

   void CSecurity2::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CSecurity2::getDeviceName() const
   {
      return m_deviceName;
   }

   void CSecurity2::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << m_subTypeManager->getId();
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages


