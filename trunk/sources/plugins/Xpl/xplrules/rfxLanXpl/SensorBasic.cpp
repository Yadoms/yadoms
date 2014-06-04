#include "stdafx.h"
#include "SensorBasic.h"

namespace xplrules { namespace rfxLanXpl {


   //-------------------------------------------------------------------------------------
   //-------------------------------------------------------------------------------------
   //-------------------------------------------------------------------------------------
   //-------------------------------------------------------------------------------------
   //-------------------------------------------------------------------------------------
   // All this file has been written from the "RFXCOM Implementation.pdf"
   // RFXLAN xPL version 7.8 04-01-2013
   //-------------------------------------------------------------------------------------
   //-------------------------------------------------------------------------------------
   //-------------------------------------------------------------------------------------
   //-------------------------------------------------------------------------------------
   //-------------------------------------------------------------------------------------
   std::string CSensorBasic::m_keywordHex = "0x";
   std::string CSensorBasic::m_keywordDevice = "device";
   std::string CSensorBasic::m_keywordType = "type";
   std::string CSensorBasic::m_keywordTypeBattery = "battery";
   std::string CSensorBasic::m_keywordTypeDemand = "demand";
   std::string CSensorBasic::m_keywordTypeVoltage = "voltage";
   std::string CSensorBasic::m_keywordTypeMertik = "mertik";
   std::string CSensorBasic::m_keywordTypeCount = "count";
   std::string CSensorBasic::m_keywordTypeHumidity = "humidity";
   std::string CSensorBasic::m_keywordTypeStatus = "status";
   std::string CSensorBasic::m_keywordTypePressure = "pressure";
   std::string CSensorBasic::m_keywordTypeDirection = "direction";
   std::string CSensorBasic::m_keywordTypeUv = "uv";
   std::string CSensorBasic::m_keywordCurrent = "current";
   std::string CSensorBasic::m_keywordDescription = "description";
   std::string CSensorBasic::m_keywordForecast = "forecast";
   std::string CSensorBasic::m_keywordUnits = "units";

   std::string CSensorBasic::m_keywordTypeDemandValues = "heater_on|heater_off|cooling_on|cooling_off";
   std::string CSensorBasic::m_keywordTypeIoValues = "high|low";
   std::string CSensorBasic::m_keywordTypeMertikValues = "on|off|step_up|step_down|run_up|run_down|stop";
   std::string CSensorBasic::m_keywordTypeHumidityValues = "normal|comfort|dry|wet";
   std::string CSensorBasic::m_keywordTypeForecastValues = "sunny|partly cloudy|cloudy|rain";
   std::string CSensorBasic::m_keywordTypeUvDescriptionValues =  "low|medium|high|very high|dangerous";
   
   std::string CSensorBasic::m_keywordTypeTemp = "temp";
   std::string CSensorBasic::m_keywordTypeRainRate = "rainrate";
   std::string CSensorBasic::m_keywordTypeRainTotal = "raintotal";
   std::string CSensorBasic::m_keywordTypeGust = "gust";
   std::string CSensorBasic::m_keywordTypeAverageSpeed = "average_speed";
   std::string CSensorBasic::m_keywordTypeWeight = "weight";
   std::string CSensorBasic::m_keywordTypePower = "power";
   std::string CSensorBasic::m_keywordTypeEnergy = "energy";
   

