#pragma once
#include "IDeviceManager.h"
#include "RuleInstanceManager.hpp"
#include "communication/command/DeviceCommand.h"

namespace communication { namespace rules {

   //--------------------------------
   ///\brief class which handle rules
   //--------------------------------
   class CRulerFactory
   {
   public:
      //--------------------------------
      ///\brief Constructor
      //--------------------------------
      CRulerFactory();

      //--------------------------------
      ///\brief Destructor
      //--------------------------------
      virtual ~CRulerFactory();
      

      //--------------------------------
      ///\brief Get the rule from an XplMessage
      ///\param [in]  msg The message to extract the rule from
      ///\return the rule (may be null)
      //--------------------------------
      boost::shared_ptr<IRule> identifyRule(shared::xpl::CXplMessage & msg);
         
      //--------------------------------
      ///\brief Get the rule for a device
      ///\param [in]  msg The device
      ///\return the rule (may be null)
      //--------------------------------
      boost::shared_ptr<IRule> identifyRule(database::entities::CDevice & device);
         
      //--------------------------------
      ///\brief Create the XplMessage matching a command, to send to a specific device
      ///\param [in]  targetDevice The device target of the message
      ///\param [in]  deviceCommand The command data
      ///\return the XplMessage to send
      ///\throws  shared::exception::CException  if device do not support commands
      //--------------------------------
      boost::shared_ptr< shared::xpl::CXplMessage > createXplCommand(database::entities::CDevice & targetDevice, communication::command::CDeviceCommand & deviceCommand);

   private:
      //--------------------------------
      ///\brief Instance manager
      //--------------------------------
      CRuleInstanceManager m_instanceManager;
      
      //--------------------------------
      ///\brief All specific factories
      //--------------------------------      
      std::vector< boost::shared_ptr<IDeviceManager> > m_allSpecificRuleFactories;
      
      //--------------------------------
      ///\brief The Xpl standard factory
      //--------------------------------      
      boost::shared_ptr<IDeviceManager> m_standardFactories;
   };
   
} //namespace rules
} //namespace communication

