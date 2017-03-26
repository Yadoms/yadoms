#include "stdafx.h"
#include "Chime.h"
#include "ChimeByronSx.h"
#include "ChimeByronMp001.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CChime::CChime(boost::shared_ptr<yApi::IYPluginApi> api,
                  const std::string& command,
                  const shared::CDataContainer& deviceDetails)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
   {
      m_signalPower->set(0);

      createSubType(deviceDetails.get<unsigned char>("subType"));
      m_id = deviceDetails.get<unsigned int>("id");

      declare(api);
      m_subTypeManager->set(command, deviceDetails);
   }

   CChime::CChime(boost::shared_ptr<yApi::IYPluginApi> api,
                  unsigned int subType,
                  const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
   {
      m_signalPower->set(0);

      createSubType(static_cast<unsigned char>(subType));
      m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");

      declare(api);
      m_subTypeManager->reset();
   }

   CChime::CChime(boost::shared_ptr<yApi::IYPluginApi> api,
                  const RBUF& rbuf,
                  size_t rbufSize)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
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
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.CHIME.signalPower));

      declare(api);
   }

   CChime::~CChime()
   {
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

   void CChime::declare(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      if (!m_subTypeManager)
         throw shared::exception::CException("m_subTypeManager must be initialized");

      // Build device description
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeChime);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName,
                            m_subTypeManager->getModel(),
                            m_keywords,
                            details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CChime::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.CHIME);

      rbuf.CHIME.packetlength = ENCODE_PACKET_LENGTH(CHIME);
      rbuf.CHIME.packettype = pTypeChime;
      rbuf.CHIME.subtype = m_subType;
      rbuf.CHIME.seqnbr = seqNumberProvider->next();
      m_subTypeManager->idToProtocol(m_id, rbuf.CHIME.id1, rbuf.CHIME.id2, rbuf.CHIME.sound);
      m_subTypeManager->toProtocolState(rbuf.CHIME.sound);
      rbuf.CHIME.signalPower = 0;
      rbuf.CHIME.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(CHIME));
   }

   void CChime::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName,
                         m_keywords);
   }

   const std::string& CChime::getDeviceName() const
   {
      return m_deviceName;
   }

   void CChime::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages