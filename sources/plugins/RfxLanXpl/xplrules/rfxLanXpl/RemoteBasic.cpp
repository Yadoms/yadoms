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
   const xplcore::CXplMessageSchemaIdentifier CRemoteBasic::getProtocol()
   {
      return m_protocol;
   }

   const CDeviceIdentifier CRemoteBasic::getDeviceAddressFromMessage(xplcore::CXplMessage & msg)
   {
      std::string commercialName = msg.getBodyValue(m_keywordDevice);
      std::string fullDevice = msg.getBodyValue(m_keywordDevice);

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

      std::string fullDevice = msg.getBodyValue(m_keywordDevice);
      if (boost::istarts_with(fullDevice, ERemoteType::kPC.toString()))
         keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new data::CRemotePC("pcremote")));
      else if (boost::istarts_with(fullDevice, ERemoteType::kMedion.toString()))
         keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new data::CRemoteMedion("medionremote")));
      else if (boost::istarts_with(fullDevice, ERemoteType::kAtiPlus.toString()))
         keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new data::CRemoteAtiWonderPlus("atiplusremote")));
      else if (boost::istarts_with(fullDevice, ERemoteType::kAti.toString()))
         keywords.push_back(boost::shared_ptr< shared::plugin::yadomsApi::historization::IHistorizable >(new data::CRemoteAtiWonder("atiremote")));

      return keywords;
   }
   // [END] IRule implemntation


   // IReadRule implemntation
   MessageContent CRemoteBasic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;
      
      std::string fullDevice = msg.getBodyValue(m_keywordDevice);
      if (boost::istarts_with(fullDevice, ERemoteType::kPC.toString()))
      {
         boost::shared_ptr< data::CRemotePC > remote(new data::CRemotePC("pcremote"));
         data::ERemotePCCodes code = boost::lexical_cast<int>(msg.getBodyValue(m_keywordKeys));
         remote->set(code);
         data.push_back(remote);
      }
      else if (boost::istarts_with(fullDevice, ERemoteType::kMedion.toString()))
      {
         boost::shared_ptr< data::CRemoteMedion > remote(new data::CRemoteMedion("medionremote"));
         data::ERemoteMedionCodes code = boost::lexical_cast<int>(msg.getBodyValue(m_keywordKeys));
         remote->set(code);
         data.push_back(remote);
      }
      else if (boost::istarts_with(fullDevice, ERemoteType::kAtiPlus.toString()))
      {
         boost::shared_ptr< data::CRemoteAtiWonderPlus > remote(new data::CRemoteAtiWonderPlus("atiplusremote"));
         data::ERemoteAtiWonderPlusCodes code = boost::lexical_cast<int>(msg.getBodyValue(m_keywordKeys));
         remote->set(code);
         data.push_back(remote);
      }
      else if (boost::istarts_with(fullDevice, ERemoteType::kAti.toString()))
      {
         boost::shared_ptr< data::CRemoteAtiWonder > remote(new data::CRemoteAtiWonder("atiremote"));
         data::ERemoteAtiWonderCodes code = boost::lexical_cast<int>(msg.getBodyValue(m_keywordKeys));
         remote->set(code);
         data.push_back(remote);
      }

      return data;
   }
   // [END] IReadRule implemntation

   // ICommandRule implemntation
   boost::shared_ptr< xplcore::CXplMessage > CRemoteBasic::createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand> & commandData, const std::string & rfxAddress, const shared::CDataContainer & innerDetails)
   {
     
      ////////////////////////////
      // create the message
      ////////////////////////////
      
      //check the device address is valid
      std::string device = commandData->getTargetDevice();


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
      newMessage->addToBody(m_keywordDevice, device);

      int code = 0;
      if (boost::istarts_with(device, ERemoteType::kPC.toString()))
      {
         data::CRemotePC remote("pcremote");
         remote.set(commandData->getBody());
         code = remote.get().toInteger();
      }
      else if (boost::istarts_with(device, ERemoteType::kMedion.toString()))
      {
         data::CRemoteMedion remote("medionremote");
         remote.set(commandData->getBody());
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
         data::CRemoteAtiWonder remote("atiremote");
         remote.set(commandData->getBody());
         code = remote.get().toInteger();
      }
      else
         throw shared::exception::CNotSupported("remote type not supported");

      //convert int to hex string
      std::stringstream stream;
      stream << "0x" << std::hex << code;
      std::string hexNumber(stream.str());
      newMessage->addToBody(m_keywordKeys, hexNumber);

      return newMessage;
   }

   std::string CRemoteBasic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("%1%") % shared::tools::CRandom::generateRandomNumber<32>(false)).str();
   }
   // [END] ICommandRule implemntation


   const CDeviceContainer CRemoteBasic::generateDeviceParameters(shared::CDataContainer & configuration) const
   {
      throw shared::exception::CException("fail to generate a device from user configuration.");
   }

} //namespace rfxLanXpl
} //namespace xplrules
