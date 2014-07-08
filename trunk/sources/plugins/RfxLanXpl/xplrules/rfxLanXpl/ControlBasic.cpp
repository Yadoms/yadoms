#include "stdafx.h"
#include "ControlBasic.h"
#include <shared/tools/Random.h>
#include "SensorBasic.h"

namespace xplrules { namespace rfxLanXpl {

   xplcore::CXplMessageSchemaIdentifier  CControlBasic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("control.basic");

   CControlBasic::CControlBasic()
   {
   }

   CControlBasic::~CControlBasic()
   {
   }

   // IRule implementation
   const xplcore::CXplMessageSchemaIdentifier CControlBasic::getProtocol()
   {
      return m_protocol;
   }
   
   const CDeviceIdentifier CControlBasic::getDeviceAddressFromMessage(xplcore::CXplMessage & msg)
   {
      std::string commercialName = msg.getBodyValue("device");

      if (msg.getBodyValue("type") == "ninja")
         commercialName = "X10 Ninja/Robocam";

      if (msg.getBodyValue("type") == "mertik")
         commercialName = "Mertik";

      if (msg.getBodyValue("type") == "output")
         commercialName = "RFXLAN I/O line";

      return CDeviceIdentifier(msg.getBodyValue("device"), commercialName, CSensorBasic::getProtocol(), m_protocol);
   }

   std::vector< boost::shared_ptr<CDeviceKeyword> > CControlBasic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<CDeviceKeyword> > keywords;

      if (msg.getBodyValue("type") == "ninja")
         keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword("mertik", "mertik-maxitrol-gas-fire", shared::plugin::yadomsApi::kSet, shared::plugin::yadomsApi::kString, "", "left, right, up, down, p1, prog_p1, p2, prog_p2, p3, prog_p3, p4, prog_p4, center, prog_center, sweep, prog_sweep")));
      else if (msg.getBodyValue("type") == "mertik")
         keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword("ninja", "Ninja-Robocam", shared::plugin::yadomsApi::kSet, shared::plugin::yadomsApi::kString, "", "on, off, step_up, step_down, run_up, run_down, stop")));
      else if (msg.getBodyValue("type") == "output")
         keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword("iostate", "digitalio", shared::plugin::yadomsApi::kSet, shared::plugin::yadomsApi::kString, "", "high, low, toggle")));

      return keywords;
   }
   // [END] IRule implementation




   // ICommandRule implemntation
   boost::shared_ptr< xplcore::CXplMessage > CControlBasic::createXplCommand(boost::shared_ptr<yApi::IDeviceCommand> & commandData, const std::string & rfxAddress)
   {
      boost::shared_ptr< xplcore::CXplMessage > newMessage(new xplcore::CXplMessage());

      ////////////////////////////
      // do some checks
      ////////////////////////////
      /*
      //check the device address is valid
      std::string address = commandData->getTargetDevice();
      std::vector<std::string> splittedAddress;
      boost::split(splittedAddress, address, boost::is_any_of("-"), boost::token_compress_on);

      if (splittedAddress.size() != 2)
      {
         throw shared::exception::CException("ac.basic protocol needs a device address matching pattern : <address>-<unit> ");
      }

      //read command details (may throw exception if something is wrong)
      //commands::CAcBasic commandDetails(commandData->getBody());

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
      */
      return newMessage;
   }

   std::string CControlBasic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("%1%") % shared::tools::CRandom::generateRandomNumber<32>(false)).str();
   }

   // [END] ICommandRule implemntation
} //namespace rfxLanXpl
} //namespace xplrules
