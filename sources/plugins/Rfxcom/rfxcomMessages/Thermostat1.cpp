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
                              const boost::shared_ptr<shared::CDataContainer>& deviceDetails)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      m_signalPower->set(0);

      createSubType(static_cast<unsigned char>(deviceDetails->get<unsigned int>("subType")));
      m_subTypeManager->set(command);
      m_id = deviceDetails->get<unsigned int>("id");

      // Build device description
      buildDeviceName();
      auto model = m_subTypeManager->getModel();
      m_deviceDetails = deviceDetails;
   }

   CThermostat1::CThermostat1(boost::shared_ptr<yApi::IYPluginApi> api,
                              unsigned int subType,
                              const std::string& name,
                              const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      m_signalPower->set(0);

      createSubType(static_cast<unsigned char>(subType));

      m_id = manuallyDeviceCreationConfiguration->get<unsigned int>("id");

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
      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.THERMOSTAT1.rssi));

      // Build device description
      buildDeviceName();
      buildDeviceDetails();
   }

   CThermostat1::~CThermostat1()
   {
   }

   void CThermostat1::buildDeviceDetails()
   {
      if (m_deviceDetails->empty())
      {
         m_deviceDetails->set("type", pTypeThermostat1);
         m_deviceDetails->set("subType", m_subType);
         m_deviceDetails->set("id", m_id);
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

   void CThermostat1::filter() const
   {
   }

   void CThermostat1::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->declareDevice(m_deviceName, m_subTypeManager->getModel(), m_subTypeManager->getModel(), m_keywords, m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_subTypeManager->getModel() << ")";
      m_deviceDetails->printToLog(YADOMS_LOG(information));
   }

   const std::string& CThermostat1::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CThermostat1::keywords()
   {
      return m_keywords;
   }

   void CThermostat1::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages
