#include "stdafx.h"
#include "RuleFactory.h"
#include "SensorBasic.h"
#include "X10Basic.h"

namespace communication { namespace rules { namespace standard {

   CRuleFactory::CRuleFactory()
   {
   }

   CRuleFactory::~CRuleFactory()
   {
   }



   // IFactory implementation
   bool CRuleFactory::isHandled(shared::xpl::CXplMessage & message)
   {
      return isHandled(message.getSource().toString()) || isHandled(message.getTarget().toString());
   }

   boost::shared_ptr<IRule> CRuleFactory::identifyRule(shared::xpl::CXplMessage & msg, CRuleInstanceManager & instanceManager)
   {
      return identifyRule(msg.getMessageSchemaIdentifier().toString(), instanceManager);
   }
      
   bool CRuleFactory::isHandled(database::entities::CDevice & device)
   {
      return isHandled(device.HardwareIdentifier);
   }

   boost::shared_ptr<IRule> CRuleFactory::identifyRule(database::entities::CDevice & device, CRuleInstanceManager & instanceManager)
   {
      return identifyRule(device.Protocol, instanceManager);
   }

   shared::xpl::CXplActor CRuleFactory::identifyXplActor(database::entities::CDevice & device)
   {
      std::vector<std::string> splittedHardwareIdentifier;
      boost::split(splittedHardwareIdentifier, device.HardwareIdentifier(), boost::is_any_of("-."), boost::token_compress_on);

      if(splittedHardwareIdentifier.size() == 3)
      {
         return shared::xpl::CXplActor::createActor(splittedHardwareIdentifier[0], splittedHardwareIdentifier[1], splittedHardwareIdentifier[2]);
      }
      else
      {
         return shared::xpl::CXplActor::createBroadcastActor();
      }
   }

   // [END] IFactory implementation




   bool CRuleFactory::isHandled(const std::string & hardwareName)
   {
      //this factory is for the Xpl Standard
      //so all messages are handled
      return true;
   }


   boost::shared_ptr<IRule> CRuleFactory::identifyRule(const std::string & protocolName, CRuleInstanceManager & instanceManager)
   {
      if(boost::iequals(protocolName, "x10.basic"))
         return instanceManager.getRule<CX10Basic>();

      if(boost::iequals(protocolName, "sensor.basic"))
         return instanceManager.getRule<CSensorBasic>();

      //not handled
      return boost::shared_ptr<IRule>();
   }

} //namespace standard
} //namespace rules
} //namespace communication