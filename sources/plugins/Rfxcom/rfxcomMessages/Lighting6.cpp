#include "stdafx.h"
#include "Lighting6.h"
#include <shared/plugin/yadomsApi/commands/Switch.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting6::CLighting6(const std::string& command, const shared::CDataContainer& deviceParameters)
{
   m_subType = deviceParameters.get<unsigned char>("subType");
   m_id = deviceParameters.get<unsigned short>("id");
   m_groupCode = deviceParameters.get<unsigned char>("groupCode");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");
   m_state = toProtocolState(command);
   m_rssi = 0;
}

CLighting6::CLighting6(const RBUF& buffer)
{
   // Some integrity controls
   if (buffer.LIGHTING6.packetlength != LIGHTING6_size)
      throw shared::exception::CInvalidParameter("LIGHTING6 size");
   if (buffer.LIGHTING6.packettype != pTypeLighting6)
      throw shared::exception::CInvalidParameter("LIGHTING6 packettype");

   m_subType = buffer.LIGHTING6.subtype;
   m_id = buffer.LIGHTING6.id1 & (buffer.LIGHTING6.id2 << 8);
   m_groupCode = buffer.LIGHTING6.groupcode;
   m_unitCode = buffer.LIGHTING6.unitcode;
   m_state = buffer.LIGHTING6.cmnd;
   //TODO voir spec : que faire de : m_buffer.LIGHTING6.cmndseqnbr
   //TODO voir spec : que faire de : m_buffer.LIGHTING6.seqnbr2
   m_rssi = buffer.LIGHTING6.rssi;
}

CLighting6::~CLighting6()
{
}

const CByteBuffer CLighting6::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF buffer;
   MEMCLEAR(buffer.LIGHTING6);

   buffer.LIGHTING6.packetlength = (BYTE)LIGHTING6_size - sizeof(buffer.LIGHTING6.packetlength);
   buffer.LIGHTING6.packettype = pTypeLighting1;
   buffer.LIGHTING6.subtype = m_subType;
   buffer.LIGHTING6.seqnbr = seqNumberProvider->next();
   buffer.LIGHTING6.id1 = (unsigned char)((m_id & 0xFF00) >> 8);
   buffer.LIGHTING6.id2 = (unsigned char)(m_id & 0xFF);
   buffer.LIGHTING6.groupcode = m_groupCode;
   buffer.LIGHTING6.unitcode = m_unitCode;
   buffer.LIGHTING6.cmnd = m_state;
   buffer.LIGHTING6.cmndseqnbr = 0;//TODO voir spec
   buffer.LIGHTING6.seqnbr2 = 0;//TODO voir spec
   buffer.LIGHTING6.rssi = 0;
   buffer.LIGHTING1.filler = 0;

   return CByteBuffer((BYTE*)&buffer, LIGHTING6_size);
}

void CLighting6::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_subType << "." << m_id << "." << m_groupCode << "." << m_unitCode;
   std::string deviceName(ssdeviceName.str());

   context->historizeData(deviceName, "state", toYadomsState(m_state));
   context->historizeData(deviceName, "rssi", m_rssi);
}


unsigned char CLighting6::toProtocolState(const std::string& yadomsState)
{
   yApi::commands::CSwitch cmd(yadomsState);
   return cmd.getState() == yApi::commands::CSwitch::kOff ? light6_sOff : light6_sOn;
}

std::string CLighting6::toYadomsState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case light6_sOn: return yApi::commands::CSwitch(yApi::commands::CSwitch::kOn).format(); break;
   case light6_sOff: return yApi::commands::CSwitch(yApi::commands::CSwitch::kOff).format(); break;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
      break;
   }
}

} // namespace rfxcomMessages