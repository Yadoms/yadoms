#include "stdafx.h"
#include "Chime.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include "ChimeByronSx.h"
#include "ChimeByronMp001.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CChime::CChime(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   createSubType(deviceDetails.get<unsigned char>("subType"));
   m_id = deviceDetails.get<unsigned int>("id");

   declare(context);
   m_subTypeManager->set(command, deviceDetails);
}

CChime::CChime(boost::shared_ptr<yApi::IYPluginApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   createSubType(subType);
   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");

   declare(context);
   m_subTypeManager->reset();
}

CChime::CChime(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeChime, GET_RBUF_STRUCT_SIZE(CHIME), DONT_CHECK_SEQUENCE_NUMBER);

   createSubType(rbuf.CHIME.subtype);
   m_id = m_subTypeManager->idFromProtocol(rbuf.CHIME.id1, rbuf.CHIME.id2, rbuf.CHIME.sound);
   m_subTypeManager->setFromProtocolState(rbuf.CHIME.sound);
   m_rssi.set(NormalizeRssiLevel(rbuf.CHIME.rssi));

   declare(context);
}

CChime::~CChime()
{
}

void CChime::createSubType(unsigned char subType)
{
   m_subType = subType;
   switch(m_subType)
   {
   case sTypeByronSX : m_subTypeManager.reset(new CChimeByronSx()); break;
   case sTypeByronMP001 : m_subTypeManager.reset(new CChimeByronMp001()); break;
   case sTypeSelectPlus: m_subTypeManager.reset(new CChimeByronMp001()); break;
   case sTypeSelectPlus3: m_subTypeManager.reset(new CChimeByronMp001()); break;
   case sTypeEnvivo: m_subTypeManager.reset(new CChimeByronMp001()); break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }
}

void CChime::declare(boost::shared_ptr<yApi::IYPluginApi> context)
{
   BOOST_ASSERT_MSG(!!m_subTypeManager, "m_subTypeManager must be initialized");

   // Build device description
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeChime);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_subTypeManager->getModel(), details.serialize());

      m_subTypeManager->declare(context, m_deviceName);
      context->declareKeyword(m_deviceName, m_rssi);
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
   rbuf.CHIME.rssi = 0;
   rbuf.CHIME.filler = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(CHIME));
}

void CChime::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   m_subTypeManager->historize(context, m_deviceName);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CChime::getDeviceName() const
{
   return m_deviceName;
}                     

void CChime::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

} // namespace rfxcomMessages