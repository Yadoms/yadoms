#include "stdafx.h"
#include "AcBasic.h"
#include <shared/tools/Random.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/historization/Switch.h>


namespace xplrules { namespace rfxLanXpl {
   
   DECLARE_ENUM_IMPLEMENTATION_NESTED(CAcBasic::EState, EState,
      ((Off))
      ((On))
      ((Dim))
   );
      
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

   KeywordList CAcBasic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      KeywordList keywords;
      keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new shared::plugin::yadomsApi::historization::CSwitch(m_keywordCommand)));
      return keywords;
   }

   // [END] IRule implementation


   // IReadRule implemntation
   MessageContent CAcBasic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;
      try
      {
         EState valFromEquipment(msg.getBodyValue(m_keywordCommand));
        
         boost::shared_ptr<shared::plugin::yadomsApi::historization::CSwitch> sw(new shared::plugin::yadomsApi::historization::CSwitch(m_keywordCommand));
         switch (valFromEquipment)
         {
         case EState::kOnValue:
            sw->set(true);
            break;
         case EState::kOffValue:
            sw->set(false);
            break;
         case EState::kDimValue:
            sw->set(boost::lexical_cast<int>(msg.getBodyValue(m_keywordLevel)));
            break;
         }
         data.push_back(sw);
      }
      catch (...)
      {

      }
      
      return data;
   }
   // [END] IReadRule implementation


   // ICommandRule implementation
   boost::shared_ptr< xplcore::CXplMessage > CAcBasic::createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand> & commandData, const std::string & rfxAddress)
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
      shared::plugin::yadomsApi::historization::CSwitch commandDetails(commandData->getBody().serialize());

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

      //set the device address and unit (parse from argetDevice.Address)
      newMessage->addToBody(m_keywordAddress, splittedAddress[0]);
      newMessage->addToBody(m_keywordUnit, splittedAddress[1]);


      if (commandDetails.switchLevel() == 0)
      {
         //set the command
         EState s = EState::kOff;
         newMessage->addToBody(m_keywordCommand, s);
      }
      else if (commandDetails.switchLevel() == 100)
      {
         //set the command
         EState s = EState::kOn;
         newMessage->addToBody(m_keywordCommand, s);
      }
      else
      {
         //set the command
         EState s = EState::kDim;
         newMessage->addToBody(m_keywordCommand, s);
         newMessage->addToBody(m_keywordLevel, boost::lexical_cast<std::string>(commandDetails.switchLevel()));
      }

      return newMessage;
   }
   

   std::string CAcBasic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("0x%1$08X-%2%") % shared::tools::CRandom::generateRandomNumber<26>(false) % shared::tools::CRandom::generateRandomNumber<4>(false)).str();
   }

   // [END] ICommandRule implemntation



} //namespace rfxLanXpl
} //namespace xplrules
