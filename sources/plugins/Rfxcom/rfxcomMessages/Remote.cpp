#include "stdafx.h"
#include "Remote.h"
#include "RemoteStandard.hpp"
#include "RemoteAtiWonder2.h"
#include "specificHistorizers/RemoteAtiWonderHistorizer.h"
#include "specificHistorizers/RemoteAtiWonderPlusHistorizer.h"
#include "specificHistorizers/RemoteMedionHistorizer.h"
#include "specificHistorizers/RemotePCHistorizer.h"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CRemote::CRemote(boost::shared_ptr<yApi::IYPluginApi> api,
                    const std::string& command,
                    const shared::CDataContainer& deviceDetails)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      m_signalPower->set(0);

      createSubType(deviceDetails.get<unsigned char>("subType"));
      m_subTypeManager->set(command);
      m_id = deviceDetails.get<unsigned int>("id");

      buildDeviceName();
   }

   CRemote::CRemote(boost::shared_ptr<yApi::IYPluginApi> api,
                    const RBUF& rbuf,
                    size_t rbufSize)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeRemote,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(REMOTE),
                           DONT_CHECK_SEQUENCE_NUMBER);

      createSubType(rbuf.REMOTE.subtype);
      m_id = rbuf.REMOTE.id;
      m_subTypeManager->setFromProtocolState(rbuf);

      m_signalPower->set(NormalizesignalPowerLevel(rbuf.REMOTE.rssi));

      buildDeviceName();
   }

   CRemote::~CRemote()
   {
   }

   void CRemote::createSubType(unsigned char subType)
   {
      m_subType = subType;
      switch (m_subType)
      {
      case sTypeATI: m_subTypeManager = boost::make_shared<CRemoteStandard<specificHistorizers::CRemoteAtiWonderHistorizer, specificHistorizers::ERemoteAtiWonderCodes>>("ATI Remote Wonder");
         break;
      case sTypeATIplus: m_subTypeManager = boost::make_shared<CRemoteStandard<specificHistorizers::CRemoteAtiWonderPlusHistorizer, specificHistorizers::ERemoteAtiWonderPlusCodes>>("ATI Remote Wonder Plus");
         break;
      case sTypeMedion: m_subTypeManager = boost::make_shared<CRemoteStandard<specificHistorizers::CRemoteMedionHistorizer, specificHistorizers::ERemoteMedionCodes>>("Medion Remote");
         break;
      case sTypePCremote: m_subTypeManager = boost::make_shared<CRemoteStandard<specificHistorizers::CRemotePCHistorizer, specificHistorizers::ERemotePCCodes>>("X10 PC Remote");
         break;
      case sTypeATIrw2: m_subTypeManager = boost::make_shared<CRemoteAtiWonder2>();
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      m_keywords.insert(m_keywords.end(), m_subTypeManager->keywords().begin(), m_subTypeManager->keywords().end());
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CRemote::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.REMOTE);

      rbuf.REMOTE.packetlength = ENCODE_PACKET_LENGTH(REMOTE);
      rbuf.REMOTE.packettype = pTypeRemote;
      rbuf.REMOTE.subtype = m_subType;
      rbuf.REMOTE.seqnbr = seqNumberProvider->next();
      rbuf.REMOTE.id = static_cast<BYTE>(m_id);
      m_subTypeManager->toProtocolState(rbuf);
      rbuf.REMOTE.rssi = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(REMOTE));
   }

   void CRemote::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CRemote::filter() const
   {
   }

   void CRemote::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      shared::CDataContainer details;
      details.set("type", pTypeRemote);
      details.set("subType", m_subType);
      details.set("id", m_id);
      auto model = m_subTypeManager->getModel();
      api->declareDevice(m_deviceName, model, model, m_keywords, details);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << model << ")";
      details.printToLog(YADOMS_LOG(information));
   }

   const std::string& CRemote::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CRemote::keywords()
   {
      return m_keywords;
   }

   void CRemote::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << m_id;
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages
