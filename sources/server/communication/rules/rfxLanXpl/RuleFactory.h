#pragma once

#include "../IFactory.h"

namespace communication { namespace rules { namespace rfxLanXpl {

   class CRuleFactory : public IFactory
   {
   public:
      CRuleFactory();
      virtual ~CRuleFactory();
      
      // IFactory implementation
      virtual bool isHandled(shared::xpl::CXplMessage & message);
      virtual boost::shared_ptr<IRule> identifyRule(shared::xpl::CXplMessage & msg, CRuleInstanceManager & instanceManager);
      // [END] IFactory implementation
   };
   
} //namespace rfxLanXpl
} //namespace rules
} //namespace communication