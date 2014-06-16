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

      if (msg.getBodyValue("type") == "ninja")
         keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword("mertik", "mertik-maxitrol-gas-fire", shared::plugin::yadomsApi::IYadomsApi::kReadWrite, shared::plugin::yadomsApi::IYadomsApi::kString, "", "left, right, up, down, p1, prog_p1, p2, prog_p2, p3, prog_p3, p4, prog_p4, center, prog_center, sweep, prog_sweep")));
      else if (msg.getBodyValue("type") == "mertik")
         keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword("ninja", "Ninja-Robocam", shared::plugin::yadomsApi::IYadomsApi::kReadWrite, shared::plugin::yadomsApi::IYadomsApi::kString, "", "on, off, step_up, step_down, run_up, run_down, stop")));
      else if (msg.getBodyValue("type") == "output")
         keywords.push_back(boost::shared_ptr<CDeviceKeyword>(new CDeviceKeyword("iostate", "digitalio", shared::plugin::yadomsApi::IYadomsApi::kReadWrite, shared::plugin::yadomsApi::IYadomsApi::kString, "", "high, low, toggle")));

      return keywords;
   }

   std::string CControlBasic::generateVirtualDeviceIdentifier()
   {
      return (boost::format("%1%") % shared::tools::CRandom::generateRandomNumber<32>(false)).str();
   }

   // [END] ICommandRule implemntation
} //namespace rfxLanXpl
} //namespace xplrules
