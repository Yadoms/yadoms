#include "stdafx.h"
#include "Lighting2.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting2::CLighting2(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(command);
   m_rssi.set(0);

   m_subType = deviceParameters.get<unsigned char>("subType");
   m_houseCode = deviceParameters.get<unsigned char>("houseCode");
   m_id = deviceParameters.get<unsigned int>("id");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");

   Init(context);
}

CLighting2::CLighting2(boost::shared_ptr<yApi::IYadomsApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(false);
   m_rssi.set(0);

   m_subType = subType;
   switch(m_subType)
   {
   case sTypeAC         :
   case sTypeHEU        :
   case sTypeANSLUT     :
   case sTypeKambrook   :
      break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }

   m_houseCode = manuallyDeviceCreationConfiguration.get<unsigned char>("houseCode");
   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
   m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

   Init(context);
}

CLighting2::CLighting2(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_state("state"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeLighting2, GET_RBUF_STRUCT_SIZE(LIGHTING2), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.LIGHTING2.subtype;

   switch (m_subType)
   {
   case sTypeAC :
   case sTypeHEU :
   case sTypeANSLUT :
      m_houseCode = 0;
      m_id = rbuf.LIGHTING2.id1 << 24 | rbuf.LIGHTING2.id2 << 16 | rbuf.LIGHTING2.id3 << 8 | rbuf.LIGHTING2.id4;
      break;
   case sTypeKambrook :
      m_houseCode = rbuf.LIGHTING2.id1;
      m_id = rbuf.LIGHTING2.id2 << 16 | rbuf.LIGHTING2.id3 << 8 | rbuf.LIGHTING2.id4;
      break;
   }
   m_unitCode = rbuf.LIGHTING2.unitcode;
   m_state.set(fromProtocolState(rbuf.LIGHTING2.cmnd, rbuf.LIGHTING2.level));
   m_rssi.set(NormalizeRssiLevel(rbuf.LIGHTING2.rssi));

   Init(context);
}

CLighting2::~CLighting2()
{
}

void CLighting2::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeLighting2);
      details.set("subType", m_subType);
      details.set("houseCode", m_houseCode);
      details.set("id", m_id);
      details.set("unitCode", m_unitCode);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_state);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

const shared::communication::CByteBuffer CLighting2::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
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
      rbuf.LIGHTING2.id1 = (unsigned char) (0xFF & (m_id >> 24));
      rbuf.LIGHTING2.id2 = (unsigned char) (0xFF & (m_id >> 16));
      rbuf.LIGHTING2.id3 = (unsigned char) (0xFF & (m_id >> 8));
      rbuf.LIGHTING2.id4 = (unsigned char) (0xFF & m_id);
      break;
   case sTypeKambrook :
      rbuf.LIGHTING2.id1 = m_houseCode;
      rbuf.LIGHTING2.id2 = (unsigned char) (0xFF & (m_id >> 16));
      rbuf.LIGHTING2.id3 = (unsigned char) (0xFF & (m_id >> 8));
      rbuf.LIGHTING2.id4 = (unsigned char) (0xFF & m_id);
      break;
   }
   rbuf.LIGHTING2.unitcode = m_unitCode;
   toProtocolState(m_state, rbuf.LIGHTING2.cmnd, rbuf.LIGHTING2.level);
   rbuf.LIGHTING2.rssi = 0;
   rbuf.LIGHTING2.filler = 0;

   return shared::communication::CByteBuffer((BYTE*)&rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING2));
}

void CLighting2::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_state);
   context->historizeData(m_deviceName, m_rssi);
}

void CLighting2::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_houseCode << "." << (unsigned int)m_id << "." << (unsigned int)m_unitCode;
   m_deviceName = ssdeviceName.str();
}

void CLighting2::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeAC         : ssModel << "AC"; break;
   case sTypeHEU        : ssModel << "HomeEasy EU"; break;
   case sTypeANSLUT     : ssModel << "ANSLUT"; break;
   case sTypeKambrook   : ssModel << "Kambrook RF3672 (Australia)"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

void CLighting2::toProtocolState(const yApi::historization::CDimmable& switchState, unsigned char& state, unsigned char& level)
{
   switch(switchState.switchLevel())
   {
   case 0 :
      state = light2_sOff;
      level = 0;
      break;
   case 100 :
      state = light2_sOn;
      level = 0;
      break;
   default :
      state = light2_sSetLevel;
      level = (unsigned char) (switchState.switchLevel() * 0x0F / 100);   // switchLevel returns value from 0 to 100
      break;
   }
}

int CLighting2::fromProtocolState(unsigned char protocolState, unsigned char protocolLevel)
{
   switch(protocolState)
   {
   case light2_sOn: return 100;
   case light2_sOff: return 0;
   case light2_sSetLevel:
      {
         return (protocolLevel * 100 / 0x0F); // level needs to be from 0 to 100
      }
   default:
      {
         BOOST_ASSERT_MSG(false, "Invalid state");
         throw shared::exception::CInvalidParameter("state");
      }
   }
}

} // namespace rfxcomMessages