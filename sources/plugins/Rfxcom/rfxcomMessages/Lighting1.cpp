#include "stdafx.h"
#include "Lighting1.h"
#include <shared/plugin/yadomsApi/commands/Switch.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting1::CLighting1(const std::string& command, const shared::CDataContainer& deviceParameters)
{
   m_subType = deviceParameters.get<unsigned char>("subType");
   m_houseCode = deviceParameters.get<unsigned char>("houseCode");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");
   m_state = toProtocolState(command);
   m_rssi = 0;
}

CLighting1::CLighting1(const RBUF& buffer)
{
   // Some integrity controls
   if (buffer.LIGHTING1.packetlength != LIGHTING1_size)
      throw shared::exception::CInvalidParameter("LIGHTING1 size");
   if (buffer.LIGHTING1.packettype != pTypeLighting1)
      throw shared::exception::CInvalidParameter("LIGHTING1 packettype");

   m_subType = buffer.LIGHTING1.subtype;
   m_houseCode = buffer.LIGHTING1.housecode;
   m_unitCode = buffer.LIGHTING1.unitcode;
   m_state = buffer.LIGHTING1.cmnd;
   m_rssi = buffer.LIGHTING1.rssi;
}

CLighting1::~CLighting1()
{
}

const CByteBuffer CLighting1::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF buffer;
   MEMCLEAR(buffer.LIGHTING1);

   buffer.LIGHTING1.packetlength = (BYTE)LIGHTING1_size - sizeof(buffer.LIGHTING1.packetlength);
   buffer.LIGHTING1.packettype = pTypeLighting1;
   buffer.LIGHTING1.subtype = m_subType;
   buffer.LIGHTING1.seqnbr = seqNumberProvider->next();
   buffer.LIGHTING1.housecode = m_houseCode;
   buffer.LIGHTING1.unitcode = m_unitCode;
   buffer.LIGHTING1.cmnd = m_state;
   buffer.LIGHTING1.rssi = 0;
   buffer.LIGHTING1.filler = 0;

   return CByteBuffer((BYTE*)&buffer, LIGHTING1_size);
}

void CLighting1::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_subType << "." << m_houseCode << "." << m_unitCode;
   std::string deviceName(ssdeviceName.str());

   context->historizeData(deviceName, "state", toYadomsState(m_state));
   context->historizeData(deviceName, "rssi", m_rssi);
}

unsigned char CLighting1::toProtocolState(const std::string& yadomsState)
{
   yApi::commands::CSwitch cmd(yadomsState);
   return cmd.getState() == yApi::commands::CSwitch::kOff ? light1_sOff : light1_sOn;
}

std::string CLighting1::toYadomsState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case light1_sOn: return yApi::commands::CSwitch(yApi::commands::CSwitch::kOn).format(); break;
   case light1_sOff: return yApi::commands::CSwitch(yApi::commands::CSwitch::kOff).format(); break;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
      break;
   }
}

} // namespace rfxcomMessages