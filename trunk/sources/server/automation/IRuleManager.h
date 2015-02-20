#pragma once
#include "../database/entities/Entities.h"

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rules manager interface
   //-----------------------------------------------------
   class IRuleManager
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IRuleManager() {}

      //-----------------------------------------------------
      ///\brief               Start all rules
      //-----------------------------------------------------
      virtual void start() = 0;

      //-----------------------------------------------------
      ///\brief               Stop all rules
      //-----------------------------------------------------
      virtual void stop() = 0;      

      //-----------------------------------------------------
      ///\brief               Get all rules
      ///\return              The rule list
      //-----------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CRule> > getRules() const = 0;

      //-----------------------------------------------------
      ///\brief               Create a new rule
      /// \param[in] ruleData Data of the rule to create (name, configuration, etc...)
      /// \param[in] scriptCode The rule script code
      ///\return              The new rule id
      //-----------------------------------------------------
      virtual int createRule(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string & scriptCode) = 0;

      //--------------------------------------------------------------
      /// \brief           Get the rule informations
      /// \param[in] id    Rule Id
      /// \return          The rule informations, if available (empty string if not)
      /// \throw           CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<database::entities::CRule> getRule(int id) const = 0;
      
      //--------------------------------------------------------------
      /// \brief           Update rule informations
      /// \param[in] ruleData   The rule new configuration
      /// \param[in] scriptCode The rule script code
      /// \throw           CNotSupported if request to apply unsupported modifications
      /// \throw           CDatabaseException if fails
      //--------------------------------------------------------------
      virtual void updateRule(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string & scriptCode) = 0;
    
      //--------------------------------------------------------------
      /// \brief                          Delete a rule
      /// \param[in] id                   Rule Id to delete
      /// \throw                          CInvalidParameter if rule id is unknown
      //--------------------------------------------------------------
      virtual void deleteRule(int id) = 0;

      //--------------------------------------------------------------
      /// \brief                          Get the rule code
      /// \param[in] id                   Rule Id to get the code
      /// \throw                          CInvalidParameter if rule id is unknown
      //--------------------------------------------------------------
      virtual const std::string getRuleCode(int id) const = 0;
   };
	
} // namespace automation	
	
	