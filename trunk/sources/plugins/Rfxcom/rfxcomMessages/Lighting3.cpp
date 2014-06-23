#include "stdafx.h"
#include "Lighting3.h"
#include <shared/plugin/yadomsApi/commands/Switch.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting3::CLighting3(const std::string& command, const shared::CDataContainer& deviceParameters, boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider)
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
   yApi::commands::CSwitch cmd(yadomsCommand);
   switch(cmd.getState())
   {
   case yApi::commands::CSwitch::kOff: return light3_sOff;
   case yApi::commands::CSwitch::kOn: return light3_sOn;
   case yApi::commands::CSwitch::kDim:
      {
         switch (cmd.getDimLevel() / 10)  // getDimLevel returns value from 0 to 100
         {
         case 0: return light3_sOff;
         case 1: return light3_sLevel1;
         case 2: return light3_sLevel2;
         case 3: return light3_sLevel3;
         case 4: return light3_sLevel4;
         case 5: return light3_sLevel5;
         case 6: return light3_sLevel6;
         case 7: return light3_sLevel7;
         case 8: return light3_sLevel8;
         case 9: return light3_sLevel9;
         default: return light3_sOn;
         }
         break;
      }
   default: return light3_sOff;
   }
}

} // namespace rfxcomMessages