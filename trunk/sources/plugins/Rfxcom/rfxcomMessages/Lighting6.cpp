#include "stdafx.h"
#include "Lighting6.h"
#include <shared/plugin/yadomsApi/StandardValues.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting6::CLighting6(const std::string& command, const boost::property_tree::ptree& deviceParameters, boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider)
{
   unsigned char id = deviceParameters.get<unsigned char>("id");
   unsigned char groupCode = deviceParameters.get<unsigned char>("groupCode");
   unsigned char unitCode = deviceParameters.get<unsigned char>("unitCode");

   m_buffer.LIGHTING6.packetlength = sizeof(m_buffer.LIGHTING6) - sizeof(m_buffer.LIGHTING6.packetlength);
   m_buffer.LIGHTING6.packettype = pTypeLighting6;
   m_buffer.LIGHTING6.subtype = sTypeBlyss;
   m_buffer.LIGHTING6.seqnbr = seqNumberProvider->getNext();
   m_buffer.LIGHTING6.id1 = (unsigned char)((id & 0xFF00) >> 8);
   m_buffer.LIGHTING6.id2 = (unsigned char)(id & 0xFF);
   m_buffer.LIGHTING6.groupcode = groupCode;
   m_buffer.LIGHTING6.unitcode = unitCode;
   m_buffer.LIGHTING6.cmnd = toLighting6Command(command);
   m_buffer.LIGHTING6.cmndseqnbr = 0;//TODO voir spec
   m_buffer.LIGHTING6.seqnbr2 = 0;//TODO voir spec
   m_buffer.LIGHTING6.rssi = 0;
   m_buffer.LIGHTING6.filler = 0;
}

CLighting6::~CLighting6()
{
}

const boost::asio::const_buffer CLighting6::getBuffer() const
{
   return boost::asio::const_buffer(&m_buffer, sizeof(m_buffer.LIGHTING6));
}

unsigned char CLighting6::toLighting6Command(const std::string& yadomsCommand) const
{
   static const std::map<std::string, unsigned char> yadomsCommands = boost::assign::map_list_of
      (yApi::CStandardValues::Off, light6_sOn         )
      (yApi::CStandardValues::On , light6_sOff        )
      (""                        , light6_sGroupOn    )  //TODO d�finir les autres valeurs : � priori utilis�es qu'en entr�e (dim, bright, chime) ou non utilis�es par Yadoms (alloff, allon)
      (""                        , light6_sGroupOff   );   std::map<std::string, unsigned char>::const_iterator itcommand = yadomsCommands.find(yadomsCommand);   if (itcommand == yadomsCommands.end())      throw shared::exception::CInvalidParameter(yadomsCommand);
   return itcommand->second;
}

} // namespace rfxcomMessages