#include "stdafx.h"
#include "SensorBasic.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>
#include <shared/plugin/yadomsApi/historization/Historizers.h>
#include "data/DigimaxDemand.h"
#include "data/DigitalIoStatus.h"
#include "data/MertikStatus.h"
#include "data/Forecast.h"
#include "data/HumidityDescription.h"
#include "data/UvDescription.h"

#include "ControlBasic.h"

namespace xplrules { namespace rfxLanXpl {

   xplcore::CXplMessageSchemaIdentifier  CSensorBasic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("sensor.basic");

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
   std::string CSensorBasic::m_keywordTypeInput = "input";
   std::string CSensorBasic::m_keywordTypeStatus = "status";
   std::string CSensorBasic::m_keywordTypePressure = "pressure";
   std::string CSensorBasic::m_keywordTypeTemperature = "temp";
   std::string CSensorBasic::m_keywordTypeSetpoint = "setpoint";
   std::string CSensorBasic::m_keywordTypeDirection = "direction";
   std::string CSensorBasic::m_keywordTypeUv = "uv";
   std::string CSensorBasic::m_keywordCurrent = "current";
   std::string CSensorBasic::m_keywordDescription = "description";
   std::string CSensorBasic::m_keywordForecast = "forecast";
   std::string CSensorBasic::m_keywordUnits = "units";

   std::string CSensorBasic::m_keywordTypeDemandValues = "{ values: [heater_on, heater_off, cooling_on, cooling_off] }";
   std::string CSensorBasic::m_keywordTypeIoValues = "{ values: [high,low]}";
   std::string CSensorBasic::m_keywordTypeMertikValues = "{ values: [on, off, step_up, step_down, run_up, run_down, stop]}";
   std::string CSensorBasic::m_keywordTypeHumidityValues = "{ values: [normal, comfort, dry, wet]}";
   std::string CSensorBasic::m_keywordTypeForecastValues = "{ values: [sunny, partly cloudy, cloudy, rain]}";
   std::string CSensorBasic::m_keywordTypeUvDescriptionValues =  "{ values: [low, medium, high, very high, dangerous]}";
   
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

   const xplcore::CXplMessageSchemaIdentifier CSensorBasic::getProtocol()
   {
      return m_protocol;
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
      return CDeviceIdentifier(deviceId, deviceName, m_protocol, CControlBasic::getProtocol());
   }

   MessageContent CSensorBasic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;


