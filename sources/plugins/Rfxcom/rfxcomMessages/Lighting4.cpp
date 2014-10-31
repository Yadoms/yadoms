#include "stdafx.h"
#include "Lighting4.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting4::CLighting4(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
   :m_keyword("event"), m_rssi("rssi")
{
   m_rssi.set(0);

   m_subType = deviceParameters.get<unsigned char>("subType");
   m_id = deviceParameters.get<unsigned int>("id");

   Init(context);
}

CLighting4::CLighting4(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_keyword("id"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeLighting4, GET_RBUF_STRUCT_SIZE(LIGHTING4), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.LIGHTING4.subtype;
   m_id = rbuf.LIGHTING4.cmd1 << 16 | rbuf.LIGHTING4.cmd2 << 8 | rbuf.LIGHTING4.cmd3;
   m_rssi.set(NormalizeRssiLevel(rbuf.LIGHTING4.rssi));

   Init(context);
}

CLighting4::~CLighting4()
{
}

void CLighting4::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeLighting4);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_keyword);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

const shared::communication::CByteBuffer CLighting4::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.LIGHTING4);

   rbuf.LIGHTING4.packetlength = ENCODE_PACKET_LENGTH(LIGHTING4);
   rbuf.LIGHTING4.packettype = pTypeLighting4;
   rbuf.LIGHTING4.subtype = m_subType;
   rbuf.LIGHTING4.seqnbr = seqNumberProvider->next();
   rbuf.LIGHTING4.cmd1 = (unsigned char) (0xFF & (m_id >> 16));
   rbuf.LIGHTING4.cmd2 = (unsigned char) (0xFF & (m_id >> 8));
   rbuf.LIGHTING4.cmd3 = (unsigned char) (0xFF & m_id);
   unsigned short pulse = 1400;  // See RFXCom specification
   rbuf.LIGHTING4.pulseHigh = (unsigned char) (0xFF & (pulse >> 8));
   rbuf.LIGHTING4.pulseLow  = (unsigned char) (0xFF & pulse);
   rbuf.LIGHTING4.rssi = 0;
   rbuf.LIGHTING4.filler = 0;

   return shared::communication::CByteBuffer((BYTE*)&rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING4));
}

void CLighting4::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_keyword);
   context->historizeData(m_deviceName, m_rssi);
}

void CLighting4::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CLighting4::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypePT2262   : ssModel << "PT2262"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages