#include "stdafx.h"
#include "AcBasic.h"
#include <shared/tools/Random.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include "commands/AcBasic.h"

namespace xplrules { namespace rfxLanXpl {


   std::string CAcBasic::m_keywordAddress = "address";
   std::string CAcBasic::m_keywordUnit = "unit";
   std::string CAcBasic::m_keywordCommand = "command";
   std::string CAcBasic::m_keywordCommandValues = "{ \"values\" : [\"on\", \"off\", \"preset\"] }";
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

   std::vector< boost::shared_ptr<CDeviceKeyword> > CAcBasic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<CDeviceKeyword> > keywords;

      keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword(m_keywordCommand, yApi::CStandardCapacities::Switch, yApi::kWriteOnly, shared::plugin::yadomsApi::kNoData, shared::CStringExtension::EmptyString, m_keywordCommandValues)));
      

      shared::CDataContainer details;
      details.set("min", 0);
      details.set("max", 15);
      keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword(m_keywordLevel, m_keywordLevel, shared::plugin::yadomsApi::kReadWrite, shared::plugin::yadomsApi::kInteger, shared::CStringExtension::EmptyString, details)));

      return keywords;
   }

   // [END] IRule implementation


   // IReadRule implemntation
   MessageContent CAcBasic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair(m_keywordCommand, msg.getBodyValue(m_keywordCommand)));
      if (msg.getBody().find(m_keywordLevel) != msg.getBody().end())
         data.insert(std::make_pair(m_keywordLevel, msg.getBodyValue(m_keywordLevel)));
      return data;
   }
   // [END] IReadRule implementation


   // ICommandRule implemntation
   boost::shared_ptr< xplcore::CXplMessage > CAcBasic::createXplCommand(boost::shared_ptr<yApi::IDeviceCommand> & commandData, const std::string & rfxAddress)
   {
      ////////////////////////////
      // do some checks
      ////////////////////////////

      //check the device address is valid
      std::string address = commandData->getTargetDevice();
      std::vector<std::string> splittedAddress;
      boost::split(splittedAddress, address, boost::is_any_of("-"), boost::token_compress_on);

      if(splittedAddress.size() != 2)
      {
         throw shared::exception::CException("ac.basic protocol needs a device address matching pattern : <address>-<unit> ");
      }

      //read command details (may throw exception if something is wrong)
      commands::CAcBasic commandDetails(commandData->getBody());

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
      newMessage->setTarget(xplcore::CXplActor::parse(rfxAddress));

      //set the ac.basic
      newMessage->setMessageSchemaIdentifier(getProtocol());

      //set the device addesss and unit (parse from argetDevice.Address)
      newMessage->addToBody(m_keywordAddress, splittedAddress[0]);
      newMessage->addToBody(m_keywordUnit, splittedAddress[1]);

      //set the command
      newMessage->addToBody(m_keywordCommand, commandDetails.Command());

      //if there is any other data to send, just add key/value to bidy
      if (commandDetails.Level.isDefined())
         newMessage->addToBody(m_keywordLevel, boost::lexical_cast<std::string>(commandDetails.Level()));

      return newMessage;
   }
   

   std::string CAcBasic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("0x%1$08X-%2%") % shared::tools::CRandom::generateRandomNumber<26>(false) % shared::tools::CRandom::generateRandomNumber<4>(false)).str();
   }

   // [END] ICommandRule implemntation



} //namespace rfxLanXpl
} //namespace xplrules
