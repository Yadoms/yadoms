#include "stdafx.h"
#include "Thermostat1.h"
#include "Thermostat1Digimax.h"
#include "Thermostat1DigimaxShort.h"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CThermostat1::CThermostat1(boost::shared_ptr<yApi::IYPluginApi> api,
                              const std::string& command,
                              const shared::CDataContainer& deviceDetails)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      m_signalPower->set(0);

      createSubType(deviceDetails.get<unsigned char>("subType"));
      m_id = deviceDetails.get<unsigned int>("id");

      declare(api);
      m_subTypeManager->set(command);
   }

   CThermostat1::CThermostat1(boost::shared_ptr<yApi::IYPluginApi> api,
                              unsigned int subType,
                              const std::string& name,
                              const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      m_signalPower->set(0);

      createSubType(static_cast<unsigned char>(subType));

      m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);

      m_subTypeManager->reset();
   }

   CThermostat1::CThermostat1(boost::shared_ptr<yApi::IYPluginApi> api,
                              const RBUF& rbuf,
                              size_t rbufSize)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeThermostat1,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(THERMOSTAT1),
                           DONT_CHECK_SEQUENCE_NUMBER);

      createSubType(rbuf.THERMOSTAT1.subtype);
      m_id = rbuf.THERMOSTAT1.id1 << 8 | rbuf.THERMOSTAT1.id2;
      m_subTypeManager->setFromProtocolState(rbuf);
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.THERMOSTAT1.rssi));

      declare(api);
   }

   CThermostat1::~CThermostat1()
   {
   }

   void CThermostat1::buildDeviceDetails()
   {
      if (m_deviceDetails.empty())
      {
         m_deviceDetails.set("type", pTypeThermostat1);
         m_deviceDetails.set("subType", m_subType);
         m_deviceDetails.set("id", m_id);
      }
   }

   void CThermostat1::createSubType(unsigned char subType)
   {
      m_subType = subType;
      switch (m_subType)
      {
      case sTypeAC: m_subTypeManager = boost::make_shared<CThermostat1Digimax>();
         break;
      case sTypeHEU: m_subTypeManager = boost::make_shared<CThermostat1DigimaxShort>();
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      m_keywords.insert(m_keywords.end(), m_subTypeManager->keywords().begin(), m_subTypeManager->keywords().end());
   }

   void CThermostat1::declare(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      if (!m_subTypeManager)
         throw shared::exception::CException("m_subTypeManager must be initialized");

      // Build device description
      buildDeviceName();
      auto model = m_subTypeManager->getModel();
      buildDeviceDetails();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         api->declareDevice(m_deviceName, model, model, m_keywords, m_deviceDetails);
         YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << model << ")";
         m_deviceDetails.printToLog(YADOMS_LOG(information));         
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CThermostat1::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.THERMOSTAT1);

      rbuf.THERMOSTAT1.packetlength = ENCODE_PACKET_LENGTH(THERMOSTAT1);
      rbuf.THERMOSTAT1.packettype = pTypeThermostat1;
      rbuf.THERMOSTAT1.subtype = m_subType;
      rbuf.THERMOSTAT1.seqnbr = seqNumberProvider->next();
      rbuf.THERMOSTAT1.id1 = static_cast<unsigned char>(0xFF & (m_id >> 8));
      rbuf.THERMOSTAT1.id2 = static_cast<unsigned char>(0xFF & m_id);
      m_subTypeManager->toProtocolState(rbuf);
      rbuf.THERMOSTAT1.rssi = 0;
      rbuf.THERMOSTAT1.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(THERMOSTAT1));
   }

   void CThermostat1::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CThermostat1::getDeviceName() const
   {
      return m_deviceName;
   }

   void CThermostat1::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages


