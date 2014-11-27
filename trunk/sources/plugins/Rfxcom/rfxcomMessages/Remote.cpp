#include "stdafx.h"
#include "Remote.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include "RemoteStandard.hpp"
#include "RemoteAtiWonder2.h"
#include "specificHistorizers/RemoteAtiWonderHistorizer.h"
#include "specificHistorizers/RemoteAtiWonderPlusHistorizer.h"
#include "specificHistorizers/RemoteMedionHistorizer.h"
#include "specificHistorizers/RemotePCHistorizer.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CRemote::CRemote(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   createSubType(deviceDetails.get<unsigned char>("subType"));
   m_id = deviceDetails.get<unsigned int>("id");

   declare(context);
   m_subTypeManager->set(command);
}

CRemote::CRemote(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeRemote, GET_RBUF_STRUCT_SIZE(REMOTE), DONT_CHECK_SEQUENCE_NUMBER);

   createSubType(rbuf.REMOTE.subtype);
   m_id = rbuf.REMOTE.id;
   m_subTypeManager->setFromProtocolState(rbuf);

   m_rssi.set(NormalizeRssiLevel(rbuf.REMOTE.rssi));

   declare(context);
}

CRemote::~CRemote()
{
}

void CRemote::createSubType(unsigned char subType)
{
   m_subType = subType;
   switch(m_subType)
   {
   case sTypeATI : m_subTypeManager.reset(new CRemoteStandard<specificHistorizers::CRemoteAtiWonderHistorizer>("ATI Remote Wonder")); break;
   case sTypeATIplus : m_subTypeManager.reset(new CRemoteStandard<specificHistorizers::CRemoteAtiWonderPlusHistorizer>("ATI Remote Wonder Plus")); break;
   case sTypeMedion : m_subTypeManager.reset(new CRemoteStandard<specificHistorizers::CRemoteMedionHistorizer>("Medion Remote")); break;
   case sTypePCremote : m_subTypeManager.reset(new CRemoteStandard<specificHistorizers::CRemotePCHistorizer>("X10 PC Remote")); break;
   case sTypeATIrw2 : m_subTypeManager.reset(new CRemoteAtiWonder2()); break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }
}

void CRemote::declare(boost::shared_ptr<yApi::IYadomsApi> context)
{
   BOOST_ASSERT_MSG(!!m_subTypeManager, "m_subTypeManager must be initialized");

   // Build device description
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeRemote);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_subTypeManager->getModel(), details.serialize());

      context->declareKeyword(m_deviceName, m_rssi);
   }

   m_subTypeManager->declare(context, m_deviceName);
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CRemote::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.REMOTE);

   rbuf.REMOTE.packetlength = ENCODE_PACKET_LENGTH(REMOTE);
   rbuf.REMOTE.packettype = pTypeRemote;
   rbuf.REMOTE.subtype = m_subType;
   rbuf.REMOTE.seqnbr = seqNumberProvider->next();
   rbuf.REMOTE.id = (BYTE)m_id;
   m_subTypeManager->toProtocolState(rbuf);
   rbuf.REMOTE.rssi = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(REMOTE));
}

void CRemote::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   m_subTypeManager->historize(context, m_deviceName);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CRemote::getDeviceName() const
{
   return m_deviceName;
}                     

void CRemote::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << m_id;
   m_deviceName = ssdeviceName.str();
}

} // namespace rfxcomMessages