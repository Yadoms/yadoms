#pragma once
#include "IRule.h"
#include "RuleInstanceManager.hpp"
#include <shared/xpl/XplMessage.h>

namespace communication { namespace rules {

   //------------------------------------------
   ///\brief   Interface for Rule factories
   //------------------------------------------
   class IFactory
   {
   public:
      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~IFactory() {}
      
      //------------------------------------------
      ///\brief   Check if the message is handled by this factory
      ///\param [in] message : the message to handle
      ///\return true if the message is handled
      //------------------------------------------
      virtual bool isHandled(shared::xpl::CXplMessage & message) = 0;

      //------------------------------------------
      ///\brief   Get the rule to use for treating message
      ///\param [in] message : the message to handle
      ///\param [in] instanceManager : the instance manager
      ///\return  A shared_ptr to the rule which can handle the message
      //------------------------------------------
      virtual boost::shared_ptr<IRule> identifyRule(shared::xpl::CXplMessage & message, CRuleInstanceManager & instanceManager) = 0;
   };
   
   
} //namespace rules
} //namespace communication

