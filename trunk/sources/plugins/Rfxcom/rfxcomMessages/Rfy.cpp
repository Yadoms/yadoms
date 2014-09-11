#include "stdafx.h"
#include "Rfy.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/commands/Curtain.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CRfy::CRfy(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
{
   m_subType = deviceParameters.get<unsigned char>("subType");
   m_id = deviceParameters.get<unsigned int>("id");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");
   m_state = toProtocolState(command);
   m_rssi = 0;

   buildDeviceModel();
   buildDeviceName();
}

CRfy::CRfy(const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
{
   CheckReceivedMessage(rbuf, pTypeRFY, GET_RBUF_STRUCT_SIZE(RFY), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.RFY.subtype;
   m_id = (rbuf.RFY.id1 << 16) | (rbuf.RFY.id2 << 8) | rbuf.RFY.id3;
   m_unitCode = rbuf.RFY.unitcode;
   m_state = rbuf.RFY.cmnd;
   m_rssi = rbuf.RFY.rssi * 100 / 0x0F;

   buildDeviceModel();
   buildDeviceName();
}

CRfy::~CRfy()
{
}

const CByteBuffer CRfy::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.RFY);

   rbuf.RFY.packetlength = ENCODE_PACKET_LENGTH(RFY);
   rbuf.RFY.packettype = pTypeRFY;
   rbuf.RFY.subtype = m_subType;
   rbuf.RFY.seqnbr = seqNumberProvider->next();
   rbuf.RFY.id1 = (unsigned char)((m_id & 0xFF0000) >> 16);
   rbuf.RFY.id2 = (unsigned char)((m_id & 0xFF00) >> 8);
   rbuf.RFY.id3 = (unsigned char)(m_id & 0xFF);
   rbuf.RFY.unitcode = m_unitCode;
   rbuf.RFY.cmnd = m_state;
   rbuf.RFY.rfu1 = 0;
   rbuf.RFY.rfu2 = 0;
   rbuf.RFY.rfu3 = 0;
   rbuf.RFY.rssi = 0;
   rbuf.LIGHTING1.filler = 0;

   return CByteBuffer((BYTE*)&rbuf, GET_RBUF_STRUCT_SIZE(RFY));
}

void CRfy::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeRFY);
      details.set("subType", m_subType);
      details.set("id", m_id);
      details.set("unitCode", m_unitCode);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, "state", yApi::CStandardCapacities::Switch);
      context->declareKeyword(m_deviceName, "rssi", yApi::CStandardCapacities::Rssi);
   }

   context->historizeData(m_deviceName, "state", toYadomsState(m_state));
   context->historizeData(m_deviceName, "rssi", m_rssi);
}

void CRfy::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (unsigned int)m_unitCode << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CRfy::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeRFY: ssModel << "Somfy"; break;
   case sTypeRFYext: ssModel << "SomfyExt"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }
   
   m_deviceModel = ssModel.str();
}

unsigned char CRfy::toProtocolState(const shared::CDataContainer& yadomsState)
{
   yApi::commands::CCurtain cmd(yadomsState);
   switch (cmd.getCommand()())
   {
   case yApi::commands::CCurtain::ECommand::kOpen: return rfy_sUp;
   case yApi::commands::CCurtain::ECommand::kClose: return rfy_sDown;
   case yApi::commands::CCurtain::ECommand::kStop: return rfy_sStop;
   default:
      BOOST_ASSERT_MSG(false, "Unsupported value");
      throw shared::exception::CInvalidParameter(yadomsState.serialize());
   }
}

std::string CRfy::toYadomsState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case rfy_sUp: return yApi::commands::CCurtain(yApi::commands::CCurtain::ECommand::kOpen).format(); break;
   case rfy_sDown: return yApi::commands::CCurtain(yApi::commands::CCurtain::ECommand::kClose).format(); break;
   case rfy_sStop: return yApi::commands::CCurtain(yApi::commands::CCurtain::ECommand::kStop).format(); break;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
      break;
   }
}

} // namespace rfxcomMessages