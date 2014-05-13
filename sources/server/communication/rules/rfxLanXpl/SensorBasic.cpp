#include "stdafx.h"
#include "SensorBasic.h"

namespace communication { namespace rules { namespace rfxLanXpl {


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

   CSensorBasic::CSensorBasic()
   {
   }

   CSensorBasic::~CSensorBasic()
   {
   }

   const CDeviceIdentifier CSensorBasic::getDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      std::string deviceId = msg.getBodyValue("device");
      std::string deviceName="";

      if(boost::istarts_with(deviceId, "digimax"))
         deviceName="digimax";

      if(boost::istarts_with(deviceId, "rfxsensor"))
         deviceName="RFXSensor";

      if(boost::istarts_with(deviceId, "rfxmeter"))
         deviceName="RFXMeter";

      if(boost::istarts_with(deviceId, "io"))
      {
         deviceName="RFXLAN I/O Line ";
         deviceName += deviceId[2];
      }

      if(boost::istarts_with(deviceId, "0x") && boost::iequals(msg.getBodyValue("type"), "mertik"))
         deviceName="Mertik-Maxitrol";
      
      if(isOregonDevice(msg))
      {
         deviceName="Oregon";
         
         if(boost::istarts_with(deviceId, "temp1"))
            deviceName += " Inside temperature (THR128/138 THC138)";

         if(boost::istarts_with(deviceId, "temp2"))
            deviceName += " Outside/Water/BBQ temperature (THC238/268, THN122N/132N, THWR288A, THRN122N, AW129/131)";

         if(boost::istarts_with(deviceId, "temp3"))
            deviceName += " Water temperature (THWR800)";

         if(boost::istarts_with(deviceId, "temp4"))
            deviceName += " Outside Temperature (RTHN318)";

         if(boost::istarts_with(deviceId, "th1"))
            deviceName += " Inside Temp-Hygro  (THGN122N/123N, THGR122NX, THGR228N, THGR238/268)";

         if(boost::istarts_with(deviceId, "th2"))
            deviceName += " Inside Temp-Hygro  (THGR810)";

         if(boost::istarts_with(deviceId, "th3"))
            deviceName += " Outside Temp-Hygro (RTGR328N)";

         if(boost::istarts_with(deviceId, "th4"))
            deviceName += " Outside Temp-Hygro (THGR328N)";

         if(boost::istarts_with(deviceId, "th5"))
            deviceName += " Outside Temp-Hygro (WTGR800)";

         if(boost::istarts_with(deviceId, "th6"))
            deviceName += " Outside Temp-Hygro (THGR918, THGRN228NX, THGN500)";

         if(boost::istarts_with(deviceId, "thb1"))
            deviceName += " Inside Temp-Hygro-Baro (Huger-BTHR918)";

         if(boost::istarts_with(deviceId, "thb2"))
            deviceName += " Inside Temp-Hygro-Baro (BTHR918N, BTHR968)";

         if(boost::istarts_with(deviceId, "rain1"))
            deviceName += " Rain gauge (RGR126, RGR682, RGR918)";

         if(boost::istarts_with(deviceId, "rain2"))
            deviceName += " Rain gauge (PCR800)";

         if(boost::istarts_with(deviceId, "wind1"))
            deviceName += " Anemometer (WTGR800)";

         if(boost::istarts_with(deviceId, "wind2"))
            deviceName += " Anemometer (WGR800)";

         if(boost::istarts_with(deviceId, "wind3"))
            deviceName += " Anemometer (Huger-STR918, WGR918)";

         if(boost::istarts_with(deviceId, "uv1"))
            deviceName += " UV sensor (UVN128, UV138)";

         if(boost::istarts_with(deviceId, "uv2"))
            deviceName += " UV sensor (UVN800)";

         if(boost::istarts_with(deviceId, "dt1"))
            deviceName += " Date & Time (RTGR328N)";

         if(boost::istarts_with(deviceId, "WEIGHT1"))
            deviceName += " Body Weight Monitor (BWR102)";

         if(boost::istarts_with(deviceId, "WEIGHT2"))
            deviceName += " Body Weight Monitor (GR101)";

         if(boost::istarts_with(deviceId, "elec1"))
            deviceName += " Ampere meter (cent-a-meter, Electrisave, OWL CM113)";

         if(boost::istarts_with(deviceId, "elec2"))
            deviceName += " Power meter (OWL CM119, CM160)";

         if(boost::istarts_with(deviceId, "elec3"))
            deviceName += " Power meter (OWL CM180)";

         if(boost::istarts_with(deviceId, "elec4"))
            deviceName += " Ampere+Power meter (OWL CM180i)";
      }
      return CDeviceIdentifier(deviceId, deviceName);
   }

