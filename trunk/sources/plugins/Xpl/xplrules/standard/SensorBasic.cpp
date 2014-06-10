#include "stdafx.h"
#include "SensorBasic.h"

namespace xplrules { namespace standard {

   xplcore::CXplMessageSchemaIdentifier  CSensorBasic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("sensor.basic");

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
      //TODO : manage control.basic rule to use it here
      return CDeviceIdentifier(msg.getBodyValue("device"), msg.getBodyValue("device"), m_protocol, xplcore::CXplMessageSchemaIdentifier::parse("control.basic"));
   }
   
   MessageContent CSensorBasic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair(msg.getBodyValue("type"), msg.getBodyValue("current")));

      std::map<std::string, std::string>::const_iterator currentKeyValuePair;

      // Extraction of all known data expected
      for(currentKeyValuePair = msg.getBody().begin(); currentKeyValuePair!=msg.getBody().end(); ++currentKeyValuePair)
      {
         if(!boost::iequals(currentKeyValuePair->first, "device") &&
            !boost::iequals(currentKeyValuePair->first, "type") &&
            !boost::iequals(currentKeyValuePair->first, "current") &&
            !boost::iequals(currentKeyValuePair->first, "units"))
         {
            data.insert(std::make_pair(currentKeyValuePair->first, currentKeyValuePair->second));
         }
      }

      return data;
   }
      
   std::vector< boost::shared_ptr<CDeviceKeyword> > CSensorBasic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<CDeviceKeyword> > keywords;

      /*
      boost::shared_ptr<database::entities::CKeyword> mainKeyword(new database::entities::CKeyword());

      //COMMON
      mainKeyword->Name = msg.getBodyValue("type");
      if(msg.getBody().find("units") != msg.getBody().end())
         mainKeyword->Units = msg.getBodyValue("units");


      std::map<std::string, std::string>::const_iterator currentKeyValuePair;

      // Each keyword contained in message (other than device, type, current et units) is keyword specific to the device.
      // It have to provide data.
      for(currentKeyValuePair = msg.getBody().begin(); currentKeyValuePair!=msg.getBody().end(); ++currentKeyValuePair)
      {
         if(!boost::iequals(currentKeyValuePair->first, "device") &&
            !boost::iequals(currentKeyValuePair->first, "type") &&
            !boost::iequals(currentKeyValuePair->first, "current") &&
            !boost::iequals(currentKeyValuePair->first, "units"))
         {
            boost::shared_ptr<database::entities::CKeyword> descriptionKeyword(new database::entities::CKeyword());
            descriptionKeyword->Name = rewriteKeyword(currentKeyValuePair->first);
            keywords.push_back(descriptionKeyword);
         }
      }
      */
      return keywords;
   }

   std::string CSensorBasic::rewriteKeyword(const std::string & keyword)
   {
      if(boost::iequals(keyword, "temperature"))
         return "temp";
      return keyword;
   }

} //namespace standard
} //namespace xplrules
