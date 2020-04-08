#include "stdafx.h"
#include "ControlBasic.h"
#include <shared/tools/Random.h>
#include "SensorBasic.h"
#include "data/MertikCommand.h"
#include "data/Ninja.h"
#include "data/DigitalIoCommand.h"

namespace xplrules
{
   namespace rfxLanXpl
   {
      xplcore::CXplMessageSchemaIdentifier CControlBasic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("control.basic");

      std::string CControlBasic::m_keywordCurrent = "current";
      std::string CControlBasic::m_keywordType = "type";
      std::string CControlBasic::m_keywordDevice = "device";

      DECLARE_ENUM_IMPLEMENTATION_NESTED(CControlBasic::EType, EType,
         ((Ninja))
         ((Mertik))
         ((Output))
      );

      CControlBasic::CControlBasic()
      {
      }

      CControlBasic::~CControlBasic()
      {
      }

      // IRule implementation
      xplcore::CXplMessageSchemaIdentifier CControlBasic::getProtocol()
      {
         return m_protocol;
      }

      CDeviceIdentifier CControlBasic::getDeviceAddressFromMessage(xplcore::CXplMessage& msg)
      {
         auto commercialName = msg.getBodyValue(m_keywordDevice);

         EType type(msg.getBodyValue(m_keywordType));
         switch (type)
         {
         case EType::kNinjaValue:
            commercialName = "X10 Ninja/Robocam";
            break;
         case EType::kMertikValue:
            commercialName = "Mertik";
            break;
         case EType::kOutputValue:
            commercialName = "RFXLAN I/O line";
            break;
         }

         shared::CDataContainerSharedPtr innerDetails = new_CDataContainerSharedPtr();
         innerDetails->set("type", type);
         return CDeviceIdentifier(msg.getBodyValue(m_keywordDevice),
                                  commercialName,
                                  CSensorBasic::getProtocol(),
                                  m_protocol, innerDetails);
      }

      KeywordList CControlBasic::identifyKeywords(xplcore::CXplMessage& msg)
      {
         KeywordList keywords;

         EType type(msg.getBodyValue(m_keywordType));
         switch (type)
         {
         case EType::kNinjaValue:
            keywords.push_back(boost::make_shared<data::CNinja>("ninja"));
            break;
         case EType::kMertikValue:
            keywords.push_back(boost::make_shared<data::CMertickCommand>("mertik"));
            break;
         case EType::kOutputValue:
            keywords.push_back(boost::make_shared<data::CDigitalIoCommand>("digitalio"));
            break;
         }
         return keywords;
      }

      // [END] IRule implementation


      // ICommandRule implemntation
      boost::shared_ptr<xplcore::CXplMessage> CControlBasic::createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand>& commandData,
                                                                              const std::string& rfxAddress,
                                                                              const shared::CDataContainerSharedPtr& innerDetails)
      {
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

         auto type = innerDetails->get<EType>("type");
         switch (type)
         {
         case EType::kMertikValue:
            {
               data::CMertickCommand command("mertik");
               command.set(data::CMertickCommand(commandData->getBody()));
               newMessage->addToBody(m_keywordType, type.toString());
               newMessage->addToBody(m_keywordCurrent, command.get().toString());
            }
            break;

         case EType::kNinjaValue:
            {
               data::CNinja command("ninja");
               command.set(data::CNinja(commandData->getBody()));
               newMessage->addToBody(m_keywordType, type.toString());
               newMessage->addToBody(m_keywordCurrent, command.get().toString());
            }

         case EType::kOutputValue:
            {
               data::CDigitalIoCommand command("digitalio");
               command.set(data::CDigitalIoCommand(commandData->getBody()));
               newMessage->addToBody(m_keywordType, type.toString());
               newMessage->addToBody(m_keywordCurrent, command.get().toString());
            }
            break;
         }
         return newMessage;
      }

      std::string CControlBasic::generateVirtualDeviceIdentifier()
      {
         return (boost::format("%1%") % shared::tools::CRandom::generate()).str();
      }

      // [END] ICommandRule implemntation
   } //namespace rfxLanXpl
} //namespace xplrules


