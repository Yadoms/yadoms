#include "stdafx.h"
#include "X10Basic.h"
#include <shared/tools/Random.h>
#include <shared/exception/NotSupported.hpp>

namespace xplrules
{
   namespace rfxLanXpl
   {
      xplcore::CXplMessageSchemaIdentifier CX10Basic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("x10.basic");
      std::string CX10Basic::m_keywordCommand = "command";
      std::string CX10Basic::m_keywordLevel = "level";
      std::string CX10Basic::m_keywordProtocol = "protocol";
      std::string CX10Basic::m_keywordDevice = "device";

      DECLARE_ENUM_IMPLEMENTATION_NESTED(CX10Basic::EProtocol, EProtocol,
         ((X10))
         ((Arc))
         ((Flamingo))
         ((Koppla))
         ((Waveman))
         ((Harrison))
         ((He105))
         ((Rts10))
      );

      DECLARE_ENUM_IMPLEMENTATION_NESTED(CX10Basic::ECommands, ECommands,
         ((On)("on"))
         ((Off)("off"))
         ((Dim)("dim"))
         ((Bright)("bright"))
         ((AllLightsOn)("all_lights_on"))
         ((AllLightsOff)("all_lights_off"))
      );


      CX10Basic::CX10Basic()
      {
      }

      CX10Basic::~CX10Basic()
      {
      }

      // IRule implemntation
      xplcore::CXplMessageSchemaIdentifier CX10Basic::getProtocol()
      {
         return m_protocol;
      }

      std::string CX10Basic::getDeviceCommercialName(const EProtocol& protocol)
      {
         std::string commercialName;
         switch (protocol)
         {
         case EProtocol::kArcValue:
            commercialName = "KlikAanKlikUit, Chacon, HomeEasy, DomiaLite, Domia, ByeByeStandBy, ELRO AB600, NEXA Proove Intertechno Duwi";
            break;

         case EProtocol::kFlamingoValue:
            commercialName = "Flamingo";
            break;

         case EProtocol::kKopplaValue:
            commercialName = "Ikea Koppla";
            break;

         case EProtocol::kHe105Value:
            commercialName = "HomeEasy";
            break;

         case EProtocol::kRts10Value:
            commercialName = "Digimax";
            break;

         case EProtocol::kHarrisonValue:
            commercialName = "Harrison";
            break;
         }
         return commercialName;
      }


      CDeviceIdentifier CX10Basic::getDeviceAddressFromMessage(xplcore::CXplMessage& msg)
      {
         auto commercialName = msg.getBodyValue(m_keywordDevice);


         auto protocol = EProtocol::kX10;

         if (msg.getBody().find(m_keywordProtocol) != msg.getBody().end())
         {
            protocol = msg.getBodyValue(m_keywordProtocol);

            auto commercialNameFromProtocol = getDeviceCommercialName(protocol);
            if (!commercialNameFromProtocol.empty())
               commercialName = commercialNameFromProtocol;
         }

         shared::CDataContainerSharedPtr innerDetails = new_CDataContainerSharedPtr();
         innerDetails->set("x10protocol", protocol);

         return CDeviceIdentifier(msg.getBodyValue(m_keywordDevice), commercialName, m_protocol, m_protocol, innerDetails);
      }


      KeywordList CX10Basic::identifyKeywords(xplcore::CXplMessage& msg)
      {
         KeywordList keywords;

         //by default procotol is x10
         auto protocol = EProtocol::kX10;

         //if protocol is specified, then take it
         if (msg.getBody().find(m_keywordProtocol) != msg.getBody().end())
            protocol = msg.getBodyValue(m_keywordProtocol);

         switch (protocol)
         {
         case EProtocol::kKopplaValue:
            keywords.push_back(boost::make_shared<shared::plugin::yPluginApi::historization::CDimmable>(m_keywordCommand));
            break;

         case EProtocol::kHarrisonValue:
            keywords.push_back(boost::make_shared<shared::plugin::yPluginApi::historization::CCurtain>(m_keywordCommand));
            break;

         default:
            keywords.push_back(boost::make_shared<shared::plugin::yPluginApi::historization::CSwitch>(m_keywordCommand));
            break;
         }
         return keywords;
      }

      // [END] IRule implemntation


      // IReadRule implemntation
      MessageContent CX10Basic::extractMessageData(xplcore::CXplMessage& msg)
      {
         MessageContent data;


         //by default procotol is x10
         auto protocol = EProtocol::kX10;

         //if protocol is specified, then take it
         if (msg.getBody().find(m_keywordProtocol) != msg.getBody().end())
            protocol = msg.getBodyValue(m_keywordProtocol);

         ECommands command(msg.getBodyValue(m_keywordCommand));

         switch (protocol)
         {
         case EProtocol::kKopplaValue:
            {
               auto swDimmableData(boost::make_shared<shared::plugin::yPluginApi::historization::CDimmable>(m_keywordCommand));
               switch (command)
               {
               case ECommands::kOnValue:
               case ECommands::kAllLightsOnValue:
               case ECommands::kBrightValue:
                  swDimmableData->set(true);

                  if (msg.getBody().find(m_keywordLevel) != msg.getBody().end())
                     swDimmableData->set(boost::lexical_cast<int>(msg.getBodyValue(m_keywordLevel)));
                  break;
               case ECommands::kOffValue:
               case ECommands::kAllLightsOffValue:
                  swDimmableData->set(false);
                  break;
               case ECommands::kDimValue:
                  swDimmableData->set(boost::lexical_cast<int>(msg.getBodyValue(m_keywordLevel)));
                  break;
               default:
                  throw shared::exception::CNotSupported(command.toString());
               }
               data.push_back(swDimmableData);
            }
            break;

         case EProtocol::kHarrisonValue:
            {
               auto curtain(boost::make_shared<shared::plugin::yPluginApi::historization::CCurtain>(m_keywordCommand));
               switch (command)
               {
               case ECommands::kOnValue:
                  curtain->set(shared::plugin::yPluginApi::historization::ECurtainCommand::kOpen);
                  break;
               case ECommands::kOffValue:
                  curtain->set(shared::plugin::yPluginApi::historization::ECurtainCommand::kClose);
                  break;
               case ECommands::kDimValue:
               case ECommands::kBrightValue:
                  curtain->set(shared::plugin::yPluginApi::historization::ECurtainCommand::kStop);
                  break;
               default:
                  throw shared::exception::CNotSupported(command.toString());
               }
               data.push_back(curtain);
            }
            break;

         default:
            {
               auto swData(boost::make_shared<shared::plugin::yPluginApi::historization::CSwitch>(m_keywordCommand));
               switch (command)
               {
               case ECommands::kOnValue:
               case ECommands::kAllLightsOnValue:
                  swData->set(true);
                  break;
               case ECommands::kOffValue:
               case ECommands::kAllLightsOffValue:
                  swData->set(false);
                  break;
               default:
                  throw shared::exception::CNotSupported(command.toString());
               }
               data.push_back(swData);
            }
            break;
         }

         return data;
      }

      // [END] IReadRule implemntation

      // ICommandRule implemntation
      boost::shared_ptr<xplcore::CXplMessage> CX10Basic::createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand>& commandData,
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

         auto protocol = innerDetails->get<EProtocol>("x10protocol");

