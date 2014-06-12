#include "stdafx.h"
#include "Curtain1.h"
#include <shared/exception/InvalidParameter.hpp>

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
   m_buffer.CURTAIN1.filler = 0;//TODO attendre le .h v6.19, il y a des choses � mettre dans cet octet (voir doc)
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
   static const std::map<std::string, unsigned char> yadomsCommands = boost::assign::map_list_of
      ("on" , curtain_sOpen      )  //TODO d�finir des commandes standard dans Yadoms ?
      ("off", curtain_sClose     )
      (""   , curtain_sStop      )  //TODO d�finir les autres valeurs : � priori utilis�es qu'en entr�e (dim, bright, chime) ou non utilis�es par Yadoms (alloff, allon)
      (""   , curtain_sProgram   );   std::map<std::string, unsigned char>::const_iterator itcommand = yadomsCommands.find(yadomsCommand);   if (itcommand == yadomsCommands.end())      throw shared::exception::CInvalidParameter(yadomsCommand);
   return itcommand->second;
}

} // namespace rfxcomMessages