   MessageContent CSensorBasic::extractMessageData(shared::xpl::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair(msg.getBodyValue("type"), msg.getBodyValue("current")));

      if(msg.getBody().find("description") != msg.getBody().end())
         data.insert(std::make_pair("description", msg.getBodyValue("description")));

      if(msg.getBody().find("forecast") != msg.getBody().end())
         data.insert(std::make_pair("forecast", msg.getBodyValue("forecast")));

      return data;
   }

   std::vector< boost::shared_ptr<database::entities::CKeyword> > CSensorBasic::identifyKeywords(shared::xpl::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<database::entities::CKeyword> > keywords;

      boost::shared_ptr<database::entities::CKeyword> mainKeyword(new database::entities::CKeyword());

      //COMMON
      mainKeyword->Name = msg.getBodyValue("type");
      if(msg.getBody().find("units") != msg.getBody().end())
         mainKeyword->Units = msg.getBodyValue("units");

      if(boost::iequals(msg.getBodyValue("type"), "battery"))
      {
         mainKeyword->Type = "numeric";
         mainKeyword->Minimum = 0;
         mainKeyword->Maximum = 100;
      }

      //DIGIMAX
      if(boost::starts_with(msg.getBodyValue("device"), "digimax"))
      {
         if(msg.getBodyValue("type") == "demand")
         {
            mainKeyword->Type = "enumeration";
            mainKeyword->Parameters = "heater_on|heater_off|cooling_on|cooling_off";
         }
         else
         {
            mainKeyword->Type = "numeric";
         }
      }

      //RFXSENSOR
      if(boost::starts_with(msg.getBodyValue("device"), "rfxsensor"))
      {
         if(boost::iequals(msg.getBodyValue("type"), "voltage"))
         {
            if(!mainKeyword->Units.isDefined())
               mainKeyword->Units = "volt";
         }
         mainKeyword->Type = "numeric";
      }

      //RFXMETER
      if(boost::starts_with(msg.getBodyValue("device"), "rfxmeter") && boost::iequals(msg.getBodyValue("type"), "count"))
      {
         mainKeyword->Type = "numeric";
         mainKeyword->Minimum = 0;
         mainKeyword->Maximum = 16777215;
      }

      //RFXLAN IO
      if(boost::starts_with(msg.getBodyValue("device"), "io"))
      {
         mainKeyword->Type = "enumeration";
         mainKeyword->Parameters = "high|low";
      }

      //Mertik
      if(boost::starts_with(msg.getBodyValue("device"), "0x") && boost::iequals(msg.getBodyValue("type"), "mertik"))
      {
         mainKeyword->Type = "enumeration";
         mainKeyword->Parameters = "on|off|step_up|step_down|run_up|run_down|stop";
      }

      //Oregon
      if(isOregonDevice(msg))
      {
         if(boost::iequals(msg.getBodyValue("type"), "humidity"))
         {
            mainKeyword->Type = "numeric";
            mainKeyword->Minimum = 0;
            mainKeyword->Maximum =100;

            boost::shared_ptr<database::entities::CKeyword> descriptionKeyword(new database::entities::CKeyword());
            descriptionKeyword->Name = "description";
            descriptionKeyword->Type = "enumeration";
            descriptionKeyword->Parameters = "normal|comfort|dry|wet";
            keywords.push_back(descriptionKeyword);
         }

         if(boost::iequals(msg.getBodyValue("type"), "status"))
         {
            mainKeyword->Type = "enumeration";
            mainKeyword->Parameters = "normal|comfort|dry|wet";
         }

         if(boost::iequals(msg.getBodyValue("type"), "temp") ||
            boost::iequals(msg.getBodyValue("type"), "rainrate") ||
            boost::iequals(msg.getBodyValue("type"), "raintotal") ||
            boost::iequals(msg.getBodyValue("type"), "gust") ||
            boost::iequals(msg.getBodyValue("type"), "average_speed") ||
            boost::iequals(msg.getBodyValue("type"), "weight") ||
            boost::iequals(msg.getBodyValue("type"), "power") ||
            boost::iequals(msg.getBodyValue("type"), "energy"))
         {
            mainKeyword->Type = "numeric";
         }

         if(boost::iequals(msg.getBodyValue("type"), "pressure"))
         {
            mainKeyword->Type = "numeric";
            boost::shared_ptr<database::entities::CKeyword> forecastKeyword(new database::entities::CKeyword());
            forecastKeyword->Name = "forecast";
            forecastKeyword->Type = "enumeration";
            forecastKeyword->Parameters = "sunny|partly cloudy|cloudy|rain";
            keywords.push_back(forecastKeyword);
         }

         if(boost::iequals(msg.getBodyValue("type"), "direction"))
         {
            mainKeyword->Minimum = 0;
            mainKeyword->Maximum = 359;
            mainKeyword->Type = "numeric";
         }

         if(boost::iequals(msg.getBodyValue("type"), "uv"))
         {
            mainKeyword->Minimum = 0;
            mainKeyword->Maximum = 12;

            boost::shared_ptr<database::entities::CKeyword> descriptionKeyword(new database::entities::CKeyword());
            descriptionKeyword->Name = "description";
            descriptionKeyword->Type = "enumeration";
            descriptionKeyword->Parameters = "low|medium|high|very high|dangerous";
            keywords.push_back(descriptionKeyword);
         }

         if(boost::starts_with(msg.getBodyValue("device"), "elec") && boost::iequals(msg.getBodyValue("type"), "current"))
         {
            mainKeyword->Units = "A";
            mainKeyword->Type = "numeric";
         }

      }

      keywords.push_back(mainKeyword);

      return keywords;

   }


