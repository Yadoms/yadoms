#include "stdafx.h"
#include "Lighting5.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
//TODO support à minima : il faudrait gérer l'éclairage led coloré mieux que ça (créer historizer et tout)
// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting5::CLighting5(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(command);
   m_rssi.set(0);

   m_subType = deviceParameters.get<unsigned char>("subType");
   m_id = deviceParameters.get<unsigned int>("id");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");
 
   Init(context);
}

CLighting5::CLighting5(boost::shared_ptr<yApi::IYadomsApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(false);
   m_rssi.set(0);

   m_subType = subType;
   switch(m_subType)
   {
   case sTypeLightwaveRF :
   case sTypeEMW100      :
   case sTypeBBSB        :
   case sTypeMDREMOTE    :
   case sTypeRSL         :
   case sTypeLivolo      :
   case sTypeTRC02       :
      break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }

   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
   m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

   Init(context);
}

CLighting5::CLighting5(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_state("state"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeLighting5, GET_RBUF_STRUCT_SIZE(LIGHTING5), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.LIGHTING5.subtype;
   m_id = rbuf.LIGHTING5.id1 << 16 | rbuf.LIGHTING5.id2 << 8 | rbuf.LIGHTING5.id3;
   m_unitCode = rbuf.LIGHTING5.unitcode;
   m_state.set(fromProtocolState(rbuf.LIGHTING5.cmnd, rbuf.LIGHTING5.level));
   m_rssi.set(NormalizeRssiLevel(rbuf.LIGHTING5.rssi));

   Init(context);
}

CLighting5::~CLighting5()
{
}

void CLighting5::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeLighting5);
      details.set("subType", m_subType);
      details.set("id", m_id);
      details.set("unitCode", m_unitCode);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_state);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

const shared::communication::CByteBuffer CLighting5::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.LIGHTING5);

   rbuf.LIGHTING5.packetlength = ENCODE_PACKET_LENGTH(LIGHTING5);
   rbuf.LIGHTING5.packettype = pTypeLighting5;
   rbuf.LIGHTING5.subtype = m_subType;
   rbuf.LIGHTING5.seqnbr = seqNumberProvider->next();
   rbuf.LIGHTING5.id1 = (unsigned char) (0xFF & (m_id >> 16));
   rbuf.LIGHTING5.id2 = (unsigned char) (0xFF & (m_id >> 8));
   rbuf.LIGHTING5.id3 = (unsigned char) (0xFF & m_id);
   rbuf.LIGHTING5.unitcode = m_unitCode;
   toProtocolState(rbuf.LIGHTING5.cmnd, rbuf.LIGHTING5.level);
   rbuf.LIGHTING5.rssi = 0;
   rbuf.LIGHTING5.filler = 0;

   return shared::communication::CByteBuffer((BYTE*)&rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING5));
}

void CLighting5::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_state);
   context->historizeData(m_deviceName, m_rssi);
}

void CLighting5::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (unsigned int)m_id << "." << (unsigned int)m_unitCode;
   m_deviceName = ssdeviceName.str();
}

void CLighting5::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeLightwaveRF : ssModel << "LightwaveRF, Siemens (AD protocol)"; break;
   case sTypeEMW100      : ssModel << "EMW100 GAO/Everflourish"; break;
   case sTypeBBSB        : ssModel << "BBSB new types"; break;
   case sTypeMDREMOTE    : ssModel << "MDREMOTE LED dimmer"; break;
   case sTypeRSL         : ssModel << "Chacon EMW200"; break;
   case sTypeLivolo      : ssModel << "Livolo"; break;
   case sTypeTRC02       : ssModel << "RGB TRC02"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

