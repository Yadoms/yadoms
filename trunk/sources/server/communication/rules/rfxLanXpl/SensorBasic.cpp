#include "stdafx.h"
#include "SensorBasic.h"

namespace communication { namespace rules { namespace rfxLanXpl {

   CSensorBasic::CSensorBasic()
   {
   }

   CSensorBasic::~CSensorBasic()
   {
   }

   const DeviceIdentifier CSensorBasic::GetDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return msg.getBodyValue("device");
   }

   MessageContent CSensorBasic::ExtractMessageData(shared::xpl::CXplMessage & msg)
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
      mainKeyword->setName(msg.getBodyValue("type"));
      if(msg.getBody().find("units") != msg.getBody().end())
         mainKeyword->setUnits(msg.getBodyValue("units"));

      if(boost::iequals(msg.getBodyValue("type"), "battery"))
      {
         mainKeyword->setType("numeric");
         mainKeyword->setMinimum(0);
         mainKeyword->setMaximum(100);
      }

      //DIGIMAX
      if(boost::starts_with(msg.getBodyValue("device"), "digimax"))
      {
         if(msg.getBodyValue("type") == "demand")
         {
            mainKeyword->setType("enumeration");
            mainKeyword->setParameters("heater_on|heater_off|cooling_on|cooling_off");
         }
         else
         {
            mainKeyword->setType("numeric");
         }
      }

      //RFXSENSOR
      if(boost::starts_with(msg.getBodyValue("device"), "rfxsensor"))
      {
         if(boost::iequals(msg.getBodyValue("type"), "voltage"))
         {
            if(!mainKeyword->isUnitsFilled())
               mainKeyword->setUnits("volt");
         }
         mainKeyword->setType("numeric");
      }

      //RFXMETER
      if(boost::starts_with(msg.getBodyValue("device"), "rfxmeter") && boost::iequals(msg.getBodyValue("type"), "count"))
      {
         mainKeyword->setType("numeric");
         mainKeyword->setMinimum(0);
         mainKeyword->setMaximum(16777215);
      }

      //RFXLAN IO
      if(boost::starts_with(msg.getBodyValue("device"), "io"))
      {
         mainKeyword->setType("enumeration");
         mainKeyword->setParameters("high|low");
      }

      //Mertik
      if(boost::starts_with(msg.getBodyValue("device"), "0x") && boost::iequals(msg.getBodyValue("type"), "mertik"))
      {
         mainKeyword->setType("enumeration");
         mainKeyword->setParameters("on|off|step_up|step_down|run_up|run_down|stop");
      }

      //Oregon
      if(isOregonDevice(msg))
      {
         if(boost::iequals(msg.getBodyValue("type"), "humidity"))
         {
            mainKeyword->setType("numeric");
            mainKeyword->setMinimum(0);
            mainKeyword->setMaximum(100);

            boost::shared_ptr<database::entities::CKeyword> descriptionKeyword(new database::entities::CKeyword());
            descriptionKeyword->setName("description");
            descriptionKeyword->setType("enumeration");
            descriptionKeyword->setParameters("normal|comfort|dry|wet");
            keywords.push_back(descriptionKeyword);
         }

         if(boost::iequals(msg.getBodyValue("type"), "status"))
         {
            mainKeyword->setType("enumeration");
            mainKeyword->setParameters("normal|comfort|dry|wet ");
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
            mainKeyword->setType("numeric");
         }

         if(boost::iequals(msg.getBodyValue("type"), "pressure"))
         {
            mainKeyword->setType("numeric");
            boost::shared_ptr<database::entities::CKeyword> forecastKeyword(new database::entities::CKeyword());
            forecastKeyword->setName("forecast");
            forecastKeyword->setType("enumeration");
            forecastKeyword->setParameters("sunny|partly cloudy|cloudy|rain");
            keywords.push_back(forecastKeyword);
         }

         if(boost::iequals(msg.getBodyValue("type"), "direction"))
         {
            mainKeyword->setMinimum(0);
            mainKeyword->setMaximum(359);
            mainKeyword->setType("numeric");
         }

         if(boost::iequals(msg.getBodyValue("type"), "uv"))
         {
            mainKeyword->setMinimum(0);
            mainKeyword->setMaximum(12);

            boost::shared_ptr<database::entities::CKeyword> descriptionKeyword(new database::entities::CKeyword());
            descriptionKeyword->setName("description");
            descriptionKeyword->setType("enumeration");
            descriptionKeyword->setParameters("low|medium|high|very high|dangerous");
            keywords.push_back(descriptionKeyword);
         }

         if(boost::starts_with(msg.getBodyValue("device"), "elec") && boost::iequals(msg.getBodyValue("type"), "current"))
         {
            mainKeyword->setUnits("A");
            mainKeyword->setType("numeric");
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