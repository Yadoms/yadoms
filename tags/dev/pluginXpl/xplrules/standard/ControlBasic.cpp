#include "stdafx.h"
#include "ControlBasic.h"
#include <shared/tools/Random.h>
#include "SensorBasic.h"

namespace xplrules { namespace standard {

   xplcore::CXplMessageSchemaIdentifier  CControlBasic::m_protocol = xplcore::CXplMessageSchemaIdentifier::parse("control.basic");

   CControlBasic::CControlBasic()
   {
   }

   CControlBasic::~CControlBasic()
   {
   }

   const xplcore::CXplMessageSchemaIdentifier CControlBasic::getProtocol()
   {
      return m_protocol;
   }


   const CDeviceIdentifier CControlBasic::getDeviceAddressFromMessage(xplcore::CXplMessage & msg)
   {
      return CDeviceIdentifier(msg.getBodyValue("device"), msg.getBodyValue("device"), CSensorBasic::getProtocol(), m_protocol);
   }

   MessageContent CControlBasic::extractMessageData(xplcore::CXplMessage & msg)
   {
      MessageContent data;
      data.insert(std::make_pair("command", msg.getBodyValue("command")));
      if(msg.getBody().find("level") != msg.getBody().end())
         data.insert(std::make_pair("level", msg.getBodyValue("level")));
      if(msg.getBody().find("protocol") != msg.getBody().end())
         data.insert(std::make_pair("protocol", msg.getBodyValue("protocol")));
      return data;
   }

   
   std::vector< boost::shared_ptr<CDeviceKeyword> > CControlBasic::identifyKeywords(xplcore::CXplMessage & msg)
   {
      std::vector< boost::shared_ptr<CDeviceKeyword> > keywords;
      keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword("current", "current", shared::plugin::yadomsApi::IYadomsApi::kReadWrite, shared::plugin::yadomsApi::IYadomsApi::kString, "", "")));
      return keywords;
   }

   std::string CControlBasic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("%1%") % shared::tools::CRandom::generateRandomNumber<32>(false)).str();
   }

   // [END] ICommandRule implemntation



   // ICommandRule implemntation
   boost::shared_ptr< xplcore::CXplMessage > CControlBasic::createXplCommand(boost::shared_ptr<yApi::IDeviceCommand> & commandData, const std::string & rfxAddress)
   {
      ////////////////////////////
      // create the message
      ////////////////////////////

      //create the message
      boost::shared_ptr< xplcore::CXplMessage > newMessage(new xplcore::CXplMessage());

      //TODO : implement it

      return newMessage;
   }
   // [END] ICommandRule implemntation


} //namespace standard
} //namespace xplrules
