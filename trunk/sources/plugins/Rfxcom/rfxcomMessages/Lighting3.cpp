#include "stdafx.h"
#include "Lighting3.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting3::CLighting3(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(command);
   m_rssi.set(0);

   m_subType = deviceParameters.get<unsigned char>("subType");
   m_system = deviceParameters.get<unsigned char>("system");
   m_channel = deviceParameters.get<unsigned short>("channel");

   Init(context);
}

CLighting3::CLighting3(boost::shared_ptr<yApi::IYadomsApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(false);
   m_rssi.set(0);

   m_subType = subType;
   if (m_subType != sTypeKoppla)
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

   m_system = manuallyDeviceCreationConfiguration.get<unsigned char>("system");
   m_channel = manuallyDeviceCreationConfiguration.get<unsigned short>("channel");

   Init(context);
}

CLighting3::CLighting3(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_state("state"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeLighting3, GET_RBUF_STRUCT_SIZE(LIGHTING3), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.LIGHTING3.subtype;
   m_system = rbuf.LIGHTING3.system;
   m_channel = rbuf.LIGHTING3.channel8_1 & (rbuf.LIGHTING3.channel10_9 << 8);
   m_state.set(fromProtocolState(rbuf.LIGHTING3.cmnd));
   m_rssi.set(NormalizeRssiLevel(rbuf.LIGHTING2.rssi));

   Init(context);
}

CLighting3::~CLighting3()
{
}

void CLighting3::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeLighting3);
      details.set("subType", m_subType);
      details.set("system", m_system);
      details.set("channel", m_channel);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_state);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

const shared::communication::CByteBuffer CLighting3::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.LIGHTING3);

   rbuf.LIGHTING3.packetlength = ENCODE_PACKET_LENGTH(LIGHTING3);
   rbuf.LIGHTING3.packettype = pTypeLighting3;
   rbuf.LIGHTING3.subtype = m_subType;
   rbuf.LIGHTING3.seqnbr = seqNumberProvider->next();
   rbuf.LIGHTING3.system = m_system;
   rbuf.LIGHTING3.channel8_1 = (unsigned char)(m_channel & 0xFF);
   rbuf.LIGHTING3.channel10_9 = (unsigned char)((m_channel & 0xFF00) >> 8);
   rbuf.LIGHTING3.cmnd = toProtocolState(m_state);
   rbuf.LIGHTING3.rssi = 0;
   rbuf.LIGHTING3.filler = 0;

   return shared::communication::CByteBuffer((BYTE*)&rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING3));
}

void CLighting3::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_state);
   context->historizeData(m_deviceName, m_rssi);
}

void CLighting3::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (unsigned int)m_system << "." << (unsigned int)m_channel;
   m_deviceName = ssdeviceName.str();
}

void CLighting3::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeKoppla: ssModel << "Ikea Koppla"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

unsigned char CLighting3::toProtocolState(const yApi::historization::CDimmable& switchState)
{
   switch (switchState.switchLevel() / 10)  // switchLevel returns value from 0 to 100
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
}

int CLighting3::fromProtocolState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case light3_sOff   : return 0;
   case light3_sLevel1: return 10;
   case light3_sLevel2: return 20;
   case light3_sLevel3: return 30;
   case light3_sLevel4: return 40;
   case light3_sLevel5: return 50;
   case light3_sLevel6: return 60;
   case light3_sLevel7: return 70;
   case light3_sLevel8: return 80;
   case light3_sLevel9: return 90;
   case light3_sOn    : return 100;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
      break;
   }
}

} // namespace rfxcomMessages