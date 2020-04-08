#include "stdafx.h"
#include "RemoteBasic.h"
#include <shared/tools/Random.h>
#include <shared/exception/NotSupported.hpp>
#include "data/RemotePC.h"
#include "data/RemoteMedion.h"
#include "data/RemoteAtiWonder.h"
#include "data/RemoteAtiWonderPlus.h"

namespace xplrules { namespace rfxLanXpl {

   xplcore::CXplMessageSchemaIdentifier  CRemoteBasic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("x10.basic");
   std::string CRemoteBasic::m_keywordKeys = "keys";
   std::string CRemoteBasic::m_keywordDevice = "device";

   DECLARE_ENUM_IMPLEMENTATION_NESTED(CRemoteBasic::ERemoteType, ERemoteType,
      ((PC))
      ((Medion))
      ((Ati))
      ((AtiPlus))
   )

   CRemoteBasic::CRemoteBasic()
   {
   }

   CRemoteBasic::~CRemoteBasic()
   {
   }

   // IRule implemntation
   xplcore::CXplMessageSchemaIdentifier CRemoteBasic::getProtocol()
   {
      return m_protocol;
   }

   CDeviceIdentifier CRemoteBasic::getDeviceAddressFromMessage(xplcore::CXplMessage & msg)
   {
      auto commercialName = msg.getBodyValue(m_keywordDevice);
      auto fullDevice = msg.getBodyValue(m_keywordDevice);

      if (boost::istarts_with(fullDevice, ERemoteType::kPC.toString()))
         commercialName = "X10 PC Remote";
      else if (boost::istarts_with(fullDevice, ERemoteType::kMedion.toString()))
         commercialName = "Medion Remote";
      else if (boost::istarts_with(fullDevice, ERemoteType::kAtiPlus.toString()))
         commercialName = "ATI Wonder Plus remote";
      else if (boost::istarts_with(fullDevice, ERemoteType::kAti.toString()))
         commercialName = "ATI Wonder remote";
      
      return CDeviceIdentifier(fullDevice, commercialName, m_protocol, m_protocol);
   }

  
   KeywordList CRemoteBasic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      KeywordList keywords;

      auto fullDevice = msg.getBodyValue(m_keywordDevice);
      if (boost::istarts_with(fullDevice, ERemoteType::kPC.toString()))
         keywords.push_back(boost::make_shared<data::CRemotePC>("pcremote"));
      else if (boost::istarts_with(fullDevice, ERemoteType::kMedion.toString()))
         keywords.push_back(boost::make_shared<data::CRemoteMedion>("medionremote"));
      else if (boost::istarts_with(fullDevice, ERemoteType::kAtiPlus.toString()))
         keywords.push_back(boost::make_shared<data::CRemoteAtiWonderPlus>("atiplusremote"));
      else if (boost::istarts_with(fullDevice, ERemoteType::kAti.toString()))
         keywords.push_back(boost::make_shared<data::CRemoteAtiWonder>("atiremote"));

      return keywords;
   }
   // [END] IRule implemntation


   // IReadRule implemntation
   MessageContent CRemoteBasic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;

      auto fullDevice = msg.getBodyValue(m_keywordDevice);
      if (boost::istarts_with(fullDevice, ERemoteType::kPC.toString()))
      {
         auto remote(boost::make_shared<data::CRemotePC>("pcremote"));
         data::ERemotePCCodes code(boost::lexical_cast<int>(msg.getBodyValue(m_keywordKeys)));
         remote->set(code);
         data.push_back(remote);
      }
      else if (boost::istarts_with(fullDevice, ERemoteType::kMedion.toString()))
      {
         auto remote(boost::make_shared<data::CRemoteMedion>("medionremote"));
         data::ERemoteMedionCodes code(boost::lexical_cast<int>(msg.getBodyValue(m_keywordKeys)));
         remote->set(code);
         data.push_back(remote);
      }
      else if (boost::istarts_with(fullDevice, ERemoteType::kAtiPlus.toString()))
      {
         auto remote(boost::make_shared<data::CRemoteAtiWonderPlus>("atiplusremote"));
         data::ERemoteAtiWonderPlusCodes code(boost::lexical_cast<int>(msg.getBodyValue(m_keywordKeys)));
         remote->set(code);
         data.push_back(remote);
      }
      else if (boost::istarts_with(fullDevice, ERemoteType::kAti.toString()))
      {
         auto remote(boost::make_shared<data::CRemoteAtiWonder>("atiremote"));
         data::ERemoteAtiWonderCodes code(boost::lexical_cast<int>(msg.getBodyValue(m_keywordKeys)));
         remote->set(code);
         data.push_back(remote);
      }

      return data;
   }
   // [END] IReadRule implemntation

   // ICommandRule implemntation
   boost::shared_ptr< xplcore::CXplMessage > CRemoteBasic::createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand> & commandData,
      const std::string & rfxAddress,
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

      int code;
      if (boost::istarts_with(device, ERemoteType::kPC.toString()))
      {
         data::CRemotePC remote(commandData->getBody());
         code = remote.get().toInteger();
      }
      else if (boost::istarts_with(device, ERemoteType::kMedion.toString()))
      {
         data::CRemoteMedion remote(commandData->getBody());
         code = remote.get().toInteger();
      }
      else if (boost::istarts_with(device, ERemoteType::kAtiPlus.toString()))
      {
         throw shared::exception::CNotSupported("ATI Wonder Plus commands are not supported");
         /* uncomment this code to allow sending command
         data::CRemoteAtiWonderPlus remote("atiplusremote");
         remote.set(commandData->getBody());
         code = remote.get().toInteger();
         */
      }
      else if (boost::istarts_with(device, ERemoteType::kAti.toString()))
      {
         data::CRemoteAtiWonder remote(commandData->getBody());
         code = remote.get().toInteger();
      }
      else
         throw shared::exception::CNotSupported("remote type not supported");

      //convert int to hex string
      std::stringstream stream;
      stream << "0x" << std::hex << code;
      auto hexNumber(stream.str());
      newMessage->addToBody(m_keywordKeys, hexNumber);

      return newMessage;
   }

   std::string CRemoteBasic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("%1%") % shared::tools::CRandom::generate()).str();
   }
   // [END] ICommandRule implemntation


   CDeviceContainer CRemoteBasic::generateDeviceParameters(shared::CDataContainerSharedPtr& configuration) const
   {
      throw shared::exception::CException("fail to generate a device from user configuration.");
   }

} //namespace rfxLanXpl
} //namespace xplrules
