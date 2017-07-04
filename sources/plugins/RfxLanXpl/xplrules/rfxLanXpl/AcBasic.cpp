#include "stdafx.h"
#include "AcBasic.h"
#include <shared/tools/Random.h>
#include <shared/plugin/yPluginApi/historization/Dimmable.h>


namespace xplrules
{
   namespace rfxLanXpl
   {
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
      xplcore::CXplMessageSchemaIdentifier CAcBasic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("ac.basic");

      CAcBasic::CAcBasic()
      {
      }

      CAcBasic::~CAcBasic()
      {
      }

      // IRule implementation
      xplcore::CXplMessageSchemaIdentifier CAcBasic::getProtocol()
      {
         return m_protocol;
      }

      CDeviceIdentifier CAcBasic::getDeviceAddressFromMessage(xplcore::CXplMessage& msg)
      {
         std::string deviceId = msg.getBodyValue(m_keywordAddress) + "-" + msg.getBodyValue(m_keywordUnit);
         std::string commercialName("ANSLUT, Chacon, DI.O, KlikAanKlikUit, NEXA, Proove, Intertechno, Düwi, HomeEasy UK/EU");
         return CDeviceIdentifier(deviceId, commercialName, m_protocol, m_protocol);
      }

      KeywordList CAcBasic::identifyKeywords(xplcore::CXplMessage& msg)
      {
         KeywordList keywords;
         keywords.push_back(boost::make_shared<shared::plugin::yPluginApi::historization::CDimmable>(m_keywordCommand));
         return keywords;
      }

      // [END] IRule implementation


      // IReadRule implemntation
      MessageContent CAcBasic::extractMessageData(xplcore::CXplMessage& msg)
      {
         MessageContent data;
         try
         {
            EState valFromEquipment(msg.getBodyValue(m_keywordCommand));

            auto sw(boost::make_shared<shared::plugin::yPluginApi::historization::CDimmable>(m_keywordCommand));
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
      boost::shared_ptr<xplcore::CXplMessage> CAcBasic::createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand>& commandData,
                                                                         const std::string& rfxAddress,
                                                                         const shared::CDataContainer& innerDetails)
      {
         ////////////////////////////
         // do some checks
         ////////////////////////////

         //check the device address is valid
         std::string address = commandData->getDevice();
         std::vector<std::string> splittedAddress;
         boost::split(splittedAddress, address, boost::is_any_of("-"), boost::token_compress_on);

         if (splittedAddress.size() != 2)
         {
            throw shared::exception::CException("ac.basic protocol needs a device address matching pattern : <address>-<unit> ");
         }

         //read command details (may throw exception if something is wrong)
         shared::plugin::yPluginApi::historization::CDimmable commandDetails("command");
         commandDetails.set(commandData->getBody());

         ////////////////////////////
         // create the message
         ////////////////////////////

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
         newMessage->addToBody(m_keywordAddress, splittedAddress[0]);
         newMessage->addToBody(m_keywordUnit, splittedAddress[1]);


         if (commandDetails.switchLevel() == 0)
         {
            //set the command
            auto s = EState::kOff;
            newMessage->addToBody(m_keywordCommand, s);
         }
         else if (commandDetails.switchLevel() == 100)
         {
            //set the command
            auto s = EState::kOn;
            newMessage->addToBody(m_keywordCommand, s);
         }
         else
         {
            //set the command
            auto s = EState::kDim;
            newMessage->addToBody(m_keywordCommand, s);
            newMessage->addToBody(m_keywordLevel, boost::lexical_cast<std::string>(commandDetails.switchLevel()));
         }

         return newMessage;
      }


      std::string CAcBasic::generateVirtualDeviceIdentifier()
      {
         return (boost::format("0x%1$08X-%2%") % shared::tools::CRandom::generate() % shared::tools::CRandom::generate()).str();
      }

      // [END] ICommandRule implemntation


      CDeviceContainer CAcBasic::generateDeviceParameters(shared::CDataContainer& configuration) const
      {
         auto deviceId = configuration.get<std::string>("deviceAddress");
         std::string commercialName("ANSLUT, Chacon, DI.O, KlikAanKlikUit, NEXA, Proove, Intertechno, Düwi, HomeEasy UK/EU");

         CDeviceIdentifier device(deviceId, commercialName, m_protocol, m_protocol);

         CDeviceContainer dc(device);
         dc.addKeyword(boost::make_shared<shared::plugin::yPluginApi::historization::CDimmable>(m_keywordCommand));
         return dc;
      }
   } //namespace rfxLanXpl
} //namespace xplrules


