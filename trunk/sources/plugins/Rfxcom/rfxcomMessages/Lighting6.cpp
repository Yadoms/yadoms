#include "stdafx.h"
#include "Lighting6.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting6::CLighting6(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(command);
   m_rssi.set(0);

   m_subType = deviceParameters.get<unsigned char>("subType");
   m_id = deviceParameters.get<unsigned short>("id");
   m_groupCode = deviceParameters.get<unsigned char>("groupCode");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");

   Init(context);
}

CLighting6::CLighting6(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_state("state"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeLighting6, GET_RBUF_STRUCT_SIZE(LIGHTING6), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.LIGHTING6.subtype;
   m_id = (rbuf.LIGHTING6.id1 << 8) | rbuf.LIGHTING6.id2;
   m_groupCode = rbuf.LIGHTING6.groupcode;
   m_unitCode = rbuf.LIGHTING6.unitcode;
   m_state.set(fromProtocolState(rbuf.LIGHTING6.cmnd));
   m_rssi.set(rbuf.LIGHTING6.rssi * 100 / 0x0F);

   Init(context);
}

CLighting6::~CLighting6()
{
}

void CLighting6::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeLighting6);
      details.set("subType", m_subType);
      details.set("id", m_id);
      details.set("groupCode", m_groupCode);
      details.set("unitCode", m_unitCode);

      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_state);
      context->declareKeyword(m_deviceName, m_rssi);
   }
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
   rbuf.LIGHTING6.cmnd = toProtocolState(m_state);
   rbuf.LIGHTING6.cmndseqnbr = seqNumberProvider->last() % 4;
   rbuf.LIGHTING6.seqnbr2 = 0;
   rbuf.LIGHTING6.rssi = 0;
   rbuf.LIGHTING6.filler = 0;

   return CByteBuffer((BYTE*)&rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING6));
}

void CLighting6::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_state);
   context->historizeData(m_deviceName, m_rssi);
}

void CLighting6::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (char)m_groupCode << "." << (unsigned int)m_id << "." << (unsigned int)m_unitCode;
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

unsigned char CLighting6::toProtocolState(const yApi::commands::CSwitch& switchState)
{
   return switchState.isOn() ? light6_sOn : light6_sOff;
}

bool CLighting6::fromProtocolState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case light6_sOn: return true;
   case light6_sOff: return false;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
   }
}

} // namespace rfxcomMessages