void CLighting5::toProtocolState(unsigned char& protocolCmnd, unsigned char& protocolLevel) const
{
   switch(m_subType)
   {
   case sTypeLightwaveRF :
      {
         if      (m_state.switchLevel() ==   0) { protocolCmnd = 0x00; protocolLevel = 0; }
         else if (m_state.switchLevel() == 100) { protocolCmnd = 0x01; protocolLevel = 0; }
         else                                   { protocolCmnd = 0x10; protocolLevel = (unsigned char) (m_state.switchLevel() * 0x1F / 100); }
         break;
      }
   case sTypeEMW100      :
      {
         protocolLevel = 0;
         protocolCmnd = m_state.isOn() ? 0x01 : 0x00;
         break;
      }
   case sTypeBBSB        :
   case sTypeRSL         :
      {
         protocolLevel = 0;
         protocolCmnd = m_state.isOn() ? 0x01 : 0x00;
         break;
      }
   case sTypeMDREMOTE    :
      {
         protocolLevel = 0;
         if      (m_state.switchLevel() ==   0) protocolCmnd = 0x00; // Off
         else if (m_state.switchLevel() <= 37 ) protocolCmnd = 0x06; // 25%
         else if (m_state.switchLevel() <= 75 ) protocolCmnd = 0x05; // 50%
         else                                   protocolCmnd = 0x04; // 100%
         break;
      }
   case sTypeLivolo      :
      {
         protocolLevel = 0;
         protocolCmnd = m_state.isOn() ? 0x01 : 0x00;//TODO inexact, pour faire ON, faut envoyer plusieurs messages (off puis toggle)
         break;
      }
   case sTypeTRC02       :
      {
         protocolLevel = 0;
         protocolCmnd = m_state.isOn() ? 0x01 : 0x00;
         break;
      }
   default:
      {
         BOOST_ASSERT_MSG(false, "Unknown subtype");
         throw shared::exception::CInvalidParameter("subtype");
      }
   }
}

int CLighting5::fromProtocolState(unsigned char protocolCmnd, unsigned char protocolLevel) const
{
   switch(m_subType)
   {
   case sTypeLightwaveRF :
      {
         switch (protocolCmnd)
         {
         case 0x00 :                // Off
            return 0;
         case 0x01 :                // On
            return 100;
         case 0x10 :                // Set level
            return protocolLevel * 100 / 0x1F;
         default:
            {
               BOOST_ASSERT_MSG(false, "Invalid state");
               throw shared::exception::CInvalidParameter("state");
            }
         }
         break;
      }
   case sTypeEMW100      :
      {
         switch (protocolCmnd)
         {
         case 0x00 :                // Off
            return 0;
         case 0x01 :                // On
            return 100;
         default:
            {
               BOOST_ASSERT_MSG(false, "Invalid state");
               throw shared::exception::CInvalidParameter("state");
            }
         }
         break;
      }
   case sTypeBBSB        :
   case sTypeRSL         :
      {
         switch (protocolCmnd)
         {
         case 0x00 :                // Off
         case 0x02 :                // Group Off
            return 0;
         case 0x01 :                // On
         case 0x03 :                // Group On
            return 100;
         default:
            {
               BOOST_ASSERT_MSG(false, "Invalid state");
               throw shared::exception::CInvalidParameter("state");
            }
         }
         break;
      }
   case sTypeMDREMOTE    :
      {
         switch (protocolCmnd)
         {
         case 0x00 :                // Power
            return 0;
         case 0x01 :                // Light
            return 100;
         case 0x04 :                // 100%
            return 100;
         case 0x05 :                // 50%
            return 50;
         case 0x06 :                // 25%
            return 25;
         default:
            {
               BOOST_ASSERT_MSG(false, "Invalid state");
               throw shared::exception::CInvalidParameter("state");
            }
         }
         break;
      }
   case sTypeLivolo      :
      {
         switch (protocolCmnd)
         {
         case 0x00 :                // Group Off
            return 0;
         case 0x01 :                // Toggle On/Off
            return 100;//TODO inexact, pour faire ON, faut envoyer plusieurs messages (off puis toggle)
         default:
            {
               BOOST_ASSERT_MSG(false, "Invalid state");
               throw shared::exception::CInvalidParameter("state");
            }
         }
         break;
      }
   case sTypeTRC02       :
      {
         switch (protocolCmnd)
         {
         case 0x00 :                // Off
            return 0;
         case 0x01 :                // On
            return 100;
         default:
            {
               BOOST_ASSERT_MSG(false, "Invalid state");
               throw shared::exception::CInvalidParameter("state");
            }
         }
         break;
      }
   default:
      {
         BOOST_ASSERT_MSG(false, "Unknown subtype");
         throw shared::exception::CInvalidParameter("subtype");
      }
   }
}

} // namespace rfxcomMessages