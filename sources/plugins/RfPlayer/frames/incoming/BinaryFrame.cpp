#include "stdafx.h"
#include "BinaryFrame.h"

#define IS_PACKED
#include "../../rfplayerApi/usb_frame_api.h"

#include <shared/Log.h>
#include <shared/exception/NotSupported.hpp>

#include <shared/plugin/yPluginApi/historization/Rssi.h>
#include <shared/plugin/yPluginApi/historization/Power.h>
#include <shared/plugin/yPluginApi/historization/Energy.h>
#include <shared/plugin/yPluginApi/historization/BatteryLevel.h>
#include <shared/plugin/yPluginApi/historization/SignalLevel.h>
#include <shared/plugin/yPluginApi/historization/Temperature.h>
#include <shared/plugin/yPluginApi/historization/Humidity.h>
#include <shared/plugin/yPluginApi/historization/Pressure.h>
#include <shared/plugin/yPluginApi/historization/Speed.h>
#include <shared/plugin/yPluginApi/historization/Direction.h>
#include <shared/plugin/yPluginApi/historization/Uv.h>
#include <shared/plugin/yPluginApi/historization/Rain.h>
#include <shared/plugin/yPluginApi/historization/RainRate.h>
#include <shared/plugin/yPluginApi/historization/Switch.h>

#include <shared/communication/BufferLogger.h>

#include "../../specificHistorizers/incoming/Type0State.h"
#include "../../specificHistorizers/incoming/Type1State.h"
#include "../../specificHistorizers/incoming/Type2KeyCode.h"
#include "../../specificHistorizers/incoming/Type3RemoteCode.h"
#include "../../specificHistorizers/incoming/Type3ShutterCode.h"
#include "../../specificHistorizers/incoming/Type10OperatingMode.h"
#include "../../specificHistorizers/incoming/Type11State.h"

namespace frames {
namespace incoming {


   CBinaryFrame::CBinaryFrame(char sourceDest, boost::shared_ptr<shared::communication::CByteBuffer> content)
      :m_sourceDest(sourceDest), m_content(content)
   {

   }

   CBinaryFrame::~CBinaryFrame()
   {

   }

   void CBinaryFrame::printToLog(std::ostream & out) const
   {
      YADOMS_LOG(debug) << "Received frame:" << std::endl << shared::communication::CBufferLogger::msgToString(*m_content.get());

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

      //ensure keywords are created (in case of multiple frames giving different keywords)
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, m_deviceDetails);

      //historize only if keywords exists
      if (!m_keywords.empty())
      {
         //historize data
         api->historizeData(m_deviceName, m_keywords);
      }
   }

   const char CBinaryFrame::getSourceDest() const
   {
      return m_sourceDest;
   }

   boost::shared_ptr< shared::communication::CByteBuffer > CBinaryFrame::getContent() const
   {
      return m_content;
   }

   void CBinaryFrame::identifyOregonDeviceFromIdPhy(unsigned short idPhy, unsigned short channel)
   {
      switch (idPhy)
      {
      case 0:
         m_deviceModel += " thermometer probe";
         break;
      case 0x1A2D:
         m_deviceModel += " THGR122/228/238/268, THGN122/123/132";
         break;
      case 0xCA2C:
         m_deviceModel += " THGR328";
         break;
      case 0x0ACC:
         m_deviceModel += " RTGR328";
         break;
      case 0xEA4C:
         m_deviceModel += " THC238/268, THWR288,THRN122,THN122/132,AW129/131";
         break;
      case 0x1A3D:
         m_deviceModel += " THGR918/928, THGRN228, THGN50";
         break;
      case 0x5A6D:
         m_deviceModel += " THGR918N";
         break;
      case 0x1A89:
         m_deviceModel += " WGR800";
         break;
      case 0xCA48:
         m_deviceModel += " THWR800";
         break;
      case 0xFA28:
         m_deviceModel += " THGR810, THGN800";
         break;
      case 0x2A19:
         m_deviceModel += " PCR800";
         break;
      case 0xDA78:
         m_deviceModel += " UVN800";
         break;

      }

      m_deviceName = (boost::format("%1%") % (channel >> 8)).str();
      m_deviceDetails.set("adr_channel", channel);
      m_deviceDetails.set("idPhy", idPhy);
   }

