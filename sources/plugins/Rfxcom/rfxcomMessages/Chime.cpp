#include "stdafx.h"
#include "Chime.h"
#include "ChimeByronSx.h"
#include "ChimeByronMp001.h"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CChime::CChime(boost::shared_ptr<yApi::IYPluginApi> api,
                  const std::string& command,
                  const shared::CDataContainer& deviceDetails)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      m_signalPower->set(0);

      createSubType(static_cast<unsigned char>(deviceDetails.get<unsigned int>("subType")));
      m_subTypeManager->set(command, deviceDetails);
      m_id = deviceDetails.get<unsigned int>("id");

      // Build device description
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CChime::CChime(boost::shared_ptr<yApi::IYPluginApi> api,
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

   CChime::CChime(boost::shared_ptr<yApi::IYPluginApi> api,
                  const RBUF& rbuf,
                  size_t rbufSize)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeChime,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(CHIME),
                           DONT_CHECK_SEQUENCE_NUMBER);

      createSubType(rbuf.CHIME.subtype);
      m_id = m_subTypeManager->idFromProtocol(rbuf.CHIME.id1, rbuf.CHIME.id2, rbuf.CHIME.sound);
      m_subTypeManager->setFromProtocolState(rbuf.CHIME.sound);
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.CHIME.rssi));

      // Build device description
      buildDeviceName();
      buildDeviceDetails();
   }

   CChime::~CChime()
   {
   }

   void CChime::buildDeviceDetails()
   {
      if (m_deviceDetails.empty())
      {
         m_deviceDetails.set("type", pTypeChime);
         m_deviceDetails.set("subType", m_subType);
         m_deviceDetails.set("id", m_id);
      }
   }

   void CChime::createSubType(unsigned char subType)
   {
      m_subType = subType;
      switch (m_subType)
      {
      case sTypeByronSX: m_subTypeManager = boost::make_shared<CChimeByronSx>();
         break;
      case sTypeByronMP001: m_subTypeManager = boost::make_shared<CChimeByronMp001>();
         break;
      case sTypeSelectPlus: m_subTypeManager = boost::make_shared<CChimeByronMp001>();
         break;
      case sTypeSelectPlus3: m_subTypeManager = boost::make_shared<CChimeByronMp001>();
         break;
      case sTypeEnvivo: m_subTypeManager = boost::make_shared<CChimeByronMp001>();
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      m_keywords.insert(m_keywords.end(), m_subTypeManager->keywords().begin(), m_subTypeManager->keywords().end());
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CChime::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.CHIME);

      rbuf.CHIME.packetlength = ENCODE_PACKET_LENGTH(CHIME);
      rbuf.CHIME.packettype = pTypeChime;
      rbuf.CHIME.subtype = m_subType;
      rbuf.CHIME.seqnbr = seqNumberProvider->next();
      m_subTypeManager->idToProtocol(m_id, rbuf.CHIME.id1, rbuf.CHIME.id2, rbuf.CHIME.sound);
      m_subTypeManager->toProtocolState(rbuf.CHIME.sound);
      rbuf.CHIME.rssi = 0;
      rbuf.CHIME.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(CHIME));
   }

   void CChime::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName,
                         m_keywords);
   }

   void CChime::filter() const
   {
   }

   void CChime::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->declareDevice(m_deviceName, m_subTypeManager->getModel(), m_subTypeManager->getModel(), m_keywords, m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_subTypeManager->getModel() << ")";
      m_deviceDetails.printToLog(YADOMS_LOG(information));
   }

   const std::string& CChime::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CChime::keywords()
   {
      return m_keywords;
   }

   void CChime::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages
