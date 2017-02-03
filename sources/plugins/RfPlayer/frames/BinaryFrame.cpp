#include "stdafx.h"
#include "BinaryFrame.h"

#define IS_PACKED
#include "../rfplayerApi/usb_frame_api.h"

#include <shared/Log.h>

namespace frames {


   CBinaryFrame::CBinaryFrame(char sourceDest, boost::shared_ptr<shared::communication::CByteBuffer> content)
      :m_sourceDest(sourceDest), m_content(content)
   {
      
   }

   CBinaryFrame::~CBinaryFrame()
   {

   }

   void CBinaryFrame::printToLog(std::ostream & out) const
   {
      REGULAR_INCOMING_RF_TO_BINARY_USB_FRAME * pFrame = (REGULAR_INCOMING_RF_TO_BINARY_USB_FRAME*)m_content->begin();

      switch (pFrame->header.frameType)
      {
      case REGULAR_INCOMING_BINARY_USB_FRAME_TYPE:
         out << "Binary : REGULAR ";

         if (pFrame->header.dataFlag == 0)
            out << "    " << "433 MHz";
         if (pFrame->header.dataFlag == 1)
            out << "    " << "866 MHz";

         out << "    " << "RfLevel : " << (int)pFrame->header.rfLevel << " dBm";
         out << "    " << "FloorNoise : " << (int)pFrame->header.floorNoise << " dBm";
         out << "    " << "RfQuality : " << (int)pFrame->header.rfQuality << "/10";

         switch (pFrame->header.protocol)
         {
         case RECEIVED_PROTOCOL_UNDEFINED: out << "    " << "protocol : RECEIVED_PROTOCOL_UNDEFINED    ";  break;
         case RECEIVED_PROTOCOL_X10: out << "    " << "protocol : X10    ";  break;
         case RECEIVED_PROTOCOL_VISONIC: out << "    " << "protocol : VISONIC";  break;
         case RECEIVED_PROTOCOL_BLYSS: out << "    " << "protocol : BLYSS  ";  break;
         case RECEIVED_PROTOCOL_CHACON: out << "    " << "protocol : CHACON ";  break;
         case RECEIVED_PROTOCOL_OREGON: out << "    " << "protocol : OREGON ";  break;
         case RECEIVED_PROTOCOL_DOMIA: out << "    " << "protocol : DOMIA  ";  break;
         case RECEIVED_PROTOCOL_OWL: out << "    " << "protocol : OWL    ";  break;
         case RECEIVED_PROTOCOL_X2D: out << "    " << "protocol : X2D    ";  break;
         case RECEIVED_PROTOCOL_RFY: out << "    " << "protocol : RFY    ";  break;
         case RECEIVED_PROTOCOL_KD101: out << "    " << "protocol : KD101  ";  break;
         case RECEIVED_PROTOCOL_PARROT: out << "    " << "protocol : PARROT ";  break;
         case RECEIVED_PROTOCOL_DIGIMAX: out << "    " << "protocol : DIGIMAX";  break;
         }

         switch (pFrame->header.infoType)
         {
         case INFOS_TYPE0:
            out << "    " << " InfoType : INFOS_TYPE0";

            out << "    " << " Used by X10 / DOMIA LITE protocol / PARROT";
            out << "    " << " ID=" << (boost::format("0x%0X") % pFrame->infos.type0.id).str();
            out << "    " << " ID HOME=" << (char)(0x30 + ((pFrame->infos.type0.id & 0x00F0) >> 4));
            out << "    " << " ID DEVICE=" << (pFrame->infos.type0.id & 0x000F);

            switch (pFrame->infos.type0.subtype)
            {
            case 0: out << "    " << " STATE= OFF"; break;
            case 1: out << "    " << " STATE= ON"; break;
            case 2: out << "    " << " STATE= BRIGHT"; break;
            case 3: out << "    " << " STATE= DIM"; break;
            case 4: out << "    " << " STATE= All_OFF"; break;
            case 5: out << "    " << " STATE= ALL_ON"; break;
            }

            break;
         case INFOS_TYPE1:
            out << "    " << " InfoType : INFOS_TYPE1";
            break;
         case INFOS_TYPE2:
            out << "    " << " InfoType : INFOS_TYPE2";
            break;
         case INFOS_TYPE3:
            out << "    " << " InfoType : INFOS_TYPE3";
            break;
         case INFOS_TYPE4:
            out << "    " << " InfoType : INFOS_TYPE4";
            break;
         case INFOS_TYPE5:
            out << "    " << " InfoType : INFOS_TYPE5";
            break;
         case INFOS_TYPE6:
            out << "    " << " InfoType : INFOS_TYPE6";
            break;
         case INFOS_TYPE7:
            out << "    " << " InfoType : INFOS_TYPE7";
            break;
         case INFOS_TYPE8:
            out << "    " << " InfoType : INFOS_TYPE8";


            out << "    " << " Used by OWL ( Energy/power sensors)";
            if (pFrame->infos.type8.subtype == 0)
            {
               out << "    " << " REGULAR frame";
               switch (pFrame->infos.type8.idPHY)
               {
               case 0: out << "    " << " CM119/CM160"; break;
               case 1: out << "    " << " CM130"; break;
               case 2: out << "    " << " CM180"; break;
               case 3: out << "    " << " CM180i"; break;
               }
               out << "    " << " Adr_Channel=" << pFrame->infos.type8.idChannel;
               out << "    " << " Channel=" << (pFrame->infos.type8.idChannel >> 4);

               out << "    " << " Battery=" << ((pFrame->infos.type8.qualifier & 0x0001) == 0 ? "OK" : "Low");
               out << "    " << ((pFrame->infos.type8.qualifier & 0x0002) == 0 ? " Only the total instantaneous Power is given" : " Power on each input 1, 2, 3 are added (CM180i only).");

               out << "    " << " Energy=" << (pFrame->infos.type8.energyLsb + (pFrame->infos.type8.energyMsb << 8)) << " Wh";
               out << "    " << " Power=" << pFrame->infos.type8.power << " W";
               out << "    " << " P1=" << pFrame->infos.type8.powerI1 << " W";
               out << "    " << " P2=" << pFrame->infos.type8.powerI2 << " W";
               out << "    " << " P3=" << pFrame->infos.type8.powerI3 << " W";
            }
            else
               out << "    " << " NOT REGULAR frame";

            break;
         case INFOS_TYPE9:
            out << "    " << " InfoType : INFOS_TYPE9";
            break;
         case INFOS_TYPE10:
            out << "    " << " InfoType : INFOS_TYPE10";
            break;
         case INFOS_TYPE11:
            out << " InfoType : INFOS_TYPE11";
            break;
         case INFOS_TYPE12:
            out << " InfoType : INFOS_TYPE12";
            break;

         }

         break;

      case RFLINK_INCOMING_BINARY_USB_FRAME_TYPE:
         out << "Binary : RFLINK . Not decoded";
         break;
      }
   }