   bool CSensorBasic::isOregonDevice(shared::xpl::CXplMessage & msg)
   {
      std::string device = boost::to_lower_copy(msg.getBodyValue("device"));
      return boost::starts_with(device, "temp1") ||
         boost::starts_with(device, "temp2") ||
         boost::starts_with(device, "temp3") ||
         boost::starts_with(device, "temp4") ||
         boost::starts_with(device, "th1") ||
         boost::starts_with(device, "th2") ||
         boost::starts_with(device, "th3") ||
         boost::starts_with(device, "th4") ||
         boost::starts_with(device, "th5") ||
         boost::starts_with(device, "th6") ||
         boost::starts_with(device, "thb1") ||
         boost::starts_with(device, "thb2") ||
         boost::starts_with(device, "rain1") ||
         boost::starts_with(device, "rain2") ||
         boost::starts_with(device, "wind1") ||
         boost::starts_with(device, "wind2") ||
         boost::starts_with(device, "wind3") ||
         boost::starts_with(device, "uv1") ||
         boost::starts_with(device, "uv2") ||
         boost::starts_with(device, "dt1") ||
         boost::starts_with(device, "weight1") ||
         boost::starts_with(device, "weight2") ||
         boost::starts_with(device, "elec1") ||
         boost::starts_with(device, "elec2") ||
         boost::starts_with(device, "elec3") ||
         boost::starts_with(device, "elec4");
   }


} //namespace rfxLanXpl
} //namespace rules
} //namespace communication