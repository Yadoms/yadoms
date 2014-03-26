#include "stdafx.h"
#include "RulerFactory.h"
#include "standard/RuleFactory.h"
#include "rfxLanXpl/RuleFactory.h"

namespace communication { namespace rules {

   CRulerFactory::CRulerFactory()
      :m_standardFactories(boost::shared_ptr<IFactory>(new standard::CRuleFactory()))
   {
      //all specific rules factory
      m_allSpecificRuleFactories.push_back(boost::shared_ptr<IFactory>(new rfxLanXpl::CRuleFactory));
   }

   CRulerFactory::~CRulerFactory()
   {
   }
   
   boost::shared_ptr<IRule> CRulerFactory::identifyRule(shared::xpl::CXplMessage & msg)
   {      
      BOOST_FOREACH(boost::shared_ptr<IFactory> currentFactory, m_allSpecificRuleFactories)
      {
         if(currentFactory->isHandled(msg))
         {
            boost::shared_ptr<IRule> foundRule = currentFactory->identifyRule(msg, m_instanceManager);
            if(foundRule.get() != NULL)
               return foundRule;
         }
      }
      return m_standardFactories->identifyRule(msg, m_instanceManager);
   }
   
   
} //namespace rules
} //namespace communication

