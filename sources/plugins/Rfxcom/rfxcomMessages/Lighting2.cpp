#include "stdafx.h"
#include "Lighting2.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/commands/Switch.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting2::CLighting2(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
{
   m_subType = deviceParameters.get<unsigned char>("subType");
   m_houseCode = deviceParameters.get<unsigned char>("houseCode");
   m_id = deviceParameters.get<unsigned int>("id");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");
   toProtocolState(command, m_state, m_level);
   m_rssi = 0;

   buildDeviceName();
   buildDeviceModel();
}

CLighting2::CLighting2(const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
{
   CheckReceivedMessage(rbuf, pTypeLighting2, LIGHTING2_size, DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.LIGHTING2.subtype;

   switch (m_subType)
   {
   case sTypeAC :
   case sTypeHEU :
   case sTypeANSLUT :
      m_houseCode = 0;
      m_id = rbuf.LIGHTING2.id1 << 24 | rbuf.LIGHTING2.id2 << 16 | rbuf.LIGHTING2.id3 << 8 | rbuf.LIGHTING2.id4;
      break;
   case 0x03 :    // No constant : described in the spec but not in the .h file     // TODO voir si un .h plus récent existe
      m_houseCode = rbuf.LIGHTING2.id1;
      m_id = rbuf.LIGHTING2.id2 << 16 | rbuf.LIGHTING2.id3 << 8 | rbuf.LIGHTING2.id4;
      break;
   }
   m_unitCode = rbuf.LIGHTING2.unitcode;
   m_state = rbuf.LIGHTING2.cmnd;
   m_rssi = rbuf.LIGHTING2.rssi * 100 / 0x0F;

   buildDeviceName();
   buildDeviceModel();
}

CLighting2::~CLighting2()
{
}

const CByteBuffer CLighting2::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.LIGHTING2);

   rbuf.LIGHTING2.packetlength = ENCODE_PACKET_LENGTH(LIGHTING2);
   rbuf.LIGHTING2.packettype = pTypeLighting2;
   rbuf.LIGHTING2.subtype = m_subType;
   rbuf.LIGHTING2.seqnbr = seqNumberProvider->next();
   switch (m_subType)
   {
   case sTypeAC :
   case sTypeHEU :
   case sTypeANSLUT :
      rbuf.LIGHTING2.id1 = (unsigned char) (0x0F & (m_id >> 24));
      rbuf.LIGHTING2.id2 = (unsigned char) (0x0F & (m_id >> 16));
      rbuf.LIGHTING2.id3 = (unsigned char) (0x0F & (m_id >> 8));
      rbuf.LIGHTING2.id4 = (unsigned char) (0x0F & m_id);
      break;
   case 0x03 :    // No constant : described in the spec but not in the .h file     // TODO voir si un .h plus récent existe
      rbuf.LIGHTING2.id1 = m_houseCode;
      rbuf.LIGHTING2.id2 = (unsigned char) (0x0F & (m_id >> 16));
      rbuf.LIGHTING2.id3 = (unsigned char) (0x0F & (m_id >> 8));
      rbuf.LIGHTING2.id4 = (unsigned char) (0x0F & m_id);
      break;
   }
   rbuf.LIGHTING2.unitcode = m_unitCode;
   rbuf.LIGHTING2.cmnd = m_state;
   rbuf.LIGHTING2.rssi = 0;
   rbuf.LIGHTING2.filler = 0;

   return CByteBuffer((BYTE*)&rbuf, LIGHTING2_size);
}

void CLighting2::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
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

void CLighting2::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_houseCode << "." << (unsigned int)m_unitCode;
   m_deviceName = ssdeviceName.str();
}

void CLighting2::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeX10        : ssModel << "X10 lighting"; break;
   case sTypeARC        : ssModel << "ARC"; break;
   case sTypeAB400D     : ssModel << "ELRO AB400D (Flamingo)"; break;
   case sTypeWaveman    : ssModel << "Waveman"; break;
   case sTypeEMW200     : ssModel << "Chacon EMW200"; break;
   case sTypeIMPULS     : ssModel << "IMPULS"; break;
   case sTypeRisingSun  : ssModel << "RisingSun"; break;
   case sTypePhilips    : ssModel << "Philips SBC"; break;
   case sTypeEnergenie  : ssModel << "Energenie ENER010"; break;
   case sTypeEnergenie5 : ssModel << "Energenie 5-gang"; break;
   case sTypeGDR2       : ssModel << "COCO GDR2-2000R"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

void CLighting2::toProtocolState(const shared::CDataContainer& yadomsState, unsigned char& state, unsigned char& level)
{
   yApi::commands::CSwitch cmd(yadomsState);
   level = 0;
   switch(cmd.State())
   {
   case yApi::commands::CSwitch::EState::kOff : state = light2_sOff; break;
   case yApi::commands::CSwitch::EState::kOn  : state = light2_sOn; break;
   case yApi::commands::CSwitch::EState::kDim :
      state = light2_sSetLevel;
      level = (unsigned char) (cmd.DimLevel() * 0x0F / 100);   // getDimLevel returns value from 0 to 100
      break;
   default: throw shared::exception::CInvalidParameter("state");
   }
}

std::string CLighting2::toYadomsState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case light1_sOn: return yApi::commands::CSwitch(yApi::commands::CSwitch::EState::kOn).format(); break;
   case light1_sOff: return yApi::commands::CSwitch(yApi::commands::CSwitch::EState::kOff).format(); break;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
      break;
   }
}

} // namespace rfxcomMessages