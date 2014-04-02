#include "stdafx.h"
#include "RuleFactory.h"
#include "AcBasic.h"
#include "SensorBasic.h"

namespace communication { namespace rules { namespace rfxLanXpl {

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
   // [END] IFactory implementation
   
   

   bool CRuleFactory::isHandled(const std::string & hardwareName)
   {
      //this factory is for the RfxComLan with Xpl firmware
      return boost::starts_with(hardwareName, "rfxcom-lan.");
   }

   shared::xpl::CXplActor CRuleFactory::identifyXplActor(database::entities::CDevice & device)
   {
      std::vector<std::string> splittedHardwareIdentifier;
      boost::split(splittedHardwareIdentifier, device.HardwareIdentifier(), boost::is_any_of("-."), boost::token_compress_on);

      boost::split(splittedHardwareIdentifier, device.HardwareIdentifier(), boost::is_any_of("-."), boost::token_compress_on);

      if(splittedHardwareIdentifier.size() == 3)
      {
         return shared::xpl::CXplActor::createActor(splittedHardwareIdentifier[0], splittedHardwareIdentifier[1], splittedHardwareIdentifier[2]);
      }
      else
      {
         if(splittedHardwareIdentifier.size()>0)
            return shared::xpl::CXplActor::createActor("rfxcom", "lan", splittedHardwareIdentifier[splittedHardwareIdentifier.size()-1]);
         else
            throw shared::exception::CException("invalid hardware identifier");
      }

      
   }

   boost::shared_ptr<IRule> CRuleFactory::identifyRule(const std::string & protocolName, CRuleInstanceManager & instanceManager)
   {
      if(boost::iequals(protocolName, "ac.basic"))
         return instanceManager.getRule<CAcBasic>();
      
      if(boost::iequals(protocolName, "sensor.basic"))
         return instanceManager.getRule<CSensorBasic>();
         
      //not handled by RfxLanXpl or using standard rule
      return boost::shared_ptr<IRule>();
   }


} //namespace rfxLanXpl
} //namespace rules
} //namespace communication