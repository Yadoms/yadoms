#pragma once
#include "IRule.h"
#include "RuleInstanceManager.hpp"
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplActor.h>

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


      //------------------------------------------
      ///\brief   Check if the device is handled by this factory
      ///\param [in] device : the device to handle
      ///\return true if the device is handled
      //------------------------------------------
      virtual bool isHandled(database::entities::CDevice & device) = 0;

      //--------------------------------
      ///\brief Get the rule for a device
      ///\param [in]  msg The device
      ///\param [in] instanceManager : the instance manager
      ///\return the rule (may be null)
      //--------------------------------
      virtual boost::shared_ptr<IRule> identifyRule(database::entities::CDevice & device, CRuleInstanceManager & instanceManager) = 0;

      //--------------------------------
      ///\brief Identify the Xpl actor for the device
      ///\param [in]  device The device
      ///\return the xplActor
      //--------------------------------
      virtual shared::xpl::CXplActor identifyXplActor(database::entities::CDevice & device)=0;
   };
   
   
} //namespace rules
} //namespace communication

