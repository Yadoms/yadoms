#include "stdafx.h"
#include "Remote.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CRemote::CRemote(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceDetails)
   :m_keyword("command"), m_rssi("rssi")
{
   m_rssi.set(0);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_id = deviceDetails.get<unsigned int>("id");
   m_cmndType = deviceDetails.get<unsigned int>("commandType");
   m_cmnd = deviceDetails.get<unsigned int>("command");

   Init(context);
}

CRemote::CRemote(boost::shared_ptr<yApi::IYadomsApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_keyword("command"), m_rssi("rssi")
{
   m_rssi.set(0);

   m_subType = subType;
   switch(m_subType)
   {
   case sTypeATI         :
   case sTypeATIplus     :
   case sTypeMedion      :
   case sTypePCremote    :
   case sTypeATIrw2      :
      break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }

   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
   m_cmndType = manuallyDeviceCreationConfiguration.get<unsigned int>("commandType");
   m_cmnd = manuallyDeviceCreationConfiguration.get<unsigned int>("command");

   Init(context);
}

CRemote::CRemote(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_keyword("command"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeRemote, GET_RBUF_STRUCT_SIZE(REMOTE), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.REMOTE.subtype;
   m_id = rbuf.REMOTE.id;
   m_cmndType = rbuf.REMOTE.cmndtype;
   m_cmnd = rbuf.REMOTE.cmnd;

   m_rssi.set(NormalizeRssiLevel(rbuf.REMOTE.rssi));

   Init(context);
}

CRemote::~CRemote()
{
}

void CRemote::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeRemote);
      details.set("subType", m_subType);
      details.set("id", m_id);
      details.set("commandType", m_cmndType);
      details.set("command", m_cmnd);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_keyword);
      context->declareKeyword(m_deviceName, m_rssi);
   }
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
   rbuf.REMOTE.cmndtype = (BYTE)m_cmndType;
   rbuf.REMOTE.cmnd = (BYTE)m_cmnd;
   rbuf.REMOTE.rssi = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(REMOTE));
}

void CRemote::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_keyword);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CRemote::getDeviceName() const
{
   return m_deviceName;
}                     

void CRemote::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << m_id << "." << m_cmndType << "." << m_cmnd;
   m_deviceName = ssdeviceName.str();
}

void CRemote::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeATI         : ssModel << "ATI Remote Wonder"; break;
   case sTypeATIplus     : ssModel << "ATI Remote Wonder Plus"; break;
   case sTypeMedion      : ssModel << "Medion Remote"; break;
   case sTypePCremote    : ssModel << "X10 PC Remote"; break;
   case sTypeATIrw2      : ssModel << "ATI Remote Wonder II"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages