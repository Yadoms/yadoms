#pragma once
#include "IRule.h"

namespace xplrules { 

   class CRuleInstanceManager
   {
   public:
      CRuleInstanceManager()
      {
      }
      
      virtual ~CRuleInstanceManager()
      {
      }
      
      template<class T>
      boost::shared_ptr< IRule > getRule()
      {
         BOOST_FOREACH(boost::shared_ptr< IRule > pRule, m_instanciatedRules)
         {
             boost::shared_ptr< IRule > pFoundRule = boost::dynamic_pointer_cast< T >(pRule);
             if(pFoundRule.get() != NULL)
               return pFoundRule;
         }
         
         boost::shared_ptr< IRule > createdRule = boost::shared_ptr< IRule >(new T());
         m_instanciatedRules.push_back(createdRule);
         return createdRule;
      }
   
   private:
      //------------------------------
      ///\brief All instantiated rules
      //------------------------------
      std::vector< boost::shared_ptr<IRule> > m_instanciatedRules;
   };
   

} //namespace xplrules
