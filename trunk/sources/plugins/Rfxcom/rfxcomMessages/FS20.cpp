#include "stdafx.h"
#include "FS20.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;


#define FHT8V_SYNCRHONISE_NOW          0x0
#define FHT8V_OPEN_VALVE               0x1
#define FHT8V_CLOSE_VALVE              0x2
#define FHT8V_OPEN_VALVE_AT_LEVEL      0x6
#define FHT8V_RELATIVE_OFFSET          0x8
#define FHT8V_DECALCIFICATION_CYCLE    0xA
#define FHT8V_SYNCHRONISATION_ACTIVE   0xC
#define FHT8V_TEST                     0xE
#define FHT8V_PAIR_VALVLE              0xF

#define FHT80_SENSOR_OPENED            0x1
#define FHT80_SENSOR_CLOSED            0x2


namespace rfxcomMessages
{

CFS20::CFS20(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceDetails)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(command);
   m_rssi.set(0);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_houseCode = deviceDetails.get<std::string>("houseCode");
   m_groupAddress = deviceDetails.get<std::string>("groupAddress");
   m_subAddress = deviceDetails.get<std::string>("subAddress");

   Init(context);
}

CFS20::CFS20(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_state("state"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeFS20, GET_RBUF_STRUCT_SIZE(FS20), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.FS20.subtype;

   // See RFXtrx SDK specifications to address decode/encode
   std::ostringstream ssHouseCode;
   ssHouseCode << ((rbuf.FS20.hc1 >> 6) & 0x03) + 1;
   ssHouseCode << ((rbuf.FS20.hc1 >> 4) & 0x03) + 1;
   ssHouseCode << ((rbuf.FS20.hc1 >> 2) & 0x03) + 1;
   ssHouseCode << ((rbuf.FS20.hc1     ) & 0x03) + 1;
   ssHouseCode << ((rbuf.FS20.hc2 >> 6) & 0x03) + 1;
   ssHouseCode << ((rbuf.FS20.hc2 >> 4) & 0x03) + 1;
   ssHouseCode << ((rbuf.FS20.hc2 >> 2) & 0x03) + 1;
   ssHouseCode << ((rbuf.FS20.hc2     ) & 0x03) + 1;
   m_houseCode = ssHouseCode.str();

   std::ostringstream ssGroupAddress;
   ssGroupAddress << ((rbuf.FS20.addr >> 6) & 0x03) + 1;
   ssGroupAddress << ((rbuf.FS20.addr >> 4) & 0x03) + 1;
   m_groupAddress = ssGroupAddress.str();

   std::ostringstream ssSubAddress;
   ssSubAddress << ((rbuf.FS20.addr >> 2) & 0x03) + 1;
   ssSubAddress << ((rbuf.FS20.addr     ) & 0x03) + 1;
   m_subAddress = ssSubAddress.str();

   switch(m_subType)
   {
   case sTypeFS20:
      {
         static const unsigned int dimMaxIndex = 0x10;
         static const int dimValues[dimMaxIndex + 1] = {0, 6, 12, 19, 25, 31, 37, 44, 50, 56, 62, 69, 75, 81, 87, 94, 100};
         const unsigned int dimIndex = rbuf.FS20.cmd1 & 0x1F;
         if (dimIndex > dimMaxIndex)
         {
            YADOMS_LOG(warning) << "FS20 unsupported command received (" << dimIndex << ")";
            return;
         }
         m_state.set(dimValues[dimIndex]);
         break;
      }
   case sTypeFHT8V:
      {
         const unsigned int cmd = rbuf.FS20.cmd1 & 0x0F;
         const unsigned int level = rbuf.FS20.cmd2;
         switch(cmd)
         {
         case FHT8V_SYNCRHONISE_NOW:
         case FHT8V_OPEN_VALVE_AT_LEVEL:
         case FHT8V_DECALCIFICATION_CYCLE:
            m_state.set((int)(level * 100 / 255));
            break;
         case FHT8V_OPEN_VALVE:
            m_state.set(true);
            break;
         case FHT8V_CLOSE_VALVE:
            m_state.set(false);
            break;
         default:
            YADOMS_LOG(warning) << "FS20/FHT8V unsupported command received (" << cmd << ")";
            return;
         }
         break;
      }
   case sTypeFHT80:
      {
         const unsigned int cmd = rbuf.FS20.cmd1 & 0x0F;
         switch(cmd)
         {
         case FHT80_SENSOR_OPENED:
            m_state.set(true);
            break;
         case FHT80_SENSOR_CLOSED:
            m_state.set(false);
            break;
         default:
            YADOMS_LOG(warning) << "FS20/FHT8V unsupported command received (" << cmd << ")";
            return;
         }
         break;
      }
   default:
      {
         YADOMS_LOG(warning) << "FS20 invalid received message (unknown subType " << m_subType << ")";
         return;
      }
   }

   m_rssi.set(NormalizeRssiLevel(rbuf.FS20.rssi));

   Init(context);
}

CFS20::~CFS20()
{
}

void CFS20::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeFS20);
      details.set("subType", m_subType);
      details.set("houseCode", m_houseCode);
      details.set("groupAddress", m_groupAddress);
      details.set("subAddress", m_subAddress);

      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_state);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CFS20::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.FS20);

   rbuf.FS20.packetlength = ENCODE_PACKET_LENGTH(FS20);
   rbuf.FS20.packettype = pTypeFS20;
   rbuf.FS20.subtype = m_subType;
   rbuf.FS20.seqnbr = seqNumberProvider->next();

   // See RFXtrx SDK specifications to address decode/encode
   if ( !regex_match(m_houseCode    , boost::regex("[1-4]{8}")) ||
        !regex_match(m_groupAddress , boost::regex("[1-4]{2}")) ||
        !regex_match(m_subAddress   , boost::regex("[1-4]{2}")) )
   {
      YADOMS_LOG(warning) << "FS20 encoding : invalid house code(" << m_houseCode << "), group adress(" << m_groupAddress << ") or sub-address (" << m_subAddress << ")";
      throw shared::exception::CInvalidParameter("FS20 houseCode");
   }
   rbuf.FS20.hc1 =
      ((m_houseCode[0] - '1') << 6) |
      ((m_houseCode[1] - '1') << 4) |
      ((m_houseCode[2] - '1') << 2) |
      ((m_houseCode[3] - '1'))     ;
   rbuf.FS20.hc2 =
      ((m_houseCode[4] - '1') << 6) |
      ((m_houseCode[5] - '1') << 4) |
      ((m_houseCode[6] - '1') << 2) |
      ((m_houseCode[7] - '1'))     ;

   rbuf.FS20.addr =
      ((m_groupAddress[0] - '1') << 6) |
      ((m_groupAddress[1] - '1') << 4) |
      ((m_subAddress  [0] - '1') << 2) |
      ((m_subAddress  [1] - '1'))  ;


   switch(m_subType)
   {
   case sTypeFS20:
      {
         if      (m_state.switchLevel() == 0  ) { rbuf.FS20.cmd1 = 0x20 | 0   ; }
         else if (m_state.switchLevel() < 7   ) { rbuf.FS20.cmd1 = 0x20 | 0x01; }
         else if (m_state.switchLevel() < 13  ) { rbuf.FS20.cmd1 = 0x20 | 0x02; }
         else if (m_state.switchLevel() < 19  ) { rbuf.FS20.cmd1 = 0x20 | 0x03; }
         else if (m_state.switchLevel() < 26  ) { rbuf.FS20.cmd1 = 0x20 | 0x04; }
         else if (m_state.switchLevel() < 32  ) { rbuf.FS20.cmd1 = 0x20 | 0x05; }
         else if (m_state.switchLevel() < 38  ) { rbuf.FS20.cmd1 = 0x20 | 0x06; }
         else if (m_state.switchLevel() < 44  ) { rbuf.FS20.cmd1 = 0x20 | 0x07; }
         else if (m_state.switchLevel() < 51  ) { rbuf.FS20.cmd1 = 0x20 | 0x08; }
         else if (m_state.switchLevel() < 57  ) { rbuf.FS20.cmd1 = 0x20 | 0x09; }
         else if (m_state.switchLevel() < 63  ) { rbuf.FS20.cmd1 = 0x20 | 0x0A; }
         else if (m_state.switchLevel() < 69  ) { rbuf.FS20.cmd1 = 0x20 | 0x0B; }
         else if (m_state.switchLevel() < 76  ) { rbuf.FS20.cmd1 = 0x20 | 0x0C; }
         else if (m_state.switchLevel() < 82  ) { rbuf.FS20.cmd1 = 0x20 | 0x0D; }
         else if (m_state.switchLevel() < 88  ) { rbuf.FS20.cmd1 = 0x20 | 0x0E; }
         else if (m_state.switchLevel() < 94  ) { rbuf.FS20.cmd1 = 0x20 | 0x0F; }
         else if (m_state.switchLevel() <= 100) { rbuf.FS20.cmd1 = 0x20 | 0x10; }
         else
         {
            BOOST_ASSERT_MSG(false, "FS20 encoding : invalid switch level");
         }

         rbuf.FS20.cmd2 = 0;
         break;
      }
   case sTypeFHT8V:
      {
         if      (m_state.switchLevel() == 0  )
         {
            rbuf.FS20.cmd1 = 0x30 | FHT8V_CLOSE_VALVE;
            rbuf.FS20.cmd2 = 0;
         }
         else if (m_state.switchLevel() <= 100)
         {
            rbuf.FS20.cmd1 = 0x30 | FHT8V_OPEN_VALVE;
            rbuf.FS20.cmd2 = 0;
         }
         else
         {
            rbuf.FS20.cmd1 = 0x30 | FHT8V_OPEN_VALVE_AT_LEVEL;
            rbuf.FS20.cmd2 = (BYTE)(m_state.switchLevel() * 0xFF / 100);
         }
         break;
      }
   default:
      {
         // Note sTypeFHT80 is read-only
         YADOMS_LOG(warning) << "FS20 invalid received message (unknown subType " << m_subType << ")";
         throw shared::exception::CInvalidParameter("FS20 subType");
      }
   }

   rbuf.FS20.rssi = 0;
   rbuf.FS20.filler = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(FS20));
}

void CFS20::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_state);
   context->historizeData(m_deviceName, m_rssi);
}

void CFS20::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << m_houseCode << "." << m_groupAddress << "." << m_subAddress;
   m_deviceName = ssdeviceName.str();
}

void CFS20::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeFS20: ssModel << "FS20"; break;
   case sTypeFHT8V: ssModel << "FHT8V valve"; break;
   case sTypeFHT80: ssModel << "FHT80 door/window sensor"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }
   
   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages