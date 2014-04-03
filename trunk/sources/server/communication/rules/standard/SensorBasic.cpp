#include "stdafx.h"
#include "SensorBasic.h"

namespace communication { namespace rules { namespace standard {

   CSensorBasic::CSensorBasic()
   {
   }
   
   CSensorBasic::~CSensorBasic()
   {
   }
   
   const DeviceIdentifier CSensorBasic::getDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return msg.getBodyValue("device");
   }
   
   MessageContent CSensorBasic::extractMessageData(shared::xpl::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair(msg.getBodyValue("type"), msg.getBodyValue("current")));

      std::map<std::string, std::string>::const_iterator currentKeyValuePair;

      //on extrait toutes les données sauf celles connues
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
      
   std::vector< boost::shared_ptr<database::entities::CKeyword> > CSensorBasic::identifyKeywords(shared::xpl::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<database::entities::CKeyword> > keywords;

      boost::shared_ptr<database::entities::CKeyword> mainKeyword(new database::entities::CKeyword());

      //COMMON
      mainKeyword->Name = msg.getBodyValue("type");
      if(msg.getBody().find("units") != msg.getBody().end())
         mainKeyword->Units = msg.getBodyValue("units");


      std::map<std::string, std::string>::const_iterator currentKeyValuePair;

      //chaque mot clé contenu dans le message (autre que device, type, current et units sont des mots clés spécifiues au device)
      //doit etre traité comme fournissant une donnée
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

      return keywords;
   }

   std::string CSensorBasic::rewriteKeyword(const std::string & keyword)
   {
      if(boost::iequals(keyword, "temperature"))
         return "temp";
      return keyword;
   }

} //namespace standard
} //namespace rules
} //namespace communication