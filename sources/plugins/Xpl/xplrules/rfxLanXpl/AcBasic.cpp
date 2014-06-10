#include "stdafx.h"
#include "AcBasic.h"
#include <shared/tools/Random.h>
//#include "communication/rules/KeywordManager.h"

namespace xplrules { namespace rfxLanXpl {


   std::string CAcBasic::m_keywordAddress = "address";
   std::string CAcBasic::m_keywordUnit = "unit";
   std::string CAcBasic::m_keywordCommand = "command";
   std::string CAcBasic::m_keywordCommandValues = "{ values : [on, off, preset] }";
   std::string CAcBasic::m_keywordLevel = "level";
   xplcore::CXplMessageSchemaIdentifier  CAcBasic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("ac.basic");

   CAcBasic::CAcBasic()
   {
   }

   CAcBasic::~CAcBasic()
   {
   }

   // IRule implementation
   const xplcore::CXplMessageSchemaIdentifier CAcBasic::getProtocol()
   {
      return m_protocol;
   }

   const CDeviceIdentifier CAcBasic::getDeviceAddressFromMessage(xplcore::CXplMessage & msg)
   {
      std::string deviceId = msg.getBodyValue(m_keywordAddress) + "-" + msg.getBodyValue(m_keywordUnit);
      std::string commercialName = "ANSLUT, Chacon, DI.O, KlikAanKlikUit, NEXA, Proove, Intertechno, Düwi, HomeEasy UK/EU";
      return CDeviceIdentifier(deviceId, commercialName, m_protocol, m_protocol);
   }

   MessageContent CAcBasic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair(m_keywordCommand, msg.getBodyValue(m_keywordCommand)));
      if(msg.getBody().find(m_keywordLevel) != msg.getBody().end())
         data.insert(std::make_pair(m_keywordLevel, msg.getBodyValue(m_keywordLevel)));
      return data;
   }


   std::vector< boost::shared_ptr<CDeviceKeyword> > CAcBasic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<CDeviceKeyword> > keywords;

      keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword(m_keywordCommand, m_keywordCommand, shared::plugin::yadomsApi::IYadomsApi::kReadWrite, m_keywordCommandValues)));

      boost::property_tree::ptree details;
      details.put("min", 0);
      details.put("max", 15);
      keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword(m_keywordLevel, m_keywordLevel, shared::plugin::yadomsApi::IYadomsApi::kReadWrite, details)));

      return keywords;
   }

   // [END] IRule implementation



   // ICommandRule implemntation
   /*
   boost::shared_ptr< xplcore::CXplMessage > CAcBasic::createXplCommand(database::entities::CDevice & targetDevice, command::CDeviceCommand & deviceCommand)
   {
      ////////////////////////////
      // do some checks
      ////////////////////////////

      //check the device address is valid
      std::string address = targetDevice.Address;
      std::vector<std::string> splittedAddress;
      boost::split(splittedAddress, address, boost::is_any_of("-"), boost::token_compress_on);

      if(splittedAddress.size() != 2)
      {
         throw shared::exception::CException("ac.basic protocol needs a device address matching pattern : <address>-<unit> ");
      }

      //check the command value
      communication::command::CDeviceCommand::CommandData content = deviceCommand.getCommandData();
      if(content.find(m_keywordCommand) == content.end())
      {
         throw shared::exception::CException( (boost::format("ac.basic protocol needs a parameter '%1%'") % m_keywordCommand).str());
      }

      if(!CKeywordManager::isEnumerationValue(content[m_keywordCommand], m_keywordCommandValues))
      {
         throw shared::exception::CException( (boost::format("ac.basic protocol needs a parameter '%1%' that match one off : '%2%' ( '|' separated )") % m_keywordCommand % m_keywordCommandValues).str());
      }



      ////////////////////////////
      // create the message
      ////////////////////////////

      //create the message
      boost::shared_ptr< xplcore::CXplMessage > newMessage(new xplcore::CXplMessage());

      //the AC.BSACI XplMessage if a xpl-trig
      newMessage->setTypeIdentifier(xplcore::CXplMessage::kXplCommand);

      //set hop to 1
      newMessage->setHop(1);

      //set the target (rfxcom-lan-<hax mac address>)
      newMessage->setTarget(xplcore::CXplActor::parse(targetDevice.HardwareIdentifier()));

      //set the ac.basic
      newMessage->setMessageSchemaIdentifier(xplcore::CXplMessageSchemaIdentifier("ac", "basic"));

      //set the device addesss and unit (parse from argetDevice.Address)
      newMessage->addToBody(m_keywordAddress, splittedAddress[0]);
      newMessage->addToBody(m_keywordUnit, splittedAddress[1]);

      //set the command
      newMessage->addToBody(m_keywordCommand, content[m_keywordCommand]);

      //if there is any other data to send, just add key/value to bidy
      communication::command::CDeviceCommand::CommandData::const_iterator i;
      for(i=content.begin(); i!=content.end(); ++i)
      {
         if(!boost::iequals(i->first, m_keywordCommand))
            newMessage->addToBody(i->first, i->second);
      }

      return newMessage;
   }
   */

   std::string CAcBasic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("0x%1$08X-%2%") % shared::tools::CRandom::generateRandomNumber<26>(false) % shared::tools::CRandom::generateRandomNumber<4>(false)).str();
   }

   // [END] ICommandRule implemntation



} //namespace rfxLanXpl
} //namespace xplrules
