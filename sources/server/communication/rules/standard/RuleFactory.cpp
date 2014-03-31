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
   
   bool CRuleFactory::isHandled(shared::xpl::CXplMessage & message)
   {
      //this factory is for the Xpl Standard
      //so all messages are handled
      return true;
   }

   // IFactory implementation
   boost::shared_ptr<IRule> CRuleFactory::identifyRule(shared::xpl::CXplMessage & msg, CRuleInstanceManager & instanceManager)
   {
      if(boost::iequals(msg.getMessageSchemaIdentifier().toString(), "x10.basic"))
         return instanceManager.getRule<CX10Basic>();
         
      if(boost::iequals(msg.getMessageSchemaIdentifier().toString(), "sensor.basic"))
         return instanceManager.getRule<CSensorBasic>();
      
      //not handled
      return boost::shared_ptr<IRule>();
   }
   // [END] IFactory implementation
   
} //namespace standard
} //namespace rules
} //namespace communication