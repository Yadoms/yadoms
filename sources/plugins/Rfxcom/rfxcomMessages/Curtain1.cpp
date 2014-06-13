#include "stdafx.h"
#include "Curtain1.h"
#include <shared/plugin/yadomsApi/commands/Curtain.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CCurtain1::CCurtain1(const std::string& command, const boost::property_tree::ptree& deviceParameters, boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider)
{
   unsigned char houseCode = deviceParameters.get<unsigned char>("houseCode");
   unsigned char unitCode = deviceParameters.get<unsigned char>("unitCode");

   m_buffer.CURTAIN1.packetlength = sizeof(m_buffer.CURTAIN1) - sizeof(m_buffer.CURTAIN1.packetlength);
   m_buffer.CURTAIN1.packettype = pTypeCurtain;
   m_buffer.CURTAIN1.subtype = sTypeHarrison;
   m_buffer.CURTAIN1.seqnbr = seqNumberProvider->getNext();
   m_buffer.CURTAIN1.housecode = houseCode;
   m_buffer.CURTAIN1.unitcode = unitCode;
   m_buffer.CURTAIN1.cmnd = toCurtain1Command(command);
   m_buffer.CURTAIN1.filler = 0;//TODO attendre le .h v6.19, il y a des choses à mettre dans cet octet (voir doc)
}

CCurtain1::~CCurtain1()
{
}

const boost::asio::const_buffer CCurtain1::getBuffer() const
{
   return boost::asio::const_buffer(&m_buffer, sizeof(m_buffer.CURTAIN1));
}

unsigned char CCurtain1::toCurtain1Command(const std::string& yadomsCommand) const
{
   yApi::commands::CCurtain cmd(yadomsCommand);
   switch (cmd.get())
   {
   case yApi::commands::CCurtain::kOpen: return curtain_sOpen;
   case yApi::commands::CCurtain::kClose: return curtain_sClose;
   case yApi::commands::CCurtain::kStop: return curtain_sStop;
   default:
      BOOST_ASSERT_MSG(false, "Unsupported value");
      throw shared::exception::CInvalidParameter(yadomsCommand);
   }
}

} // namespace rfxcomMessages