   std::string CSensorBasic::m_keywordDeviceDigimax = "digimax";
   std::string CSensorBasic::m_keywordDeviceRfxSensor = "rfxsensor";
   std::string CSensorBasic::m_keywordDeviceRfxMeter = "rfxmeter";
   std::string CSensorBasic::m_keywordDeviceRfxLanIo = "io";
   std::string CSensorBasic::m_keywordDeviceMertik = "";
   std::string CSensorBasic::m_keywordDeviceOregonTemp1="temp1";
   std::string CSensorBasic::m_keywordDeviceOregonTemp2="temp2";
   std::string CSensorBasic::m_keywordDeviceOregonTemp3="temp3";
   std::string CSensorBasic::m_keywordDeviceOregonTemp4="temp4";
   std::string CSensorBasic::m_keywordDeviceOregonTh1="th1";
   std::string CSensorBasic::m_keywordDeviceOregonTh2="th2";
   std::string CSensorBasic::m_keywordDeviceOregonTh3="th3";
   std::string CSensorBasic::m_keywordDeviceOregonTh4="th4";
   std::string CSensorBasic::m_keywordDeviceOregonTh5="th5";
   std::string CSensorBasic::m_keywordDeviceOregonTh6="th6";
   std::string CSensorBasic::m_keywordDeviceOregonThb1="thb1";
   std::string CSensorBasic::m_keywordDeviceOregonThb2="thb2";
   std::string CSensorBasic::m_keywordDeviceOregonRain1="rain1";
   std::string CSensorBasic::m_keywordDeviceOregonRain2="rain2";
   std::string CSensorBasic::m_keywordDeviceOregonWind1="wind1";
   std::string CSensorBasic::m_keywordDeviceOregonWind2="wind2";
   std::string CSensorBasic::m_keywordDeviceOregonWind3="wind3";
   std::string CSensorBasic::m_keywordDeviceOregonUv1="uv1";
   std::string CSensorBasic::m_keywordDeviceOregonUv2="uv2";
   std::string CSensorBasic::m_keywordDeviceOregonDateTime1="dt1";
   std::string CSensorBasic::m_keywordDeviceOregonWeight1="WEIGHT1";
   std::string CSensorBasic::m_keywordDeviceOregonWeight2="WEIGHT2";
   std::string CSensorBasic::m_keywordDeviceOregonElec1="elec1";
   std::string CSensorBasic::m_keywordDeviceOregonElec2="elec2";
   std::string CSensorBasic::m_keywordDeviceOregonElec3="elec3";
   std::string CSensorBasic::m_keywordDeviceOregonElec4="elec4";

   std::string CSensorBasic::m_keywordUnitAmpere = "A";
   std::string CSensorBasic::m_keywordUnitVolts = "V";

   CSensorBasic::CSensorBasic()
   {
   }

   CSensorBasic::~CSensorBasic()
   {
   }

   const CDeviceIdentifier CSensorBasic::getDeviceAddressFromMessage(xplcore::CXplMessage & msg)
   {
      std::string deviceId = msg.getBodyValue(m_keywordDevice);
      std::string deviceName="";

      if(boost::istarts_with(deviceId, m_keywordDeviceDigimax))
         deviceName="Digimax";

      if(boost::istarts_with(deviceId, m_keywordDeviceRfxSensor))
         deviceName="RFXSensor";

      if(boost::istarts_with(deviceId, m_keywordDeviceRfxMeter))
         deviceName="RFXMeter";

      if(boost::istarts_with(deviceId, m_keywordDeviceRfxLanIo))
      {
         deviceName="RFXLAN I/O Line ";
         deviceName += deviceId[2];
      }

      if(boost::istarts_with(deviceId, m_keywordHex) && boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeMertik))
         deviceName="Mertik-Maxitrol";

      if(isOregonDevice(msg))
      {
         deviceName="Oregon";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonTemp1))
            deviceName += " Inside temperature (THR128/138 THC138)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonTemp2))
            deviceName += " Outside/Water/BBQ temperature (THC238/268, THN122N/132N, THWR288A, THRN122N, AW129/131)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonTemp3))
            deviceName += " Water temperature (THWR800)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonTemp4))
            deviceName += " Outside Temperature (RTHN318)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonTh1))
            deviceName += " Inside Temp-Hygro  (THGN122N/123N, THGR122NX, THGR228N, THGR238/268)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonTh2))
            deviceName += " Inside Temp-Hygro  (THGR810)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonTh3))
            deviceName += " Outside Temp-Hygro (RTGR328N)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonTh4))
            deviceName += " Outside Temp-Hygro (THGR328N)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonTh5))
            deviceName += " Outside Temp-Hygro (WTGR800)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonTh6))
            deviceName += " Outside Temp-Hygro (THGR918, THGRN228NX, THGN500)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonThb1))
            deviceName += " Inside Temp-Hygro-Baro (Huger-BTHR918)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonThb2))
            deviceName += " Inside Temp-Hygro-Baro (BTHR918N, BTHR968)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonRain1))
            deviceName += " Rain gauge (RGR126, RGR682, RGR918)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonRain2))
            deviceName += " Rain gauge (PCR800)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonWind1))
            deviceName += " Anemometer (WTGR800)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonWind2))
            deviceName += " Anemometer (WGR800)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonWind3))
            deviceName += " Anemometer (Huger-STR918, WGR918)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonUv1))
            deviceName += " UV sensor (UVN128, UV138)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonUv2))
            deviceName += " UV sensor (UVN800)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonDateTime1))
            deviceName += " Date & Time (RTGR328N)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonWeight1))
            deviceName += " Body Weight Monitor (BWR102)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonWeight1))
            deviceName += " Body Weight Monitor (GR101)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonElec1))
            deviceName += " Ampere meter (cent-a-meter, Electrisave, OWL CM113)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonElec2))
            deviceName += " Power meter (OWL CM119, CM160)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonElec3))
            deviceName += " Power meter (OWL CM180)";