      if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeBattery))
      {
         boost::shared_ptr< shared::plugin::yadomsApi::historization::CBatteryLevel > battLvl(new shared::plugin::yadomsApi::historization::CBatteryLevel(m_keywordTypeBattery));
         battLvl->set(boost::lexical_cast<int>(msg.getBodyValue(m_keywordCurrent)));
         data.push_back(battLvl);
      }

      //DIGIMAX
      if (boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceDigimax))
      {

         if (msg.getBodyValue(m_keywordType) == m_keywordTypeDemand)
         {
            boost::shared_ptr< data::CDigimaxDemand > digimaxDemand(new data::CDigimaxDemand(m_keywordTypeDemand));
            digimaxDemand->set(data::EDigimaxDemand::parse(msg.getBodyValue(m_keywordCurrent)));
            data.push_back(digimaxDemand);
         }

         if (msg.getBodyValue(m_keywordType) == m_keywordTypeTemp)
         {
            boost::shared_ptr< shared::plugin::yadomsApi::historization::CTemperature > digimaxTemp(new shared::plugin::yadomsApi::historization::CTemperature(m_keywordTypeTemp));
            digimaxTemp->set(msg.getBodyValue(m_keywordCurrent));
            data.push_back(digimaxTemp);
         }

         if (msg.getBodyValue(m_keywordType) == m_keywordTypeSetpoint)
         {
            boost::shared_ptr< shared::plugin::yadomsApi::historization::CTemperature > digimaxSetPoint(new shared::plugin::yadomsApi::historization::CTemperature(m_keywordTypeSetpoint));
            digimaxSetPoint->set(msg.getBodyValue(m_keywordCurrent));
            data.push_back(digimaxSetPoint);
         }

      }

      //RFXSENSOR
      if (boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceRfxSensor))
      {
         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeVoltage))
         {
            boost::shared_ptr< shared::plugin::yadomsApi::historization::CVoltage > volt(new shared::plugin::yadomsApi::historization::CVoltage(m_keywordTypeVoltage));
            volt->set(boost::lexical_cast<double>(msg.getBodyValue(m_keywordCurrent)));
            data.push_back(volt);
         }
            
         if (msg.getBodyValue(m_keywordType) == m_keywordTypeTemp)
         {
            boost::shared_ptr< shared::plugin::yadomsApi::historization::CTemperature > temp(new shared::plugin::yadomsApi::historization::CTemperature(m_keywordTypeTemp));
            temp->set(msg.getBodyValue(m_keywordCurrent));
            data.push_back(temp);
         }
      }

      //RFXMETER
      if (boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceRfxMeter) && boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeCount))
      {
         boost::shared_ptr< shared::plugin::yadomsApi::historization::CCounter > temp(new shared::plugin::yadomsApi::historization::CCounter(m_keywordTypeCount));
         temp->set(boost::lexical_cast<int>(msg.getBodyValue(m_keywordCurrent)));
         data.push_back(temp);

      }

      //RFXLAN IO
      if (boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceRfxLanIo))
      {
         boost::shared_ptr< data::CDigitalIoStatus > digitalLine(new data::CDigitalIoStatus(m_keywordTypeDemand));
         digitalLine->set(data::EDigitalIoStatus::parse(msg.getBodyValue(m_keywordCurrent)));
         data.push_back(digitalLine);
      }

      //Mertik
      if (boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordHex) && boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeMertik))
      {
         boost::shared_ptr< data::CMertickStatus > mertikStatus(new data::CMertickStatus(m_keywordTypeDemand));
         mertikStatus->set(data::EMertikStatus::parse(msg.getBodyValue(m_keywordCurrent)));
         data.push_back(mertikStatus);
      }

      //Oregon
      if (isOregonDevice(msg))
      {
         //Temperature
         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeTemp))
         {
            boost::shared_ptr< shared::plugin::yadomsApi::historization::CTemperature > temp(new shared::plugin::yadomsApi::historization::CTemperature(m_keywordTypeTemp));
            temp->set(boost::lexical_cast<double>(msg.getBodyValue(m_keywordCurrent)));
            data.push_back(temp);
         }

         //Humidity
         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeHumidity))
         {
            boost::shared_ptr< shared::plugin::yadomsApi::historization::CHumidity > humidity(new shared::plugin::yadomsApi::historization::CHumidity(m_keywordTypeHumidity));
            humidity->set(boost::lexical_cast<double>(msg.getBodyValue(m_keywordCurrent)));
            data.push_back(humidity);

            boost::shared_ptr< data::CHumidityDescription > humidityDescription(new data::CHumidityDescription(m_keywordDescription));
            humidityDescription->set(data::EHumidityDescription::parse(msg.getBodyValue(m_keywordCurrent)));
            data.push_back(humidityDescription);
         }

         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeStatus))
         {
            boost::shared_ptr< data::CHumidityDescription > humidityDescription(new data::CHumidityDescription(m_keywordTypeStatus));
            humidityDescription->set(data::EHumidityDescription::parse(msg.getBodyValue(m_keywordCurrent)));
            data.push_back(humidityDescription);
         }

         //Pressure
         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypePressure))
         {
            boost::shared_ptr< shared::plugin::yadomsApi::historization::CPressure > pressure(new shared::plugin::yadomsApi::historization::CPressure(m_keywordTypePressure));
            pressure->set(boost::lexical_cast<double>(msg.getBodyValue(m_keywordCurrent)));
            data.push_back(pressure);


            boost::shared_ptr< data::CForecast > forecast(new data::CForecast(m_keywordForecast));
            forecast->set(data::EForecast::parse(msg.getBodyValue(m_keywordCurrent)));
            data.push_back(forecast);
         }

      }
      /* TODO
      data.insert(std::make_pair(msg.getBodyValue(m_keywordType), msg.getBodyValue(m_keywordCurrent)));

      if(msg.getBody().find(m_keywordDescription) != msg.getBody().end())
         data.insert(std::make_pair(m_keywordDescription, msg.getBodyValue(m_keywordDescription)));

      if(msg.getBody().find(m_keywordForecast) != msg.getBody().end())
         data.insert(std::make_pair(m_keywordForecast, msg.getBodyValue(m_keywordForecast)));
         */
      return data;
   }










   KeywordList CSensorBasic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      KeywordList keywords;
      
      if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeBattery))
      {
         keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new shared::plugin::yadomsApi::historization::CBatteryLevel(m_keywordTypeBattery)));
      }
         
      //DIGIMAX
      if (boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceDigimax))
      {

         if (msg.getBodyValue(m_keywordType) == m_keywordTypeDemand)
            keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new data::CDigimaxDemand(m_keywordTypeDemand)));

         if (msg.getBodyValue(m_keywordType) == m_keywordTypeTemp)
            keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new shared::plugin::yadomsApi::historization::CTemperature(m_keywordTypeTemp)));

         if (msg.getBodyValue(m_keywordType) == m_keywordTypeSetpoint)
            keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new shared::plugin::yadomsApi::historization::CTemperature(m_keywordTypeSetpoint)));
            
      }



      //RFXSENSOR
      if (boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceRfxSensor))
      {
         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeVoltage))
            keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new shared::plugin::yadomsApi::historization::CVoltage(m_keywordTypeVoltage)));

         if (msg.getBodyValue(m_keywordType) == m_keywordTypeTemp)
            keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new shared::plugin::yadomsApi::historization::CTemperature(m_keywordTypeVoltage)));
      }

      //RFXMETER
      if (boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceRfxMeter) && boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeCount))
      {
         keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new shared::plugin::yadomsApi::historization::CCounter(m_keywordTypeCount)));
      }

      //RFXLAN IO
      if (boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordDeviceRfxLanIo))
      {
         keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new data::CDigitalIoStatus(m_keywordTypeInput)));
      }

      //Mertik
      if (boost::starts_with(msg.getBodyValue(m_keywordDevice), m_keywordHex) && boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeMertik))
      {
         keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new data::CMertickStatus(m_keywordTypeMertik)));
      }

      //Oregon
      if (isOregonDevice(msg))
      {
         //Temperature
         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeTemp))
         {
            keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new shared::plugin::yadomsApi::historization::CTemperature(m_keywordTypeTemp)));
         }

         //Humidity
         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeHumidity))
         {
            keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new shared::plugin::yadomsApi::historization::CHumidity(m_keywordTypeHumidity)));
            keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new data::CHumidityDescription(m_keywordDescription)));
         }

         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeStatus))
            keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new data::CHumidityDescription(m_keywordTypeStatus)));

         //Pressure
         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypePressure))
         {
            keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new shared::plugin::yadomsApi::historization::CPressure(m_keywordTypePressure)));
            keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new data::CForecast(m_keywordForecast)));
         }


         /*

         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeRainRate) ||
            boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeRainTotal) ||
            boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeGust) ||
            boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeAverageSpeed) ||
            boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeWeight) ||
            boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypePower) ||
            boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeEnergy))
         {
            keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword(msg.getBodyValue(m_keywordType), msg.getBodyValue(m_keywordType), yApi::kGet, yApi::kNumeric, units, details)));
         }

         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeDirection))
         {
            details.set("min", 0);
            details.set("max", 359);
            keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword(m_keywordTypeDirection, m_keywordTypeDirection, yApi::kGet, yApi::kNumeric, yApi::CStandardUnits::NoUnits, details)));
         }

         if (boost::iequals(msg.getBodyValue(m_keywordType), m_keywordTypeUv))
         {
            details.set("min", 0);
            details.set("max", 12);
            keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword(m_keywordTypeUv, m_keywordTypeUv, yApi::kGet, yApi::kNumeric, yApi::CStandardUnits::NoUnits, details)));
            keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword(m_keywordDescription, m_keywordDescription, yApi::kGet, yApi::kString, yApi::CStandardUnits::NoUnits, m_keywordTypeUvDescriptionValues)));
         }

         if ((boost::iequals(msg.getBodyValue(m_keywordDevice), m_keywordDeviceOregonElec1) ||
            boost::iequals(msg.getBodyValue(m_keywordDevice), m_keywordDeviceOregonElec2) ||
            boost::iequals(msg.getBodyValue(m_keywordDevice), m_keywordDeviceOregonElec3) ||
            boost::iequals(msg.getBodyValue(m_keywordDevice), m_keywordDeviceOregonElec4)) &&
            boost::iequals(msg.getBodyValue(m_keywordType), m_keywordCurrent))
         {
            keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword(msg.getBodyValue(m_keywordType), msg.getBodyValue(m_keywordType), yApi::kGet, yApi::kNumeric, units, details)));
         }
         */
      }


      return keywords;
   }
   

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
