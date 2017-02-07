#include "stdafx.h"
#include "BinaryFrame.h"

#define IS_PACKED
#include "../rfplayerApi/usb_frame_api.h"

#include <shared/Log.h>
#include <shared/exception/NotSupported.hpp>

#include <shared/plugin/yPluginApi/historization/Rssi.h>
#include <shared/plugin/yPluginApi/historization/Power.h>
#include <shared/plugin/yPluginApi/historization/Energy.h>
#include <shared/plugin/yPluginApi/historization/BatteryLevel.h>
#include <shared/plugin/yPluginApi/historization/SignalPower.h>
#include <shared/plugin/yPluginApi/historization/Alarm.h>
#include <shared/plugin/yPluginApi/historization/Tamper.h>

#include "../specificHistorizers/Type0State.h"
#include "../specificHistorizers/Type1State.h"
#include "../specificHistorizers/Type2KeyCode.h"

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
         out << "Binary : REGULAR " << std::endl;

         if (pFrame->header.dataFlag == 0)
            out << "    " << "433 MHz" << std::endl;
         if (pFrame->header.dataFlag == 1)
            out << "    " << "866 MHz" << std::endl;

         out << "    " << "RfLevel : " << (int)pFrame->header.rfLevel << " dBm" << std::endl;
         out << "    " << "FloorNoise : " << (int)pFrame->header.floorNoise << " dBm" << std::endl;
         out << "    " << "RfQuality : " << (int)pFrame->header.rfQuality << "/10" << std::endl;

         switch (pFrame->header.protocol)
         {
         case RECEIVED_PROTOCOL_UNDEFINED: out << "    " << "protocol : RECEIVED_PROTOCOL_UNDEFINED    " << std::endl;  break;
         case RECEIVED_PROTOCOL_X10: out << "    " << "protocol : X10    " << std::endl;  break;
         case RECEIVED_PROTOCOL_VISONIC: out << "    " << "protocol : VISONIC" << std::endl;  break;
         case RECEIVED_PROTOCOL_BLYSS: out << "    " << "protocol : BLYSS  " << std::endl;  break;
         case RECEIVED_PROTOCOL_CHACON: out << "    " << "protocol : CHACON " << std::endl;  break;
         case RECEIVED_PROTOCOL_OREGON: out << "    " << "protocol : OREGON " << std::endl;  break;
         case RECEIVED_PROTOCOL_DOMIA: out << "    " << "protocol : DOMIA  " << std::endl;  break;
         case RECEIVED_PROTOCOL_OWL: out << "    " << "protocol : OWL    " << std::endl;  break;
         case RECEIVED_PROTOCOL_X2D: out << "    " << "protocol : X2D    " << std::endl;  break;
         case RECEIVED_PROTOCOL_RFY: out << "    " << "protocol : RFY    " << std::endl;  break;
         case RECEIVED_PROTOCOL_KD101: out << "    " << "protocol : KD101  " << std::endl;  break;
         case RECEIVED_PROTOCOL_PARROT: out << "    " << "protocol : PARROT " << std::endl;  break;
         case RECEIVED_PROTOCOL_DIGIMAX: out << "    " << "protocol : DIGIMAX" << std::endl;  break;
         }

         switch (pFrame->header.infoType)
         {
         case INFOS_TYPE0:
            out << "    " << " InfoType : INFOS_TYPE0" << std::endl;

            out << "    " << " Used by X10 / DOMIA LITE protocol / PARROT" << std::endl;
            out << "    " << " ID=" << (boost::format("0x%0X%") % pFrame->infos.type0.id).str() << std::endl;
            out << "    " << " ID HOME=" << (char)(0x30 + ((pFrame->infos.type0.id & 0x00F0) >> 4)) << std::endl;
            out << "    " << " ID DEVICE=" << (pFrame->infos.type0.id & 0x000F) << std::endl;

            switch (pFrame->infos.type0.subtype)
            {
            case 0: out << "    " << " STATE= OFF" << std::endl; break;
            case 1: out << "    " << " STATE= ON" << std::endl; break;
            case 2: out << "    " << " STATE= BRIGHT" << std::endl; break;
            case 3: out << "    " << " STATE= DIM" << std::endl; break;
            case 4: out << "    " << " STATE= All_OFF" << std::endl; break;
            case 5: out << "    " << " STATE= ALL_ON" << std::endl; break;
            }

            break;
         case INFOS_TYPE1:
            out << "    " << " InfoType : INFOS_TYPE1" << std::endl;
            break;
         case INFOS_TYPE2:
            out << "    " << " InfoType : INFOS_TYPE2" << std::endl;
            break;
         case INFOS_TYPE3:
            out << "    " << " InfoType : INFOS_TYPE3" << std::endl;
            break;
         case INFOS_TYPE4:
            out << "    " << " InfoType : INFOS_TYPE4" << std::endl;
            break;
         case INFOS_TYPE5:
            out << "    " << " InfoType : INFOS_TYPE5" << std::endl;
            break;
         case INFOS_TYPE6:
            out << "    " << " InfoType : INFOS_TYPE6" << std::endl;
            break;
         case INFOS_TYPE7:
            out << "    " << " InfoType : INFOS_TYPE7" << std::endl;
            break;
         case INFOS_TYPE8:
            out << "    " << " InfoType : INFOS_TYPE8" << std::endl;


            out << "    " << " Used by OWL ( Energy/power sensors)" << std::endl;
            if (pFrame->infos.type8.subtype == 0)
            {
               out << "    " << " REGULAR frame" << std::endl;
               switch (pFrame->infos.type8.idPHY)
               {
               case 0: out << "    " << " CM119/CM160" << std::endl; break;
               case 1: out << "    " << " CM130" << std::endl; break;
               case 2: out << "    " << " CM180" << std::endl; break;
               case 3: out << "    " << " CM180i" << std::endl; break;
               }
               out << "    " << " Adr_Channel=" << pFrame->infos.type8.idChannel << std::endl;
               out << "    " << " Channel=" << (pFrame->infos.type8.idChannel >> 4) << std::endl;

               out << "    " << " Battery=" << ((pFrame->infos.type8.qualifier & 0x0001) == 0 ? "OK" : "Low") << std::endl;
               out << "    " << ((pFrame->infos.type8.qualifier & 0x0002) == 0 ? " Only the total instantaneous Power is given" : " Power on each input 1, 2, 3 are added (CM180i only).") << std::endl;

               out << "    " << " Energy=" << (pFrame->infos.type8.energyLsb + (pFrame->infos.type8.energyMsb << 8)) << " Wh" << std::endl;
               out << "    " << " Power=" << pFrame->infos.type8.power << " W" << std::endl;
               out << "    " << " P1=" << pFrame->infos.type8.powerI1 << " W" << std::endl;
               out << "    " << " P2=" << pFrame->infos.type8.powerI2 << " W" << std::endl;
               out << "    " << " P3=" << pFrame->infos.type8.powerI3 << " W" << std::endl;
            }
            else
               out << "    " << " NOT REGULAR frame" << std::endl;

            break;
         case INFOS_TYPE9:
            out << "    " << " InfoType : INFOS_TYPE9" << std::endl;
            break;
         case INFOS_TYPE10:
            out << "    " << " InfoType : INFOS_TYPE10" << std::endl;
            break;
         case INFOS_TYPE11:
            out << " InfoType : INFOS_TYPE11" << std::endl;
            break;
         case INFOS_TYPE12:
            out << " InfoType : INFOS_TYPE12" << std::endl;
            break;

         }

         break;

      case RFLINK_INCOMING_BINARY_USB_FRAME_TYPE:
         out << "Binary : RFLINK . Not decoded" << std::endl;
         break;
      }
   }

  
   void CBinaryFrame::historizeData(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      buildDeviceInfo();

      if (m_deviceName.empty())
         throw new shared::exception::CNotSupported("The binary frame content is not supported or dont contains reference to any device identifier");

      // Create device and keywords if needed
      api->deviceExists(m_deviceName);

      //ensure keywords are created (in case of multiple frames giving different keywords)
      api->declareDevice(m_deviceName, m_deviceModel, m_keywords, m_deviceDetails);

      //historize data
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
      {
         if (pFrame->header.dataFlag == 0)
            m_deviceDetails.set("frequency", 433);
         if (pFrame->header.dataFlag == 1)
            m_deviceDetails.set("frequency", 866);


         auto rssiKeyword = boost::make_shared<yApi::historization::CRssi>("rfQuality");
         rssiKeyword->set((int)pFrame->header.rfQuality* 10); //rfQuality is [1;10], RSSI [10;100]
         m_keywords.push_back(rssiKeyword);

         auto rfLevelKeyword = boost::make_shared<yApi::historization::CSignalPower>("rfLevel");
         rfLevelKeyword->set((int)pFrame->header.rfLevel);
         m_keywords.push_back(rfLevelKeyword);

         auto floorNoiseKeyword = boost::make_shared<yApi::historization::CSignalPower>("floorNoise");
         floorNoiseKeyword->set((int)pFrame->header.floorNoise);
         m_keywords.push_back(floorNoiseKeyword);

         m_deviceDetails.set("protocol", pFrame->header.protocol);


         switch (pFrame->header.infoType)
         {
         case INFOS_TYPE0:
         {
            char houseCode = (char)((pFrame->infos.type0.id & 0x00F0) >> 4) + 0x30;
            unsigned char device = (unsigned char)(pFrame->infos.type0.id & 0x000F);
            m_deviceName = (boost::format("%1%%2%") % houseCode % device).str();
            m_deviceModel = "X10 / DomiaLight";
            m_deviceDetails.set("id", pFrame->infos.type0.id);

            auto stateKeyword = boost::make_shared<specificHistorizers::CType0State>("state");
            specificHistorizers::EType0StateValues state((int)pFrame->infos.type0.subtype);
            stateKeyword->set(state);
            m_keywords.push_back(stateKeyword);

            break;
         }

            
         case INFOS_TYPE1:
         {
            unsigned int device = (pFrame->infos.type1.idMsb << 16) + pFrame->infos.type1.idLsb;
            m_deviceName = (boost::format("%1%") % device).str();
            m_deviceModel = "X10 / CHACON / KD101 / BLYSS";
            m_deviceDetails.set("id", device);

            auto stateKeyword = boost::make_shared<specificHistorizers::CType1State>("state");
            specificHistorizers::EType1StateValues state((int)pFrame->infos.type1.subtype);
            stateKeyword->set(state);
            m_keywords.push_back(stateKeyword);

            break;
         }

         case INFOS_TYPE2:
         {
            unsigned int device = (pFrame->infos.type2.idMsb << 16) + pFrame->infos.type2.idLsb;
            m_deviceName = (boost::format("%1%") % (device)).str();

            if (pFrame->infos.type2.subtype == 0)
            {
               //detector/sensor( PowerCode device)
               m_deviceModel = "Detector/sendsor";

               if (pFrame->infos.type2.qualifier & 0x0008 != 0)
               {
                  //this is a supervision frame (understood as ping frame with visonic central)
               }
               else
               {
                  auto batteryLevelKeyword = boost::make_shared<yApi::historization::CBatteryLevel>("battery");
                  batteryLevelKeyword->set((pFrame->infos.type2.qualifier & 0x0004) == 0 ? 100 : 0);
                  m_keywords.push_back(batteryLevelKeyword);

                  auto tamperKeyword = boost::make_shared<yApi::historization::CTamper>("tamper");
                  tamperKeyword->set((pFrame->infos.type2.qualifier & 0x0001) != 0);
                  m_keywords.push_back(tamperKeyword);

                  auto alarmKeyword = boost::make_shared<yApi::historization::CAlarm>("alarm");
                  alarmKeyword->set((pFrame->infos.type2.qualifier & 0x0002) != 0);
                  m_keywords.push_back(alarmKeyword);
               }
            }
            else
            {
               //remote control(CodeSecure device)
               m_deviceModel = "Remote Control";

               auto keyCodeKeyword = boost::make_shared<specificHistorizers::CType2KeyCode>("keyCode");
               keyCodeKeyword->set(specificHistorizers::EType2KeyCodeValues((int)pFrame->infos.type2.subtype));
               m_keywords.push_back(keyCodeKeyword);

            }
            break;
         }
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
         {
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
               m_deviceName = (boost::format("%1%") % (pFrame->infos.type8.idChannel >> 4)).str();
               m_deviceDetails.set("adr_channel", pFrame->infos.type8.idChannel);

               auto batteryLevelKeyword = boost::make_shared<yApi::historization::CBatteryLevel>("battery");
               batteryLevelKeyword->set((pFrame->infos.type8.qualifier & 0x0001) == 0 ? 100 : 0);
               m_keywords.push_back(batteryLevelKeyword);


               //out << "    " << ((pFrame->infos.type8.qualifier & 0x0002) == 0 ? " Only the total instantaneous Power is given" : " Power on each input 1, 2, 3 are added (CM180i only).");

               auto energyKeyword = boost::make_shared<yApi::historization::CEnergy>("energy");
               energyKeyword->set((pFrame->infos.type8.energyLsb + (pFrame->infos.type8.energyMsb << 8)));
               m_keywords.push_back(energyKeyword);

               auto totalPowerKeyword = boost::make_shared<yApi::historization::CPower>("power");
               totalPowerKeyword->set(pFrame->infos.type8.power);
               m_keywords.push_back(totalPowerKeyword);

               if ((pFrame->infos.type8.qualifier & 0x0002) != 0)
               {
                  auto power1Keyword = boost::make_shared<yApi::historization::CPower>("line1");
                  auto power2Keyword = boost::make_shared<yApi::historization::CPower>("line2");
                  auto power3Keyword = boost::make_shared<yApi::historization::CPower>("line3");

                  power1Keyword->set(pFrame->infos.type8.powerI1);
                  power2Keyword->set(pFrame->infos.type8.powerI2);
                  power3Keyword->set(pFrame->infos.type8.powerI3);
                  m_keywords.push_back(power1Keyword);
                  m_keywords.push_back(power2Keyword);
                  m_keywords.push_back(power3Keyword);

               }
            }
            break;
         }
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
      }
      case RFLINK_INCOMING_BINARY_USB_FRAME_TYPE:
         break;
      }
   }

} //namespace frames

