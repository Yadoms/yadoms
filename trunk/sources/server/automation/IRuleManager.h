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
      virtual std::vector<const boost::shared_ptr<const database::entities::CRule> > getRules() const = 0;

      //-----------------------------------------------------
      ///\brief               Create a new rule
      /// \param[in] data     Data fo the rule to create (name, configuration, etc...)
      ///\return              The new rule id
      //-----------------------------------------------------
      virtual int createRule(const database::entities::CRule& data) = 0;

      //--------------------------------------------------------------
      /// \brief           Get the rule informations
      /// \param[in] id    Rule Id
      /// \return          The rule informations, if available (empty string if not)
      /// \throw           CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<const database::entities::CRule> getRule(int id) const = 0;
      
      //--------------------------------------------------------------
      /// \brief           Update rule informations
      /// \param[in] id    Rule Id
      /// \param[in] newData The rule new configuration
      /// \throw           CNotSupported if request to apply unsupported modifications
      /// \throw           CDatabaseException if fails
      //--------------------------------------------------------------
      virtual void updateRule(const database::entities::CRule& newData) = 0;
    
      //--------------------------------------------------------------
      /// \brief                          Delete a rule
      /// \param[in] id                   Rule Id to delete
      /// \throw                          CInvalidParameter if rule id is unknown
      //--------------------------------------------------------------
      virtual void deleteRule(int id) = 0;
   };
	
} // namespace automation	
	
	