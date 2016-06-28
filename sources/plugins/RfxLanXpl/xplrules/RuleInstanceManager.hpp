#pragma once
#include "IRule.h"

namespace xplrules
{
   class CRuleInstanceManager
   {
   public:
      CRuleInstanceManager()
      {
      }

      virtual ~CRuleInstanceManager()
      {
      }

      template <class T>
      boost::shared_ptr<IRule> getRule()
      {
         std::vector<boost::shared_ptr<IRule> >::iterator i;
         for (i = m_instanciatedRules.begin(); i != m_instanciatedRules.end(); ++i)
         {
            boost::shared_ptr<IRule> pFoundRule = boost::dynamic_pointer_cast<T>(*i);
            if (pFoundRule.get() != nullptr)
               return pFoundRule;
         }

         auto createdRule = boost::shared_ptr<IRule>(boost::make_shared<T>());
         m_instanciatedRules.push_back(createdRule);
         return createdRule;
      }

   private:
      //------------------------------
      ///\brief All instantiated rules
      //------------------------------
      std::vector<boost::shared_ptr<IRule> > m_instanciatedRules;
   };
} //namespace xplrules