         if(boost::istarts_with(deviceId, m_keywordDeviceOregonElec4))
            deviceName += " Ampere+Power meter (OWL CM180i)";
      }
      return CDeviceIdentifier(deviceId, deviceName);
   }

   MessageContent CSensorBasic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair(msg.getBodyValue(m_keywordType), msg.getBodyValue(m_keywordCurrent)));

      if(msg.getBody().find(m_keywordDescription) != msg.getBody().end())
         data.insert(std::make_pair(m_keywordDescription, msg.getBodyValue(m_keywordDescription)));

      if(msg.getBody().find(m_keywordForecast) != msg.getBody().end())
         data.insert(std::make_pair(m_keywordForecast, msg.getBodyValue(m_keywordForecast)));

      return data;
   }
   /*
   std::vector< boost::shared_ptr<database::entities::CKeyword> > CSensorBasic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<database::entities::CKeyword> > keywords;

      //COMMON
      std::string units = "";
      if(msg.getBody().find(m_keywordUnits) != msg.getBody().end())
         units = msg.getBodyValue(m_keywordUnits);

      if(boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeBattery))
      {
         keywords.push_back(CKeywordManager::createNumeric(msg.getBodyValue(m_keywordType), 0, 100, units));
      }

      //DIGIMAX
      if(boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceDigimax))
      {
         if(msg.getBodyValue(m_keywordType) == m_keywordTypeDemand)
         {
            keywords.push_back(CKeywordManager::createEnumeration(msg.getBodyValue(m_keywordType), m_keywordTypeDemandValues));
         }
         else
         {
            keywords.push_back(CKeywordManager::createNumeric(msg.getBodyValue(m_keywordType), units));
         }
      }

      //RFXSENSOR
      if(boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceRfxSensor))
      {
         if(boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeVoltage))
         {
            if(units.empty())
               units = m_keywordUnitVolts;
         }
         keywords.push_back(CKeywordManager::createNumeric(msg.getBodyValue(m_keywordType), units));
      }

      //RFXMETER
      if(boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceRfxMeter) && boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeCount))
      {
         keywords.push_back(CKeywordManager::createNumeric(msg.getBodyValue(m_keywordType), 0, 16777215, units));
      }

      //RFXLAN IO
      if(boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceRfxLanIo))
      {
         keywords.push_back(CKeywordManager::createEnumeration(msg.getBodyValue(m_keywordType), m_keywordTypeIoValues));
      }

      //Mertik
      if(boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordHex) && boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeMertik))
      {
         keywords.push_back(CKeywordManager::createEnumeration(msg.getBodyValue(m_keywordType), m_keywordTypeMertikValues));
      }

      //Oregon
      if(isOregonDevice(msg))
      {
         if(boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeHumidity))
         {
            keywords.push_back(CKeywordManager::createNumeric(msg.getBodyValue(m_keywordType), 0, 100, units));
            keywords.push_back(CKeywordManager::createEnumeration(m_keywordDescription, m_keywordTypeHumidityValues));
         }

         if(boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeStatus))
         {
            keywords.push_back(CKeywordManager::createEnumeration(msg.getBodyValue(m_keywordType), m_keywordTypeHumidityValues));
         }

         if(boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeTemp) ||
            boost::iequals(msg.getBodyValue(m_keywordType),  m_keywordTypeRainRate) ||
            boost::iequals(msg.getBodyValue(m_keywordType),  m_keywordTypeRainTotal) ||
            boost::iequals(msg.getBodyValue(m_keywordType),  m_keywordTypeGust) ||
            boost::iequals(msg.getBodyValue(m_keywordType),  m_keywordTypeAverageSpeed) ||
            boost::iequals(msg.getBodyValue(m_keywordType),  m_keywordTypeWeight) ||
            boost::iequals(msg.getBodyValue(m_keywordType),  m_keywordTypePower) ||
            boost::iequals(msg.getBodyValue(m_keywordType),  m_keywordTypeEnergy))
         {
            keywords.push_back(CKeywordManager::createNumeric(msg.getBodyValue(m_keywordType), units));
         }

         if(boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypePressure))
         {
            keywords.push_back(CKeywordManager::createNumeric(msg.getBodyValue(m_keywordType), units));
            keywords.push_back(CKeywordManager::createEnumeration(m_keywordForecast, m_keywordTypeForecastValues));
         }

         if(boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeDirection))
         {
            keywords.push_back(CKeywordManager::createNumeric(msg.getBodyValue(m_keywordType), 0, 359, units));
         }

         if(boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeUv))
         {
            keywords.push_back(CKeywordManager::createNumeric(msg.getBodyValue(m_keywordType), 0, 12, units));
            keywords.push_back(CKeywordManager::createEnumeration(m_keywordDescription, m_keywordTypeUvDescriptionValues));
         }

         if( (boost::iequals(msg.getBodyValue(m_keywordDevice), m_keywordDeviceOregonElec1) ||
              boost::iequals(msg.getBodyValue(m_keywordDevice), m_keywordDeviceOregonElec2) ||
              boost::iequals(msg.getBodyValue(m_keywordDevice), m_keywordDeviceOregonElec3) ||
              boost::iequals(msg.getBodyValue(m_keywordDevice), m_keywordDeviceOregonElec4)) && 
              boost::iequals(msg.getBodyValue(m_keywordType), m_keywordCurrent))
         {
            units = m_keywordUnitAmpere;
            keywords.push_back(CKeywordManager::createNumeric(msg.getBodyValue(m_keywordType), units));
         }

      }
      return keywords;
   }
   */

   bool CSensorBasic::isOregonDevice(xplcore::CXplMessage & msg)
   {
      std::string device = boost::to_lower_copy(msg.getBodyValue(m_keywordDevice));
      return boost::starts_with(device, m_keywordDeviceOregonTemp1) ||
         boost::starts_with(device, m_keywordDeviceOregonTemp2) ||
         boost::starts_with(device, m_keywordDeviceOregonTemp3) ||
         boost::starts_with(device, m_keywordDeviceOregonTemp4) ||
         boost::starts_with(device, m_keywordDeviceOregonTh1) ||
         boost::starts_with(device, m_keywordDeviceOregonTh2) ||
         boost::starts_with(device, m_keywordDeviceOregonTh3) ||
         boost::starts_with(device, m_keywordDeviceOregonTh4) ||
         boost::starts_with(device, m_keywordDeviceOregonTh5) ||
         boost::starts_with(device, m_keywordDeviceOregonTh6) ||
         boost::starts_with(device, m_keywordDeviceOregonThb1) ||
         boost::starts_with(device, m_keywordDeviceOregonThb2) ||
         boost::starts_with(device, m_keywordDeviceOregonRain1) ||
         boost::starts_with(device, m_keywordDeviceOregonRain2) ||
         boost::starts_with(device, m_keywordDeviceOregonWind1) ||
         boost::starts_with(device, m_keywordDeviceOregonWind2) ||
         boost::starts_with(device, m_keywordDeviceOregonWind3) ||
         boost::starts_with(device, m_keywordDeviceOregonUv1) ||
         boost::starts_with(device, m_keywordDeviceOregonUv2) ||
         boost::starts_with(device, m_keywordDeviceOregonDateTime1) ||
         boost::starts_with(device, m_keywordDeviceOregonWeight1) ||
         boost::starts_with(device, m_keywordDeviceOregonWeight2) ||
         boost::starts_with(device, m_keywordDeviceOregonElec1) ||
         boost::starts_with(device, m_keywordDeviceOregonElec2) ||
         boost::starts_with(device, m_keywordDeviceOregonElec3) ||
         boost::starts_with(device, m_keywordDeviceOregonElec4);
   }


} //namespace rfxLanXpl
} //namespace xplrules
