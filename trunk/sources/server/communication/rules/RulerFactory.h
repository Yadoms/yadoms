#pragma once
#include "IFactory.h"
#include "RuleInstanceManager.hpp"

namespace communication { namespace rules {

   class CRulerFactory
   {
   public:
      CRulerFactory();
      virtual ~CRulerFactory();
         
      boost::shared_ptr<IRule> identifyRule(shared::xpl::CXplMessage & msg);
         
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

