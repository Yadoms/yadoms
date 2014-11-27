#include "stdafx.h"
#include "Lighting1.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting1::CLighting1(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(command);
   m_rssi.set(0);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_houseCode = deviceDetails.get<unsigned char>("houseCode");
   m_unitCode = deviceDetails.get<unsigned char>("unitCode");
 
   Init(context);
}

CLighting1::CLighting1(boost::shared_ptr<yApi::IYadomsApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(false);
   m_rssi.set(0);

   m_subType = subType;
   switch(m_subType)
   {
   case sTypeX10        :
   case sTypeARC        :
   case sTypeAB400D     :
   case sTypeWaveman    :
   case sTypeEMW200     :
   case sTypeIMPULS     :
   case sTypeRisingSun  :
   case sTypePhilips    :
   case sTypeEnergenie  :
   case sTypeEnergenie5 :
   case sTypeGDR2       :
      break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }

   m_houseCode = (unsigned char) manuallyDeviceCreationConfiguration.get<char>("houseCode");
   m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

   Init(context);
}

CLighting1::CLighting1(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_state("state"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeLighting1, GET_RBUF_STRUCT_SIZE(LIGHTING1), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.LIGHTING1.subtype;
   m_houseCode = rbuf.LIGHTING1.housecode;
   m_unitCode = rbuf.LIGHTING1.unitcode;
   m_state.set(fromProtocolState(rbuf.LIGHTING1.cmnd));
   m_rssi.set(NormalizeRssiLevel(rbuf.LIGHTING1.rssi));

   Init(context);
}

CLighting1::~CLighting1()
{
}

void CLighting1::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeLighting1);
      details.set("subType", m_subType);
      details.set("houseCode", m_houseCode);
      details.set("unitCode", m_unitCode);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_state);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CLighting1::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.LIGHTING1);

   rbuf.LIGHTING1.packetlength = ENCODE_PACKET_LENGTH(LIGHTING1);
   rbuf.LIGHTING1.packettype = pTypeLighting1;
   rbuf.LIGHTING1.subtype = m_subType;
   rbuf.LIGHTING1.seqnbr = seqNumberProvider->next();
   rbuf.LIGHTING1.housecode = m_houseCode;
   rbuf.LIGHTING1.unitcode = m_unitCode;
   rbuf.LIGHTING1.cmnd = toProtocolState(m_state);
   rbuf.LIGHTING1.rssi = 0;
   rbuf.LIGHTING1.filler = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING1));
}

void CLighting1::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_state);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CLighting1::getDeviceName() const
{
   return m_deviceName;
}                     

void CLighting1::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (char)m_houseCode << "." << (unsigned int)m_unitCode;
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

unsigned char CLighting1::toProtocolState(const yApi::historization::CSwitch& switchState)
{
   return switchState.get() ? light1_sOn : light1_sOff;
}

bool CLighting1::fromProtocolState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case light1_sOn:
   case light1_sAllOn:
      return true;

   case light1_sOff:
   case light1_sAllOff:
      return false;

   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
   }
}

} // namespace rfxcomMessages