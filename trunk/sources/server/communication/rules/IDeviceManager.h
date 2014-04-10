#pragma once
#include "IRule.h"
#include "RuleInstanceManager.hpp"
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplActor.h>

namespace communication { namespace rules {

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


      //------------------------------------------
      ///\brief   Check if the hardware is handled by 
      ///\param [in] hardwareIdentifier : the hardware identifier
      ///\return true if the hardware identifier is managed by this manager
      //------------------------------------------
      virtual bool matchHardware(const std::string & hardwareIdentifier) = 0;

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
   
   
} //namespace rules
} //namespace communication

