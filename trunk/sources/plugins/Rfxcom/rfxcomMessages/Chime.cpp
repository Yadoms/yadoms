#include "stdafx.h"
#include "Chime.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CChime::CChime(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceDetails)
   :m_keyword("event"), m_rssi("rssi")
{
   m_rssi.set(0);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_id = deviceDetails.get<unsigned int>("id");

   std::string sound = deviceDetails.get<std::string>("sound");
   if     (sound == "Tubular 3 Notes") m_sound = kTubular3Notes;
   else if(sound == "Big Ben")         m_sound = kBigBen;
   else if(sound == "Tubular 2 Notes") m_sound = kTubular2Notes;
   else if(sound == "Solo")            m_sound = kSolo;
   else
   {
      YADOMS_LOG(warning) << "Chime, unsupported sound \"" << sound << "\", use default";
      m_sound = kDefaultSound;
   }

   Init(context);
}

CChime::CChime(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_keyword("event"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeChime, GET_RBUF_STRUCT_SIZE(CHIME), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.CHIME.subtype;
   m_id = rbuf.CHIME.id1 << 8 | rbuf.CHIME.id2;

   switch(rbuf.CHIME.sound)
   {
   case chime_sound0:
   case chime_sound4:
      m_sound = kTubular3Notes;
      break;
   case chime_sound1:
   case chime_sound5:
      m_sound = kBigBen;
      break;
   case chime_sound2:
   case chime_sound6:
      m_sound = kTubular2Notes;
      break;
   case chime_sound3:
   case chime_sound7:
      m_sound = kSolo;
      break;
   default:
      YADOMS_LOG(warning) << "Chime, unsupported received sound \"" << rbuf.CHIME.sound << "\", use default";
      m_sound = kDefaultSound;
      break;
   }

   m_rssi.set(NormalizeRssiLevel(rbuf.CHIME.rssi));

   Init(context);
}

CChime::~CChime()
{
}

void CChime::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeChime);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_keyword);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CChime::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.CHIME);

   rbuf.CHIME.packetlength = ENCODE_PACKET_LENGTH(CHIME);
   rbuf.CHIME.packettype = pTypeChime;
   rbuf.CHIME.subtype = m_subType;
   rbuf.CHIME.seqnbr = seqNumberProvider->next();
   rbuf.CHIME.id1 = (unsigned char) (0xFF & (m_id >> 8));
   rbuf.CHIME.id2 = (unsigned char) (0xFF & m_id);
   switch(m_sound)
   {
   case kTubular3Notes: rbuf.CHIME.sound = chime_sound0; break;
   case kBigBen       : rbuf.CHIME.sound = chime_sound1; break;
   case kTubular2Notes: rbuf.CHIME.sound = chime_sound2; break;
   case kSolo         : rbuf.CHIME.sound = chime_sound3; break;
   default:
      BOOST_ASSERT_MSG(false, "Unsupported sound value");
      break;
   }
   rbuf.CHIME.rssi = 0;
   rbuf.CHIME.filler = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(CHIME));
}

void CChime::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_keyword);
   context->historizeData(m_deviceName, m_rssi);
}

void CChime::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CChime::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeByronSX   : ssModel << "Byron SX Chime"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages