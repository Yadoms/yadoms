#include "stdafx.h"
#include "X10Security.h"
#include <shared/tools/Random.h>
#include "data/Security.h"

namespace xplrules
{
   namespace rfxLanXpl
   {
      xplcore::CXplMessageSchemaIdentifier CX10Security::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("x10.security");

      std::string CX10Security::m_keywordCommand = "command";
      std::string CX10Security::m_keywordDevice = "device";
      std::string CX10Security::m_keywordType = "type";
      std::string CX10Security::m_keywordTamper = "tamper";
      std::string CX10Security::m_keywordLowBattery = "low-battery";
      std::string CX10Security::m_keywordDelay = "delay";


      CX10Security::CX10Security()
      {
      }

      CX10Security::~CX10Security()
      {
      }

      // IRule implemntation
      xplcore::CXplMessageSchemaIdentifier CX10Security::getProtocol()
      {
         return m_protocol;
      }

      CDeviceIdentifier CX10Security::getDeviceAddressFromMessage(xplcore::CXplMessage& msg)
      {
         //by default : "x10 (deviceAddr)"
         auto commercialName = (boost::format("x10 (%1%)") % msg.getBodyValue(m_keywordDevice)).str();
         if (msg.getBody().find(m_keywordType) != msg.getBody().end())
         {
            //the type should not be manage as enum 
            auto type = msg.getBodyValue(m_keywordType);
            if (boost::iequals(type, "cn"))
               commercialName = "Chacon, Avidsen, NEXA smoke detector";
            else if (boost::icontains(type, "mct") || boost::icontains(type, "mcw"))
               commercialName = "Visonic PowerCode";
         }
         return CDeviceIdentifier(msg.getBodyValue(m_keywordDevice), commercialName, m_protocol, m_protocol);
      }


      KeywordList CX10Security::identifyKeywords(xplcore::CXplMessage& msg)
      {
         KeywordList keywords;
         keywords.push_back(boost::make_shared<data::CSecurity>("security", shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet));
         return keywords;
      }

      // [END] IRule implemntation


      // IReadRule implemntation
      MessageContent CX10Security::extractMessageData(xplcore::CXplMessage& msg)
      {
         MessageContent data;

         auto security(boost::make_shared<data::CSecurity>("security", shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet));

         security->setCommand(msg.getBodyValue(m_keywordCommand));

         if (msg.getBody().find(m_keywordTamper) != msg.getBody().end())
            security->setTampered(boost::iequals(msg.getBodyValue(m_keywordTamper), "true"));

         if (msg.getBody().find(m_keywordLowBattery) != msg.getBody().end())
            security->setLowBattery(boost::iequals(msg.getBodyValue(m_keywordLowBattery), "true"));

         if (msg.getBody().find(m_keywordDelay) != msg.getBody().end())
            security->setDelay(boost::lexical_cast<int>(msg.getBodyValue(m_keywordDelay)));

         data.push_back(security);
         return data;
      }

      // [END] IReadRule implemntation

      // ICommandRule implemntation
      boost::shared_ptr<xplcore::CXplMessage> CX10Security::createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand>& commandData,
                                                                             const std::string& rfxAddress,
                                                                             const boost::shared_ptr<shared::CDataContainer>& innerDetails)
      {
         ////////////////////////////
         // Parse command data
         ////////////////////////////
         data::CSecurity securityCommand("security", shared::plugin::yPluginApi::EKeywordAccessMode::kGet);
         securityCommand.set(commandData->getBody());

         //do some checks
         if (securityCommand.getCommand() == data::CSecurity::ECommands::kDark)
            throw shared::exception::COutOfRange("The command Dark should not be sent to device.");
         if (securityCommand.getCommand() == data::CSecurity::ECommands::kLight)
            throw shared::exception::COutOfRange("The command Light should not be sent to device.");

         ////////////////////////////
         // create the message
         ////////////////////////////

         //check the device address is valid
         auto device = commandData->getDevice();


         //create the message
         auto newMessage(boost::make_shared<xplcore::CXplMessage>());

         //the AC.BSACI XplMessage if a xpl-trig
         newMessage->setTypeIdentifier(xplcore::CXplMessage::kXplCommand);

         //set hop to 1
         newMessage->setHop(1);

         //set the target (rfxcom-lan-<hax mac address>)
         newMessage->setTarget(xplcore::CXplActor::parse(rfxAddress));

         //set the ac.basic
         newMessage->setMessageSchemaIdentifier(getProtocol());

         //set the device address and unit (parse from argetDevice.Address)
         newMessage->addToBody(m_keywordDevice, device);


         newMessage->addToBody(m_keywordCommand, securityCommand.getCommand().toString());

         if (securityCommand.getDelay().isDefined())
            newMessage->addToBody(m_keywordDelay, boost::lexical_cast<std::string>(securityCommand.getDelay()()));

         return newMessage;
      }

      std::string CX10Security::generateVirtualDeviceIdentifier()
      {
         return (boost::format("%1%") % shared::tools::CRandom::generate()).str();
      }

      // [END] ICommandRule implemntation


      CDeviceContainer CX10Security::generateDeviceParameters(boost::shared_ptr<shared::CDataContainer>& configuration) const
      {
         throw shared::exception::CException("fail to generate a device from user configuration.");
      }
   } //namespace rfxLanXpl
} //namespace xplrules


