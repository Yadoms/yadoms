#include "stdafx.h"
#include "Lighting6.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
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

   buildDeviceName();
   buildDeviceModel();
}

CLighting6::CLighting6(const RBUF& buffer)
{
   // Some integrity controls
   if (buffer.LIGHTING6.packetlength != LIGHTING6_size)
      throw shared::exception::CInvalidParameter("LIGHTING6 size");
   if (buffer.LIGHTING6.packettype != pTypeLighting6)
      throw shared::exception::CInvalidParameter("LIGHTING6 packettype");

   m_subType = buffer.LIGHTING6.subtype;
   m_id = buffer.LIGHTING6.id1 | (buffer.LIGHTING6.id2 << 8);
   m_groupCode = buffer.LIGHTING6.groupcode;
   m_unitCode = buffer.LIGHTING6.unitcode;
   m_state = buffer.LIGHTING6.cmnd;
   m_rssi = buffer.LIGHTING6.rssi * 100 / 0x0F;

   buildDeviceName();
   buildDeviceModel();
}

CLighting6::~CLighting6()
{
}

const CByteBuffer CLighting6::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF buffer;
   MEMCLEAR(buffer.LIGHTING6);

   buffer.LIGHTING6.packetlength = ENCODE_PACKET_LENGTH(LIGHTING6);
   buffer.LIGHTING6.packettype = pTypeLighting6;
   buffer.LIGHTING6.subtype = m_subType;
   buffer.LIGHTING6.seqnbr = seqNumberProvider->next();
   buffer.LIGHTING6.id1 = (unsigned char)((m_id & 0xFF00) >> 8);
   buffer.LIGHTING6.id2 = (unsigned char)(m_id & 0xFF);
   buffer.LIGHTING6.groupcode = m_groupCode;
   buffer.LIGHTING6.unitcode = m_unitCode;
   buffer.LIGHTING6.cmnd = m_state;
   buffer.LIGHTING6.cmndseqnbr = seqNumberProvider->last() % 4;
   buffer.LIGHTING6.seqnbr2 = 0;
   buffer.LIGHTING6.rssi = 0;
   buffer.LIGHTING1.filler = 0;

   return CByteBuffer((BYTE*)&buffer, LIGHTING6_size);
}

void CLighting6::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   if (!context->deviceExists(m_deviceName))
   {
      context->declareDevice(m_deviceName, m_deviceModel);
      context->declareKeyword(m_deviceName, "state", yApi::CStandardCapacities::Switch);
      context->declareKeyword(m_deviceName, "rssi", yApi::CStandardCapacities::Rssi);
   }

   context->historizeData(m_deviceName, "state", toYadomsState(m_state));
   context->historizeData(m_deviceName, "rssi", m_rssi);
}

void CLighting6::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_subType << "." << m_id << "." << m_groupCode << "." << m_unitCode;
   m_deviceName = ssdeviceName.str();
}

void CLighting6::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeBlyss: ssModel << "Blyss"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
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