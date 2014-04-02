#pragma once
#include "IFactory.h"
#include "RuleInstanceManager.hpp"

namespace communication { namespace rules {

   class CRulerFactory
   {
   public:
      CRulerFactory();
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
      ///\brief Identify the Xpl actor for the device
      ///\param [in]  device The device
      ///\return the xplActor
      //--------------------------------
      shared::xpl::CXplActor identifyXplActor(database::entities::CDevice & device);

   private:
      //--------------------------------
      ///\brief Instance manager
      //--------------------------------
      CRuleInstanceManager m_instanceManager;
      
      //--------------------------------
      ///\brief All specific factories
      //--------------------------------      
      std::vector< boost::shared_ptr<IFactory> > m_allSpecificRuleFactories;
      
      //--------------------------------
      ///\brief The Xpl standard factory
      //--------------------------------      
      boost::shared_ptr<IFactory> m_standardFactories;
   };
   
} //namespace rules
} //namespace communication

