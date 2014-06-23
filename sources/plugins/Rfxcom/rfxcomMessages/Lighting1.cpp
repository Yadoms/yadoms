#include "stdafx.h"
#include "Lighting1.h"
#include <shared/plugin/yadomsApi/commands/Switch.h>


// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting1::CLighting1(const std::string& command, const shared::CDataContainer& deviceParameters, boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider)
{
   unsigned char subType = deviceParameters.get<unsigned char>("subType");
   unsigned char houseCode = deviceParameters.get<unsigned char>("houseCode");
   unsigned char unitCode = deviceParameters.get<unsigned char>("unitCode");

   m_buffer.LIGHTING1.packetlength = sizeof(m_buffer.LIGHTING1) - sizeof(m_buffer.LIGHTING1.packetlength);
   m_buffer.LIGHTING1.packettype = pTypeLighting1;
   m_buffer.LIGHTING1.subtype = subType;
   m_buffer.LIGHTING1.seqnbr = seqNumberProvider->getNext();//TODO : faire un vrai sequence number provider qui incrémente ?
   m_buffer.LIGHTING1.housecode = houseCode;
   m_buffer.LIGHTING1.unitcode = unitCode;
   m_buffer.LIGHTING1.cmnd = toLighting1Command(command);
   m_buffer.LIGHTING1.rssi = 0;
   m_buffer.LIGHTING1.filler = 0;
}

CLighting1::~CLighting1()
{
}

const boost::asio::const_buffer CLighting1::getBuffer() const
{
   return boost::asio::const_buffer(&m_buffer, sizeof(m_buffer.LIGHTING1));
}

unsigned char CLighting1::toLighting1Command(const std::string& yadomsCommand) const
{
   yApi::commands::CSwitch cmd(yadomsCommand);
   return cmd.getState() == yApi::commands::CSwitch::kOff ? light1_sOff : light1_sOn;
}

} // namespace rfxcomMessages