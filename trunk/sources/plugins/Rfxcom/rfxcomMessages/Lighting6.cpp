#include "stdafx.h"
#include "Lighting6.h"
#include <shared/exception/InvalidParameter.hpp>

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
      ("off", light6_sOn         )  //TODO définir des commandes standard dans Yadoms ?
      ("on" , light6_sOff        )
      (""   , light6_sGroupOn    )  //TODO définir les autres valeurs : à priori utilisées qu'en entrée (dim, bright, chime) ou non utilisées par Yadoms (alloff, allon)
      (""   , light6_sGroupOff   );   std::map<std::string, unsigned char>::const_iterator itcommand = yadomsCommands.find(yadomsCommand);   if (itcommand == yadomsCommands.end())      throw shared::exception::CInvalidParameter(yadomsCommand);
   return itcommand->second;
}

} // namespace rfxcomMessages