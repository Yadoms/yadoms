#include "stdafx.h"
#include "Lighting3.h"
#include <shared/plugin/yadomsApi/commands/Switch.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting3::CLighting3(const std::string& command, const shared::CDataContainer& deviceParameters)
{
   m_subType = deviceParameters.get<unsigned char>("subType");
   m_system = deviceParameters.get<unsigned char>("system");
   m_channel = deviceParameters.get<unsigned short>("channel");
   m_state = toProtocolState(command);
   m_rssi = 0;
}

CLighting3::CLighting3(const RBUF& buffer)
{
   // Some integrity controls
   if (buffer.LIGHTING3.packetlength != LIGHTING3_size)
      throw shared::exception::CInvalidParameter("LIGHTING3 size");
   if (buffer.LIGHTING3.packettype != pTypeLighting3)
      throw shared::exception::CInvalidParameter("LIGHTING3 packettype");

   m_subType = buffer.LIGHTING3.subtype;
   m_system = buffer.LIGHTING3.system;
   m_channel = buffer.LIGHTING3.channel8_1 & (buffer.LIGHTING3.channel10_9 << 8);
   m_state = buffer.LIGHTING3.cmnd;
   m_rssi = buffer.LIGHTING3.rssi;
}

CLighting3::~CLighting3()
{
}

const CByteBuffer CLighting3::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF buffer;
   MEMCLEAR(buffer.LIGHTING3);

   buffer.LIGHTING3.packetlength = (BYTE)LIGHTING3_size - sizeof(buffer.LIGHTING3.packetlength);
   buffer.LIGHTING3.packettype = pTypeLighting3;
   buffer.LIGHTING3.subtype = m_subType;
   buffer.LIGHTING3.seqnbr = seqNumberProvider->next();
   buffer.LIGHTING3.system = m_system;
   buffer.LIGHTING3.channel8_1 = (unsigned char)(m_channel & 0xFF);
   buffer.LIGHTING3.channel10_9 = (unsigned char)((m_channel & 0xFF00) >> 8);
   buffer.LIGHTING3.cmnd = m_state;
   buffer.LIGHTING3.rssi = 0;
   buffer.LIGHTING3.filler = 0;

   return CByteBuffer((BYTE*)&buffer, LIGHTING3_size);
}

void CLighting3::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_subType << "." << m_system << "." << m_channel;
   std::string deviceName(ssdeviceName.str());

   context->historizeData(deviceName, "state", toYadomsState(m_state));
   context->historizeData(deviceName, "rssi", m_rssi);
}

unsigned char CLighting3::toProtocolState(const std::string& yadomsState)
{
   yApi::commands::CSwitch cmd(yadomsState);
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

std::string CLighting3::toYadomsState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case light3_sOn: return yApi::commands::CSwitch(yApi::commands::CSwitch::kOn).format(); break;
   case light3_sOff: return yApi::commands::CSwitch(yApi::commands::CSwitch::kOff).format(); break;
   case light3_sLevel1: return yApi::commands::CSwitch(yApi::commands::CSwitch::kDim, 10).format(); break;
   case light3_sLevel2: return yApi::commands::CSwitch(yApi::commands::CSwitch::kDim, 20).format(); break;
   case light3_sLevel3: return yApi::commands::CSwitch(yApi::commands::CSwitch::kDim, 30).format(); break;
   case light3_sLevel4: return yApi::commands::CSwitch(yApi::commands::CSwitch::kDim, 40).format(); break;
   case light3_sLevel5: return yApi::commands::CSwitch(yApi::commands::CSwitch::kDim, 50).format(); break;
   case light3_sLevel6: return yApi::commands::CSwitch(yApi::commands::CSwitch::kDim, 60).format(); break;
   case light3_sLevel7: return yApi::commands::CSwitch(yApi::commands::CSwitch::kDim, 70).format(); break;
   case light3_sLevel8: return yApi::commands::CSwitch(yApi::commands::CSwitch::kDim, 80).format(); break;
   case light3_sLevel9: return yApi::commands::CSwitch(yApi::commands::CSwitch::kDim, 90).format(); break;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
      break;
   }
}

} // namespace rfxcomMessages