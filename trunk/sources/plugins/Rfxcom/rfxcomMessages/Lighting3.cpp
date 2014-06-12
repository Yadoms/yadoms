#include "stdafx.h"
#include "Lighting3.h"
#include <shared/exception/InvalidParameter.hpp>

namespace rfxcomMessages
{

CLighting3::CLighting3(const std::string& command, const boost::property_tree::ptree& deviceParameters, boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider)
{
   unsigned char system = deviceParameters.get<unsigned char>("system");
   unsigned short channel = deviceParameters.get<unsigned short>("channel");

   m_buffer.LIGHTING3.packetlength = sizeof(m_buffer.LIGHTING3) - sizeof(m_buffer.LIGHTING3.packetlength);
   m_buffer.LIGHTING3.packettype = pTypeLighting3;
   m_buffer.LIGHTING3.subtype = sTypeKoppla;
   m_buffer.LIGHTING3.seqnbr = seqNumberProvider->getNext();
   m_buffer.LIGHTING3.system = system;
   m_buffer.LIGHTING3.channel8_1 = (unsigned char)(channel & 0xFF);
   m_buffer.LIGHTING3.channel10_9 = (unsigned char)((channel & 0xFF00) >> 8);
   m_buffer.LIGHTING3.cmnd = toLighting3Command(command);
   m_buffer.LIGHTING3.rssi = 0;
   m_buffer.LIGHTING3.filler = 0;
}

CLighting3::~CLighting3()
{
}

const boost::asio::const_buffer CLighting3::getBuffer() const
{
   return boost::asio::const_buffer(&m_buffer, sizeof(m_buffer.LIGHTING3));
}

unsigned char CLighting3::toLighting3Command(const std::string& yadomsCommand) const
{
   //TODO à finir (gérer les niveaux, etc...)
   static const std::map<std::string, unsigned char> yadomsCommands = boost::assign::map_list_of
      (""   , light3_sBright  )  //TODO définir des commandes standard dans Yadoms ?
      (""   , light3_sDim     )
      ("on" , light3_sOn      )  //TODO définir les autres valeurs : à priori utilisées qu'en entrée (dim, bright, chime) ou non utilisées par Yadoms (alloff, allon)
      (""   , light3_sLevel1  )
      (""   , light3_sLevel2  )
      (""   , light3_sLevel3  )
      (""   , light3_sLevel4  )      (""   , light3_sLevel5  )
      (""   , light3_sLevel6  )
      (""   , light3_sLevel7  )
      (""   , light3_sLevel8  )
      (""   , light3_sLevel9  )
      ("off", light3_sOff     )
      (""   , light3_sProgram );   std::map<std::string, unsigned char>::const_iterator itcommand = yadomsCommands.find(yadomsCommand);   if (itcommand == yadomsCommands.end())      throw shared::exception::CInvalidParameter(yadomsCommand);
   return itcommand->second;
}

} // namespace rfxcomMessages