   void CBinaryFrame::manageOregonBatteryFromQualifier(unsigned short qualifier)
   {
      auto batteryLevelKeyword = boost::make_shared<yApi::historization::CBatteryLevel>("battery");
      batteryLevelKeyword->set((qualifier & 0x0001) == 0 ? 100 : 0);
      m_keywords.push_back(batteryLevelKeyword);
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
         rssiKeyword->set((int)pFrame->header.rfQuality * 10); //rfQuality is [1;10], RSSI [10;100]
         m_keywords.push_back(rssiKeyword);

         auto rfLevelKeyword = boost::make_shared<yApi::historization::CSignalLevel>("rfLevel");
         rfLevelKeyword->set((int)pFrame->header.rfLevel);
         m_keywords.push_back(rfLevelKeyword);

         auto floorNoiseKeyword = boost::make_shared<yApi::historization::CSignalLevel>("floorNoise");
         floorNoiseKeyword->set((int)pFrame->header.floorNoise);
         m_keywords.push_back(floorNoiseKeyword);

         switch (pFrame->header.protocol)
         {
         default:
         case RECEIVED_PROTOCOL_UNDEFINED: m_deviceModel = "";  break;
         case RECEIVED_PROTOCOL_X10: 
            m_deviceModel = "X10";  
            m_deviceDetails.set("protocol", "x10"); 
            break;
         case RECEIVED_PROTOCOL_VISONIC: 
            m_deviceModel = "VISONIC";  
            m_deviceDetails.set("protocol", "visonic");
            break;
         case RECEIVED_PROTOCOL_BLYSS:
            m_deviceModel = "BLYSS";  
            m_deviceDetails.set("protocol", "blyss");
            break;
         case RECEIVED_PROTOCOL_CHACON: 
            m_deviceModel = "CHACON";  
            m_deviceDetails.set("protocol", "chacon");
            break;
         case RECEIVED_PROTOCOL_OREGON: 
            m_deviceModel = "OREGON";  
            m_deviceDetails.set("protocol", "oregon");
            break;
         case RECEIVED_PROTOCOL_DOMIA:  
            m_deviceModel = "DOMIA";  
            m_deviceDetails.set("protocol", "domia");
            break;
         case RECEIVED_PROTOCOL_OWL:    
            m_deviceModel = "OWL";  
            m_deviceDetails.set("protocol", "owl");
            break;
         case RECEIVED_PROTOCOL_X2D:    
            m_deviceModel = "X2D";  
            m_deviceDetails.set("protocol", "x2d");
            break;
         case RECEIVED_PROTOCOL_RFY:    
            m_deviceModel = "RFY";  
            m_deviceDetails.set("protocol", "rfy");
            break;
         case RECEIVED_PROTOCOL_KD101:  
            m_deviceModel = "KD101";  
            m_deviceDetails.set("protocol", "kd101");
            break;
         case RECEIVED_PROTOCOL_PARROT: 
            m_deviceModel = "PARROT";  
            m_deviceDetails.set("protocol", "parrot");
            break;
         case RECEIVED_PROTOCOL_DIGIMAX:
            m_deviceModel = "DIGIMAX";  
            m_deviceDetails.set("protocol", "digimax");
            break;
         }

         switch (pFrame->header.infoType)
         {
         case INFOS_TYPE0:
         {
            //Used for X10 / DomiaLight
            char houseCode = (char)((pFrame->infos.type0.id & 0x00F0) >> 4) + 0x30;
            unsigned char device = (unsigned char)(pFrame->infos.type0.id & 0x000F);
            m_deviceName = (boost::format("%1%%2%") % houseCode % device).str();
            m_deviceDetails.set("id", pFrame->infos.type0.id);

            auto stateKeyword = boost::make_shared<specificHistorizers::incoming::CType0State>("state");
            specificHistorizers::incoming::EType0StateValues state((int)pFrame->infos.type0.subtype);
            stateKeyword->set(state);
            m_keywords.push_back(stateKeyword);

            break;
         }


         case INFOS_TYPE1:
         {
            unsigned int device = (pFrame->infos.type1.idMsb << 16) + pFrame->infos.type1.idLsb;
            m_deviceName = (boost::format("%1%") % device).str();
            m_deviceDetails.set("id", device);

            auto stateKeyword = boost::make_shared<specificHistorizers::incoming::CType1State>("state");
            specificHistorizers::incoming::EType1StateValues state((int)pFrame->infos.type1.subtype);
            stateKeyword->set(state);
            m_keywords.push_back(stateKeyword);

            break;
         }

         case INFOS_TYPE2:
         {
            //VISONIC alarm systems
            unsigned int device = (pFrame->infos.type2.idMsb << 16) + pFrame->infos.type2.idLsb;
            m_deviceName = (boost::format("%1%") % (device)).str();

            if (pFrame->infos.type2.subtype == 0)
            {
               //detector/sensor( PowerCode device)
               m_deviceModel += " Detector/sendsor";

               if ((pFrame->infos.type2.qualifier & 0x0008) != 0)
               {
                  //this is a supervision frame (understood as ping frame with visonic central)
               }
               else
               {
                  auto batteryLevelKeyword = boost::make_shared<yApi::historization::CBatteryLevel>("battery");
                  batteryLevelKeyword->set((pFrame->infos.type2.qualifier & 0x0004) == 0 ? 100 : 0);
                  m_keywords.push_back(batteryLevelKeyword);

                  auto tamperKeyword = boost::make_shared<yApi::historization::CTamper>("tamper", yApi::EKeywordAccessMode::kGet);
                  tamperKeyword->set((pFrame->infos.type2.qualifier & 0x0001) != 0);
                  m_keywords.push_back(tamperKeyword);

                  auto alarmKeyword = boost::make_shared<yApi::historization::CAlarm>("alarm", yApi::EKeywordAccessMode::kGet);
                  alarmKeyword->set((pFrame->infos.type2.qualifier & 0x0002) != 0);
                  m_keywords.push_back(alarmKeyword);
               }
            }
            else
            {
               //remote control(CodeSecure device)
               m_deviceModel += " Remote Control";

               auto keyCodeKeyword = boost::make_shared<specificHistorizers::incoming::CType2KeyCode>("keyCode");
               keyCodeKeyword->set(specificHistorizers::incoming::EType2KeyCodeValues((int)pFrame->infos.type2.qualifier));
               m_keywords.push_back(keyCodeKeyword);

            }
            break;
         }
         case INFOS_TYPE3:
         {
            //Somfy RTS protocol
            unsigned int device = (pFrame->infos.type3.idMsb << 16) + pFrame->infos.type3.idLsb;
            m_deviceName = (boost::format("%1%") % (device)).str();


            if (pFrame->infos.type3.subtype == 0)
            {
               //shutter device
               m_deviceModel += " Somfy Shutter";

               auto codeKeyword = boost::make_shared<specificHistorizers::incoming::CType3ShutterCode>("shutter");
               codeKeyword->set(specificHistorizers::incoming::EType3ShutterCodeValues((int)pFrame->infos.type3.qualifier & 0x000F));
               m_keywords.push_back(codeKeyword);

            }
            else
            {
               //shutter device
               m_deviceModel += " Somfy portal reomte control";

               auto codeKeyword = boost::make_shared<specificHistorizers::incoming::CType3RemoteCode>("remote");
               codeKeyword->set(specificHistorizers::incoming::EType3RemoteCodeValues((int)pFrame->infos.type3.qualifier & 0x000F));
               m_keywords.push_back(codeKeyword);
            }

            break;
         }
         case INFOS_TYPE4:
         {
            //Used by Scientific Oregon protocol(thermo / hygro sensors)
            identifyOregonDeviceFromIdPhy(pFrame->infos.type4.idPHY, pFrame->infos.type4.idChannel);
            manageOregonBatteryFromQualifier(pFrame->infos.type4.qualifier);

            auto tempKeyword = boost::make_shared<yApi::historization::CTemperature>("temperature", yApi::EKeywordAccessMode::kGet);
            tempKeyword->set(pFrame->infos.type4.temp / 10.0);
            m_keywords.push_back(tempKeyword);

            auto hygroKeyword = boost::make_shared<yApi::historization::CHumidity>("hygrometry", yApi::EKeywordAccessMode::kGet);
            hygroKeyword->set(pFrame->infos.type4.hygro);
            m_keywords.push_back(hygroKeyword);

            break;
         }
         case INFOS_TYPE5:
         {
            //Used by Scientific Oregon protocol ( Atmospheric pressure sensors)
            identifyOregonDeviceFromIdPhy(pFrame->infos.type5.idPHY, pFrame->infos.type5.idChannel);
            manageOregonBatteryFromQualifier(pFrame->infos.type5.qualifier);

            auto tempKeyword = boost::make_shared<yApi::historization::CTemperature>("temperature", yApi::EKeywordAccessMode::kGet);
            tempKeyword->set(pFrame->infos.type5.temp / 10.0);
            m_keywords.push_back(tempKeyword);

            auto hygroKeyword = boost::make_shared<yApi::historization::CHumidity>("hygrometry", yApi::EKeywordAccessMode::kGet);
            hygroKeyword->set(pFrame->infos.type5.hygro);
            m_keywords.push_back(hygroKeyword);

            auto pressureKeyword = boost::make_shared<yApi::historization::CPressure>("pressure", yApi::EKeywordAccessMode::kGet);
            pressureKeyword->set(pFrame->infos.type5.pressure);
            m_keywords.push_back(pressureKeyword);

            break;
         }
         case INFOS_TYPE6:
         {
            //Used by Scientific Oregon protocol ( Wind sensors)
            identifyOregonDeviceFromIdPhy(pFrame->infos.type6.idPHY, pFrame->infos.type6.idChannel);
            manageOregonBatteryFromQualifier(pFrame->infos.type6.qualifier);

            auto speedKeyword = boost::make_shared<yApi::historization::CSpeed>("wind_speed", yApi::EKeywordAccessMode::kGet);
            speedKeyword->set(pFrame->infos.type6.speed / 10.0);
            m_keywords.push_back(speedKeyword);

            auto directionKeyword = boost::make_shared<yApi::historization::CDirection>("wind_direction", yApi::EKeywordAccessMode::kGet);
            directionKeyword->set(pFrame->infos.type6.direction);
            m_keywords.push_back(directionKeyword);

            break;
         }
         case INFOS_TYPE7:
         {
            //Used by Scientific Oregon protocol ( UV sensors)
            identifyOregonDeviceFromIdPhy(pFrame->infos.type7.idPHY, pFrame->infos.type7.idChannel);
            manageOregonBatteryFromQualifier(pFrame->infos.type7.qualifier);

            auto uvKeyword = boost::make_shared<yApi::historization::CUv>("uv", yApi::EKeywordAccessMode::kGet);
            uvKeyword->set(pFrame->infos.type7.light);
            m_keywords.push_back(uvKeyword);
            break;
         }
         case INFOS_TYPE8:
         {
            if (pFrame->infos.type8.subtype == 0)
            {
               switch (pFrame->infos.type8.idPHY)
               {
               case 0:
                  m_deviceModel += " CM119/CM160";
                  break;
               case 1:
                  m_deviceModel += " CM130";
                  break;
               case 2:
                  m_deviceModel += " CM180";
                  break;
               case 3:
                  m_deviceModel += " CM180i";
                  break;
               }
               m_deviceName = (boost::format("%1%") % (pFrame->infos.type8.idChannel >> 4)).str();
               m_deviceDetails.set("adr_channel", pFrame->infos.type8.idChannel);

               manageOregonBatteryFromQualifier(pFrame->infos.type8.qualifier);


               //out << "    " << ((pFrame->infos.type8.qualifier & 0x0002) == 0 ? " Only the total instantaneous Power is given" : " Power on each input 1, 2, 3 are added (CM180i only).");

               auto energyKeyword = boost::make_shared<yApi::historization::CEnergy>("energy", yApi::EKeywordAccessMode::kGet);
               energyKeyword->set((pFrame->infos.type8.energyLsb + (pFrame->infos.type8.energyMsb << 8)));
               m_keywords.push_back(energyKeyword);

               auto totalPowerKeyword = boost::make_shared<yApi::historization::CPower>("power", yApi::EKeywordAccessMode::kGet);
               totalPowerKeyword->set(pFrame->infos.type8.power);
               m_keywords.push_back(totalPowerKeyword);

               if ((pFrame->infos.type8.qualifier & 0x0002) != 0)
               {
                  auto power1Keyword = boost::make_shared<yApi::historization::CPower>("line1", yApi::EKeywordAccessMode::kGet);
                  auto power2Keyword = boost::make_shared<yApi::historization::CPower>("line2", yApi::EKeywordAccessMode::kGet);
                  auto power3Keyword = boost::make_shared<yApi::historization::CPower>("line3", yApi::EKeywordAccessMode::kGet);

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
         {
            //Used by Scientific Oregon protocol ( Rain sensors)
            identifyOregonDeviceFromIdPhy(pFrame->infos.type9.idPHY, pFrame->infos.type9.idChannel);
            manageOregonBatteryFromQualifier(pFrame->infos.type9.qualifier);


            auto totalRainKeyword = boost::make_shared<yApi::historization::CRain>("rain");
            totalRainKeyword->set((pFrame->infos.type9.totalRainLsb + (pFrame->infos.type9.totalRainMsb << 8)));
            m_keywords.push_back(totalRainKeyword);

            auto rainRateKeyword = boost::make_shared<yApi::historization::CRainRate>("rain_rate");
            rainRateKeyword->set(pFrame->infos.type9.rain);
            m_keywords.push_back(rainRateKeyword);

            break;
         }
         case INFOS_TYPE10:
         {
            //Used by Thermostats X2D protocol
            switch (pFrame->infos.type10.subtype)
            {
            case 0: m_deviceModel += " GENERIC"; break;
            case 1: m_deviceModel += " RADIO TYBOX"; break;
            case 2: m_deviceModel += " TYBOX BUS"; break;
            case 3: m_deviceModel += " PACK LABEL"; break;
            case 4: m_deviceModel += " DELTA 200"; break;
            case 5: m_deviceModel += " DRIVER RF"; break;
            case 6: m_deviceModel += " STARBOX F03"; break;
            case 7: m_deviceModel += " OTHER"; break;
            case 8: m_deviceModel += " REC BIDIR"; break;
            }

            unsigned char area = pFrame->infos.type10.idLsb & 0x000F;
            unsigned int device = (pFrame->infos.type10.idMsb << 16) + pFrame->infos.type10.idLsb;
            m_deviceDetails.set("id", device);
            m_deviceDetails.set("area", (int)area);
            m_deviceName = (boost::format("%1%") % device).str();

            auto batteryLevelKeyword = boost::make_shared<yApi::historization::CBatteryLevel>("battery");
            batteryLevelKeyword->set((pFrame->infos.type10.qualifier & 0x0004) == 0 ? 100 : 0);
            m_keywords.push_back(batteryLevelKeyword);

            auto tamperKeyword = boost::make_shared<yApi::historization::CTamper>("tamper", yApi::EKeywordAccessMode::kGet);
            tamperKeyword->set((pFrame->infos.type10.qualifier & 0x0001) != 0);
            m_keywords.push_back(tamperKeyword);

            switch (pFrame->infos.type10.function)
            {
            case 1: //heating speed
            {
               auto heatingSpeedKeyword = boost::make_shared<yApi::historization::CSwitch>("heating_speed", yApi::EKeywordAccessMode::kGet);
               heatingSpeedKeyword->set((pFrame->infos.type10.mode & 0x00FF) != 0);
               m_keywords.push_back(heatingSpeedKeyword);
               break;
            }
            case 12: //regulation
            {
               auto regulationKeyword = boost::make_shared<yApi::historization::CSwitch>("regulation", yApi::EKeywordAccessMode::kGet);
               regulationKeyword->set((pFrame->infos.type10.mode & 0x00FF) != 0);
               m_keywords.push_back(regulationKeyword);
               break;
            }
            case 26: //thermic area state
            {
               auto thermicAreaStateKeyword = boost::make_shared<yApi::historization::CSwitch>("thermic_area_state", yApi::EKeywordAccessMode::kGet);
               thermicAreaStateKeyword->set((pFrame->infos.type10.mode & 0x00FF) != 0);
               m_keywords.push_back(thermicAreaStateKeyword);
               break;
            }
            case 2: //operating  mode
            {
               auto operatingModeKeyword = boost::make_shared<specificHistorizers::incoming::CType10OperatingMode>("keyCode");
               operatingModeKeyword->set(specificHistorizers::incoming::EType10OperatingModeValues((int)(pFrame->infos.type10.mode & 0x00FF)));
               m_keywords.push_back(operatingModeKeyword);

               break;
            }
            }

            break;
         }
         case INFOS_TYPE11:
         {
            unsigned int device = (pFrame->infos.type11.idMsb << 16) + pFrame->infos.type11.idLsb;
            m_deviceName = (boost::format("%1%") % device).str();
            if (pFrame->infos.type11.subtype == 0)
            {
               m_deviceModel +=  "Alarm detector/sensor";

               auto batteryLevelKeyword = boost::make_shared<yApi::historization::CBatteryLevel>("battery");
               batteryLevelKeyword->set((pFrame->infos.type11.qualifier & 0x0004) == 0 ? 100 : 0);
               m_keywords.push_back(batteryLevelKeyword);

               auto alarmKeyword = boost::make_shared<yApi::historization::CAlarm>("alarm", yApi::EKeywordAccessMode::kGet);
               alarmKeyword->set((pFrame->infos.type11.qualifier & 0x0002) != 0);
               m_keywords.push_back(alarmKeyword);

               auto tamperKeyword = boost::make_shared<yApi::historization::CTamper>("tamper", yApi::EKeywordAccessMode::kGet);
               tamperKeyword->set((pFrame->infos.type11.qualifier & 0x0001) != 0);
               m_keywords.push_back(tamperKeyword);
            }
            else
            {
               m_deviceModel += " Alarm remote control";

               auto remoteControlKeyword = boost::make_shared<specificHistorizers::incoming::CType11State>("remoteControl");
               remoteControlKeyword->set(specificHistorizers::incoming::EType11StateValues((int)pFrame->infos.type11.qualifier));
               m_keywords.push_back(remoteControlKeyword);
            }
            break;
         }
         case INFOS_TYPE12:
            //deprecated
            break;

         }

         break;
      }
      case RFLINK_INCOMING_BINARY_USB_FRAME_TYPE:
         break;
      }
   }

} //namespace incoming
} //namespace frames

