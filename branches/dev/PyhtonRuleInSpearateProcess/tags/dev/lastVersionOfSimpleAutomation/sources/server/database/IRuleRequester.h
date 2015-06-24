#pragma once

#include "entities/Entities.h"


namespace database { 


   class IRuleRequester
   {
   public:

      //--------------------------------------------------------------
      /// \brief           List all rules
      /// \return          List of registered rules
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRule> > getRules() const = 0;

      //--------------------------------------------------------------
      /// \brief           Get rule informations
      /// \param[in] ruleId Rule ID
      /// \return          Rule information
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CRule> getRule(int ruleId) const = 0;

      //--------------------------------------------------------------
      /// \brief           Add a new rule
      /// \param[in] data  Rule data
      /// \return          Rule ID
      //--------------------------------------------------------------
      virtual int addRule(const entities::CRule& data) = 0;

      //--------------------------------------------------------------
      /// \brief           Update rule data
      /// \param[in] data  New rule data
      //--------------------------------------------------------------
      virtual void updateRule(const entities::CRule& data) = 0;

      //--------------------------------------------------------------
      /// \brief           Delete rule
      /// \param[in] ruleId Rule ID
      //--------------------------------------------------------------
      virtual void deleteRule(int ruleId) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IRuleRequester()
      {
      }
   };

 
} //namespace database 
   