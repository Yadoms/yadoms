#include "stdafx.h"
#include "Curtain1.h"
#include <shared/plugin/yadomsApi/commands/Curtain.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CCurtain1::CCurtain1(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
{
   m_subType = deviceParameters.get<unsigned char>("subType");
   m_houseCode = deviceParameters.get<unsigned char>("houseCode");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");
   m_state = toProtocolState(command);

   buildDeviceModel();
   buildDeviceName();
}

CCurtain1::CCurtain1(const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
{
   CheckReceivedMessage(rbuf, pTypeCurtain, GET_RBUF_STRUCT_SIZE(CURTAIN1), DONT_CHECK_SEQUENCE_NUMBER);
   
   m_subType = rbuf.CURTAIN1.subtype;
   m_houseCode = rbuf.CURTAIN1.housecode;
   m_unitCode = rbuf.CURTAIN1.unitcode;
   m_state = rbuf.CURTAIN1.cmnd;

   buildDeviceModel();
   buildDeviceName();
}

CCurtain1::~CCurtain1()
{
}

const CByteBuffer CCurtain1::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF buffer;
   MEMCLEAR(buffer.CURTAIN1);

   buffer.CURTAIN1.packetlength = ENCODE_PACKET_LENGTH(CURTAIN1);
   buffer.CURTAIN1.packettype = pTypeCurtain;
   buffer.CURTAIN1.subtype = m_subType;
   buffer.CURTAIN1.seqnbr = seqNumberProvider->next();
   buffer.CURTAIN1.housecode = m_houseCode;
   buffer.CURTAIN1.unitcode = m_unitCode;
   buffer.CURTAIN1.cmnd = m_state;
   buffer.CURTAIN1.filler = 0;

   return CByteBuffer((BYTE*)&buffer, GET_RBUF_STRUCT_SIZE(CURTAIN1));
}

void CCurtain1::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_subType << "." << m_houseCode << "." << m_unitCode;
   std::string deviceName(ssdeviceName.str());

   context->historizeData(deviceName, "state", toYadomsState(m_state));
}

void CCurtain1::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (char)m_houseCode << "." << (unsigned int)m_unitCode;
   m_deviceName = ssdeviceName.str();
}

void CCurtain1::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeHarrison: ssModel << "Harrison"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

unsigned char CCurtain1::toProtocolState(const shared::CDataContainer& yadomsState)
{
   yApi::commands::CCurtain cmd(yadomsState);
   switch (cmd.get()())
   {
   case yApi::commands::CCurtain::ECommand::kOpen: return curtain_sOpen;
   case yApi::commands::CCurtain::ECommand::kClose: return curtain_sClose;
   case yApi::commands::CCurtain::ECommand::kStop: return curtain_sStop;
   default:
      BOOST_ASSERT_MSG(false, "Unsupported value");
      throw shared::exception::CInvalidParameter(yadomsState.serialize());
   }
}

std::string CCurtain1::toYadomsState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case curtain_sOpen: return yApi::commands::CCurtain(yApi::commands::CCurtain::ECommand::kOpen).format(); break;
   case curtain_sClose: return yApi::commands::CCurtain(yApi::commands::CCurtain::ECommand::kClose).format(); break;
   case curtain_sStop: return yApi::commands::CCurtain(yApi::commands::CCurtain::ECommand::kStop).format(); break;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
      break;
   }
}

} // namespace rfxcomMessages