   void CBinaryFrame::init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      buildDeviceInfo();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         api->declareDevice(m_deviceName, m_deviceModel, m_keywords, m_deviceDetails);
      }
   }   
   
   void CBinaryFrame::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const char CBinaryFrame::getSourceDest() const
   {
      return m_sourceDest;
   }

   boost::shared_ptr< shared::communication::CByteBuffer > CBinaryFrame::getContent() const
   {
      return m_content;
   }

   void CBinaryFrame::buildDeviceInfo()
   {
      REGULAR_INCOMING_RF_TO_BINARY_USB_FRAME * pFrame = (REGULAR_INCOMING_RF_TO_BINARY_USB_FRAME*)m_content->begin();

      switch (pFrame->header.frameType)
      {
      case REGULAR_INCOMING_BINARY_USB_FRAME_TYPE:
         if (pFrame->header.dataFlag == 0)
            m_deviceDetails.set("frequency", 433);
         if (pFrame->header.dataFlag == 1)
            m_deviceDetails.set("frequency", 866);


         m_rssi = boost::make_shared<yApi::historization::CRssi>("rfQuality");
         m_rssi->set( ((int)pFrame->header.rfQuality-1) * 10); //rfQuality is [1;10], RSSI [0;99]
         m_keywords.push_back(m_rssi);
         //out << "    " << "RfLevel : " << (int)pFrame->header.rfLevel << " dBm";
         //out << "    " << "FloorNoise : " << (int)pFrame->header.floorNoise << " dBm";
         //out << "    " << "RfQuality : " << (int)pFrame->header.rfQuality << "/10";

         m_deviceDetails.set("protocol", pFrame->header.protocol);


         switch (pFrame->header.infoType)
         {
         case INFOS_TYPE0:
            break;
         case INFOS_TYPE1:
            break;
         case INFOS_TYPE2:
            break;
         case INFOS_TYPE3:
            break;
         case INFOS_TYPE4:
            break;
         case INFOS_TYPE5:
            break;
         case INFOS_TYPE6:
            break;
         case INFOS_TYPE7:
            break;
         case INFOS_TYPE8:
            if (pFrame->infos.type8.subtype == 0)
            {
               switch (pFrame->infos.type8.idPHY)
               {
               case 0: 
                  m_deviceModel = "OWL CM119/CM160"; 
                  break;
               case 1:
                  m_deviceModel = "OWL CM130";
                  break;
               case 2:
                  m_deviceModel = "OWL CM180";
                  break;
               case 3:
                  m_deviceModel = "OWL CM180i";
                  break;
               }
               m_deviceName = (boost::format("%1") % (pFrame->infos.type8.idChannel >> 4)).str();
               m_deviceDetails.set("adr_channel", pFrame->infos.type8.idChannel);

               m_batteryLevel = boost::make_shared<yApi::historization::CBatteryLevel>("battery");
               m_batteryLevel->set((pFrame->infos.type8.qualifier & 0x0001) == 0 ? 100 : 0);
               m_keywords.push_back(m_batteryLevel);


               //out << "    " << ((pFrame->infos.type8.qualifier & 0x0002) == 0 ? " Only the total instantaneous Power is given" : " Power on each input 1, 2, 3 are added (CM180i only).");

               m_energy = boost::make_shared<yApi::historization::CEnergy>("energy");
               m_energy->set((pFrame->infos.type8.energyLsb + (pFrame->infos.type8.energyMsb << 8)));
               m_keywords.push_back(m_energy);

               m_totalPower = boost::make_shared<yApi::historization::CPower>("power");
               m_totalPower->set(pFrame->infos.type8.power);
               m_keywords.push_back(m_totalPower);

               if ((pFrame->infos.type8.qualifier & 0x0002) != 0)
               {
                  m_power1 = boost::make_shared<yApi::historization::CPower>("line1");
                  m_power2 = boost::make_shared<yApi::historization::CPower>("line2");
                  m_power3 = boost::make_shared<yApi::historization::CPower>("line3");

                  m_power1->set(pFrame->infos.type8.powerI1);
                  m_power2->set(pFrame->infos.type8.powerI2);
                  m_power3->set(pFrame->infos.type8.powerI3);
                  m_keywords.push_back(m_power1);
                  m_keywords.push_back(m_power2);
                  m_keywords.push_back(m_power3);

               }
            }
            break;
         case INFOS_TYPE9:
            break;
         case INFOS_TYPE10:
            break;
         case INFOS_TYPE11:
            break;
         case INFOS_TYPE12:
            break;

         }

         break;

      case RFLINK_INCOMING_BINARY_USB_FRAME_TYPE:
         break;
      }
   }

   void CBinaryFrame::buildKeywords()
   {

   }




} //namespace frames

