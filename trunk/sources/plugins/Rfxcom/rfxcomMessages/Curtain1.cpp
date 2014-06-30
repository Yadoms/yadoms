#include "stdafx.h"
#include "Curtain1.h"
#include <shared/plugin/yadomsApi/commands/Curtain.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CCurtain1::CCurtain1(const std::string& command, const shared::CDataContainer& deviceParameters)
{
   m_subType = deviceParameters.get<unsigned char>("subType");
   m_houseCode = deviceParameters.get<unsigned char>("houseCode");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");
   m_state = toProtocolState(command);
}

CCurtain1::CCurtain1(const RBUF& buffer)
{
   // Some integrity controls
   if (buffer.CURTAIN1.packetlength != CURTAIN1_size)
      throw shared::exception::CInvalidParameter("CURTAIN1 size");
   if (buffer.CURTAIN1.packettype != pTypeCurtain)
      throw shared::exception::CInvalidParameter("CURTAIN1 packettype");

   m_subType = buffer.CURTAIN1.subtype;
   m_houseCode = buffer.CURTAIN1.housecode;
   m_unitCode = buffer.CURTAIN1.unitcode;
   m_state = buffer.CURTAIN1.cmnd;
   //TODO attendre le .h v6.19, a-t-on quelque chose à extraire de buffer.CURTAIN1.filler (voir doc) ?
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
   buffer.CURTAIN1.filler = 0;//TODO attendre le .h v6.19, il y a des choses à mettre dans cet octet (voir doc)

   return CByteBuffer((BYTE*)&buffer, CURTAIN1_size);
}

void CCurtain1::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_subType << "." << m_houseCode << "." << m_unitCode;
   std::string deviceName(ssdeviceName.str());

   context->historizeData(deviceName, "state", toYadomsState(m_state));
}

unsigned char CCurtain1::toProtocolState(const std::string& yadomsState)
{
   yApi::commands::CCurtain cmd(yadomsState);
   switch (cmd.get())
   {
   case yApi::commands::CCurtain::kOpen: return curtain_sOpen;
   case yApi::commands::CCurtain::kClose: return curtain_sClose;
   case yApi::commands::CCurtain::kStop: return curtain_sStop;
   default:
      BOOST_ASSERT_MSG(false, "Unsupported value");
      throw shared::exception::CInvalidParameter(yadomsState);
   }
}

std::string CCurtain1::toYadomsState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case curtain_sOpen: return yApi::commands::CCurtain(yApi::commands::CCurtain::kOpen).format(); break;
   case curtain_sClose: return yApi::commands::CCurtain(yApi::commands::CCurtain::kClose).format(); break;
   case curtain_sStop: return yApi::commands::CCurtain(yApi::commands::CCurtain::kStop).format(); break;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
      break;
   }
}

} // namespace rfxcomMessages