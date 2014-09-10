#include "stdafx.h"
#include "Lighting4.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/commands/Switch.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting4::CLighting4(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
{
   m_subType = deviceParameters.get<unsigned char>("subType");
   m_id = deviceParameters.get<unsigned int>("id");
   m_rssi = 0;

   buildDeviceModel();
   buildDeviceName();
}

CLighting4::CLighting4(const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
{
   CheckReceivedMessage(rbuf, pTypeLighting4, GET_RBUF_STRUCT_SIZE(LIGHTING4), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.LIGHTING4.subtype;
   m_id = rbuf.LIGHTING4.cmd1 << 16 | rbuf.LIGHTING4.cmd2 << 8 | rbuf.LIGHTING4.cmd3;
   m_rssi = rbuf.LIGHTING4.rssi;

   buildDeviceModel();
   buildDeviceName();
}

CLighting4::~CLighting4()
{
}

const CByteBuffer CLighting4::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
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

   return CByteBuffer((BYTE*)&rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING4));
}

void CLighting4::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   if (!context->deviceExists(m_deviceName))
   {
      context->declareDevice(m_deviceName, m_deviceModel);
      context->declareKeyword(m_deviceName, "cmdId", yApi::CStandardCapacities::Switch);
      context->declareKeyword(m_deviceName, "rssi", yApi::CStandardCapacities::Rssi);
   }

   context->historizeData(m_deviceName, "cmdId", boost::lexical_cast<std::string>(m_id));
   context->historizeData(m_deviceName, "rssi", m_rssi);
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