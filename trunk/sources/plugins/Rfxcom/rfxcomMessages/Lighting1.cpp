#include "stdafx.h"
#include "Lighting1.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/commands/Switch.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting1::CLighting1(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
{
   m_subType = deviceParameters.get<unsigned char>("subType");
   m_houseCode = deviceParameters.get<unsigned char>("houseCode");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");
   m_state = toProtocolState(command);
   m_rssi = 0;

   buildDeviceName();
   buildDeviceModel();
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
   m_rssi = buffer.LIGHTING1.rssi * 100 / 0x0F;

   buildDeviceName();
   buildDeviceModel();
}

CLighting1::~CLighting1()
{
}

const CByteBuffer CLighting1::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF buffer;
   MEMCLEAR(buffer.LIGHTING1);

   buffer.LIGHTING1.packetlength = ENCODE_PACKET_LENGTH(LIGHTING1);
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
   if (!context->deviceExists(m_deviceName))
   {
      context->declareDevice(m_deviceName, m_deviceModel);
      context->declareKeyword(m_deviceName, "state", yApi::CStandardCapacities::Switch);
      context->declareKeyword(m_deviceName, "rssi", yApi::CStandardCapacities::Rssi);
   }

   context->historizeData(m_deviceName, "state", toYadomsState(m_state));
   context->historizeData(m_deviceName, "rssi", m_rssi);
}

void CLighting1::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_subType << "." << m_houseCode << "." << m_unitCode;
   m_deviceName = ssdeviceName.str();
}

void CLighting1::buildDeviceModel()
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

unsigned char CLighting1::toProtocolState(const shared::CDataContainer& yadomsState)
{
   yApi::commands::CSwitch cmd(yadomsState);
   return cmd.getState()() == yApi::commands::EState::kOff ? light1_sOff : light1_sOn;
}

std::string CLighting1::toYadomsState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case light1_sOn: return yApi::commands::CSwitch(yApi::commands::EState::kOn).format(); break;
   case light1_sOff: return yApi::commands::CSwitch(yApi::commands::EState::kOff).format(); break;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
      break;
   }
}

} // namespace rfxcomMessages