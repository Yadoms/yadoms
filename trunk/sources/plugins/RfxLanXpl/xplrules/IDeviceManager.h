#pragma once
#include "IRule.h"
#include "RuleInstanceManager.hpp"
#include "../xplcore/XplMessage.h"
#include "../xplcore/XplActor.h"

namespace xplrules {

   //------------------------------------------
   ///\brief   Interface for Device manager
   //------------------------------------------
   class IDeviceManager
   {
   public:
      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~IDeviceManager() {}
      
      //------------------------------------------
      ///\brief   Check if the message is handled by this factory
      ///\param [in] hardwareIdentifier : the hardware identifier
      ///\return true if the message is handled
      //------------------------------------------
      virtual bool isHandled(const std::string & hardwareIdentifier) = 0;

      //------------------------------------------
      ///\brief   Get the rule to use for treating message
      ///\param [in] protocol : the protocol to handle
      ///\param [in] instanceManager : the instance manager
      ///\return  A shared_ptr to the rule which can handle the message
      //------------------------------------------
      virtual boost::shared_ptr<IRule> identifyRule(const std::string & protocol, CRuleInstanceManager & instanceManager) = 0;

      //--------------------------------
      ///\brief Get the list of handled protocols
      ///\return the list of handled protocols
      //--------------------------------
      virtual std::vector<std::string> getHandledProtocols() = 0;

      //--------------------------------
      ///\brief generate a random virtual device identifier (i.e. : 0x123456-2)
      ///\param [in]  protocolIdentifier the protocol
      ///\param [in] instanceManager : the instance manager
      ///\return a virtual device identifier
      //--------------------------------
      virtual std::string generateVirtualDeviceIdentifier(const std::string & protocolIdentifier, CRuleInstanceManager & instanceManager) = 0;

   };
   
   
} //namespace xplrules


