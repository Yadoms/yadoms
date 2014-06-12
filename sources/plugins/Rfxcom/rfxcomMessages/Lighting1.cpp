#include "stdafx.h"
#include "Lighting1.h"
#include <shared/exception/InvalidParameter.hpp>

namespace rfxcomMessages
{

CLighting1::CLighting1(const std::string& command, const boost::property_tree::ptree& deviceParameters, boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider)
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
   static const std::map<std::string, unsigned char> yadomsCommands = boost::assign::map_list_of
      ("off", light1_sOff      )  //TODO définir des commandes standard dans Yadoms ?
      ("on" , light1_sOn       )
      (""   , light1_sDim      )  //TODO définir les autres valeurs : à priori utilisées qu'en entrée (dim, bright, chime) ou non utilisées par Yadoms (alloff, allon)
      (""   , light1_sBright   )
      (""   , light1_sAllOff   )
      (""   , light1_sAllOn    )
      (""   , light1_sChime    );   std::map<std::string, unsigned char>::const_iterator itcommand = yadomsCommands.find(yadomsCommand);   if (itcommand == yadomsCommands.end())      throw shared::exception::CInvalidParameter(yadomsCommand);
   return itcommand->second;
}

} // namespace rfxcomMessages