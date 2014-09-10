#include "stdafx.h"
#include "Lighting6.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/commands/Switch.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting6::CLighting6(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
{
   m_subType = deviceParameters.get<unsigned char>("subType");
   m_id = deviceParameters.get<unsigned short>("id");
   m_groupCode = deviceParameters.get<unsigned char>("groupCode");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");
   m_state = toProtocolState(command);
   m_rssi = 0;

   buildDeviceModel();
   buildDeviceName();
}

CLighting6::CLighting6(const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
{
   CheckReceivedMessage(rbuf, pTypeLighting6, GET_RBUF_STRUCT_SIZE(LIGHTING6), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.LIGHTING6.subtype;
   m_id = (rbuf.LIGHTING6.id1 << 8) | rbuf.LIGHTING6.id2;
   m_groupCode = rbuf.LIGHTING6.groupcode;
   m_unitCode = rbuf.LIGHTING6.unitcode;
   m_state = rbuf.LIGHTING6.cmnd;
   m_rssi = rbuf.LIGHTING6.rssi * 100 / 0x0F;

   buildDeviceModel();
   buildDeviceName();
}

CLighting6::~CLighting6()
{
}

const CByteBuffer CLighting6::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.LIGHTING6);

   rbuf.LIGHTING6.packetlength = ENCODE_PACKET_LENGTH(LIGHTING6);
   rbuf.LIGHTING6.packettype = pTypeLighting6;
   rbuf.LIGHTING6.subtype = m_subType;
   rbuf.LIGHTING6.seqnbr = seqNumberProvider->next();
   rbuf.LIGHTING6.id1 = (unsigned char)((m_id & 0xFF00) >> 8);
   rbuf.LIGHTING6.id2 = (unsigned char)(m_id & 0xFF);
   rbuf.LIGHTING6.groupcode = m_groupCode;
   rbuf.LIGHTING6.unitcode = m_unitCode;
   rbuf.LIGHTING6.cmnd = m_state;
   rbuf.LIGHTING6.cmndseqnbr = seqNumberProvider->last() % 4;
   rbuf.LIGHTING6.seqnbr2 = 0;
   rbuf.LIGHTING6.rssi = 0;
   rbuf.LIGHTING6.filler = 0;

   return CByteBuffer((BYTE*)&rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING6));
}

void CLighting6::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;//TODO faire partout
      details.set("type", pTypeLighting6);
      details.set("subType", m_subType);
      details.set("id", m_id);
      details.set("groupCode", m_groupCode);
      details.set("unitCode", m_unitCode);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());
      context->declareKeyword(m_deviceName, "state", yApi::CStandardCapacities::Switch);
      context->declareKeyword(m_deviceName, "rssi", yApi::CStandardCapacities::Rssi);
   }

   context->historizeData(m_deviceName, "state", toYadomsState(m_state));
   context->historizeData(m_deviceName, "rssi", m_rssi);
}

void CLighting6::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (char)m_groupCode << "." << (unsigned int)m_unitCode << "." << (unsigned int)m_id;
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

unsigned char CLighting6::toProtocolState(const shared::CDataContainer& yadomsState)
{
   yApi::commands::CSwitch cmd(yadomsState);
   return cmd.getState()() == yApi::commands::CSwitch::EState::kOff ? light6_sOff : light6_sOn;
}

std::string CLighting6::toYadomsState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case light6_sOn: return yApi::commands::CSwitch(yApi::commands::CSwitch::EState::kOn).format(); break;
   case light6_sOff: return yApi::commands::CSwitch(yApi::commands::CSwitch::EState::kOff).format(); break;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
      break;
   }
}

} // namespace rfxcomMessages