         switch (protocol)
         {
         case EProtocol::kKopplaValue:
            {
               shared::plugin::yPluginApi::historization::CDimmable swDimmableCommand(m_keywordCommand);
               swDimmableCommand.set(commandData->getBody());

               newMessage->addToBody(m_keywordProtocol, EProtocol::kKoppla.toString());
               if (swDimmableCommand.isOn())
               {
                  newMessage->addToBody(m_keywordCommand, ECommands::kOn.toString());
                  newMessage->addToBody(m_keywordLevel, boost::lexical_cast<std::string>(swDimmableCommand.switchLevel()));
               }
               else
               {
                  newMessage->addToBody(m_keywordCommand, ECommands::kOff.toString());
               }
            }
            break;

         case EProtocol::kHarrisonValue:
            {
               shared::plugin::yPluginApi::historization::CCurtain curtainCommand(m_keywordCommand);
               curtainCommand.set(shared::plugin::yPluginApi::historization::ECurtainCommand(commandData->getBody()));

               newMessage->addToBody(m_keywordProtocol, EProtocol::kHarrison.toString());
               switch (curtainCommand.get())
               {
               case shared::plugin::yPluginApi::historization::ECurtainCommand::kOpenValue:
                  newMessage->addToBody(m_keywordCommand, ECommands::kOn.toString());
                  break;
               case shared::plugin::yPluginApi::historization::ECurtainCommand::kCloseValue:
                  newMessage->addToBody(m_keywordCommand, ECommands::kOff.toString());
                  break;
               case shared::plugin::yPluginApi::historization::ECurtainCommand::kStopValue:
                  newMessage->addToBody(m_keywordCommand, ECommands::kDim.toString());
                  break;
               }
            }
            break;

         case EProtocol::kX10Value:
            {
               //in case of x10 protocol, do not add protocol name in xpl message
               shared::plugin::yPluginApi::historization::CSwitch swCommand(m_keywordCommand);
               swCommand.setCommand(commandData->getBody());
               if (swCommand.get())
                  newMessage->addToBody(m_keywordCommand, ECommands::kOn.toString());
               else
                  newMessage->addToBody(m_keywordCommand, ECommands::kOff.toString());
            }
            break;

         default:
            {
               shared::plugin::yPluginApi::historization::CSwitch swCommand(m_keywordCommand);
               swCommand.setCommand(commandData->getBody());
               newMessage->addToBody(m_keywordProtocol, protocol.toString());
               if (swCommand.get())
                  newMessage->addToBody(m_keywordCommand, ECommands::kOn.toString());
               else
                  newMessage->addToBody(m_keywordCommand, ECommands::kOff.toString());
            }
            break;
         }
         return newMessage;
      }

      std::string CX10Basic::generateVirtualDeviceIdentifier()
      {
         return (boost::format("%1%") % shared::tools::CRandom::generate()).str();
      }

      // [END] ICommandRule implemntation


      CDeviceContainer CX10Basic::generateDeviceParameters(shared::CDataContainerSharedPtr& configuration) const
      {
         auto chosenProtocol = configuration->get<std::string>("Protocol.activeSection");

         auto deviceId = configuration->get<std::string>("Protocol.content." + chosenProtocol + ".content.HouseCode") + configuration->get<std::string>("Protocol.content." + chosenProtocol + ".content.UnitCode");


         EProtocol p;

         if (boost::istarts_with(chosenProtocol, "proto_arc"))
            p = EProtocol::kArc;
         else if (boost::istarts_with(chosenProtocol, "proto_flamingo"))
            p = EProtocol::kFlamingo;
         else if (boost::istarts_with(chosenProtocol, "proto_koppla"))
            p = EProtocol::kKoppla;
         else if (boost::istarts_with(chosenProtocol, "proto_waveman"))
            p = EProtocol::kWaveman;
         else if (boost::istarts_with(chosenProtocol, "proto_harrison"))
            p = EProtocol::kHarrison;
         else if (boost::istarts_with(chosenProtocol, "proto_he105"))
            p = EProtocol::kHe105;
         else if (boost::istarts_with(chosenProtocol, "proto_rts10"))
            p = EProtocol::kRts10;
         else
            throw shared::exception::CNotSupported("The procotol " + chosenProtocol + " is not supported");

         auto commercialName = getDeviceCommercialName(p);
         if (commercialName.empty())
            commercialName = deviceId;

         shared::CDataContainerSharedPtr innerDetails = new_CDataContainerSharedPtr();
         innerDetails->set("x10protocol", p);

         CDeviceIdentifier device(deviceId, commercialName, m_protocol, m_protocol, innerDetails);

         CDeviceContainer dc(device);

         switch (p)
         {
         case EProtocol::kKopplaValue:
            dc.addKeyword(boost::make_shared<shared::plugin::yPluginApi::historization::CDimmable>(m_keywordCommand));
            break;

         case EProtocol::kHarrisonValue:
            dc.addKeyword(boost::make_shared<shared::plugin::yPluginApi::historization::CCurtain>(m_keywordCommand));
            break;

         default:
            dc.addKeyword(boost::make_shared<shared::plugin::yPluginApi::historization::CSwitch>(m_keywordCommand));
            break;
         }
         return dc;
      }
   } //namespace rfxLanXpl
} //namespace xplrules


