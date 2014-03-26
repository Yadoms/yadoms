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
   
   bool CRuleFactory::isHandled(shared::xpl::CXplMessage & message)
   {
      //this factory is for the Xpl Standard
      //so all messages are handled
      return boost::starts_with(message.getSource().toString(), "rfxcom-lan.");
   }

   // IFactory implementation
   boost::shared_ptr<IRule> CRuleFactory::identifyRule(shared::xpl::CXplMessage & msg, CRuleInstanceManager & instanceManager)
   {
      if(boost::iequals(msg.getMessageSchemaIdentifier().toString(), "ac.basic"))
         return instanceManager.getRule<CAcBasic>();
      
      if(boost::iequals(msg.getMessageSchemaIdentifier().toString(), "sensor.basic"))
         return instanceManager.getRule<CSensorBasic>();
         
      //not handled by RfxLanXpl or using standard rule
      return boost::shared_ptr<IRule>();
   }
   // [END] IFactory implementation
   
} //namespace rfxLanXpl
} //namespace rules
} //namespace communication