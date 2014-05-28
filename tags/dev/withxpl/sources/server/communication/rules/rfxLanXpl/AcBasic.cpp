#include "stdafx.h"
#include "AcBasic.h"
#include "tools/Random.h"
#include "communication/rules/KeywordManager.h"

namespace communication { namespace rules { namespace rfxLanXpl {


   std::string CAcBasic::m_keywordAddress = "address";
   std::string CAcBasic::m_keywordUnit = "unit";
   std::string CAcBasic::m_keywordCommand = "command";
   std::string CAcBasic::m_keywordCommandValues = "on|off|preset";
   std::string CAcBasic::m_keywordLevel = "level";

   CAcBasic::CAcBasic()
   {
   }

   CAcBasic::~CAcBasic()
   {
   }

   // IRule implementation
   const CDeviceIdentifier CAcBasic::getDeviceAddressFromMessage(shared::xpl::CXplMessage & msg)
   {
      return CDeviceIdentifier(msg.getBodyValue(m_keywordAddress) + "-" +  msg.getBodyValue(m_keywordUnit));
   }

   MessageContent CAcBasic::extractMessageData(shared::xpl::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair(m_keywordCommand, msg.getBodyValue(m_keywordCommand)));
      if(msg.getBody().find(m_keywordLevel) != msg.getBody().end())
         data.insert(std::make_pair(m_keywordLevel, msg.getBodyValue(m_keywordLevel)));
      return data;
   }

   std::vector< boost::shared_ptr<database::entities::CKeyword> > CAcBasic::identifyKeywords(shared::xpl::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<database::entities::CKeyword> > keywords;
      //command
      keywords.push_back(CKeywordManager::createEnumeration(m_keywordCommand, m_keywordCommandValues));

      //level
      keywords.push_back(CKeywordManager::createNumeric(m_keywordLevel, 0, 15));
      return keywords;
   }
   // [END] IRule implementation



   // ICommandRule implemntation
   boost::shared_ptr< shared::xpl::CXplMessage > CAcBasic::createXplCommand(database::entities::CDevice & targetDevice, command::CDeviceCommand & deviceCommand)
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
      boost::shared_ptr< shared::xpl::CXplMessage > newMessage(new shared::xpl::CXplMessage());

      //the AC.BSACI XplMessage if a xpl-trig
      newMessage->setTypeIdentifier(shared::xpl::CXplMessage::kXplCommand);

      //set hop to 1
      newMessage->setHop(1);

      //set the target (rfxcom-lan-<hax mac address>)
      newMessage->setTarget(shared::xpl::CXplActor::parse(targetDevice.HardwareIdentifier()));

      //set the ac.basic
      newMessage->setMessageSchemaIdentifier(shared::xpl::CXplMessageSchemaIdentifier("ac", "basic"));

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

   std::string CAcBasic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("0x%1$08X-%2%") % tools::CRandom::generateRandomNumber<26>(false) % tools::CRandom::generateRandomNumber<4>(false)).str();
   }

   // [END] ICommandRule implemntation



} //namespace rfxLanXpl
} //namespace rules
} //namespace communication