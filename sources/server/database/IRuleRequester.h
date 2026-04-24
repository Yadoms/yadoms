#pragma once
#include "entities/Entities.h"

namespace database
{
   class IRuleRequester
   {
   public:
      virtual ~IRuleRequester() = default;

      //--------------------------------------------------------------
      /// \brief           List all rules
      /// \return          List of registered rules
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRule>> getRules() const = 0;

      //--------------------------------------------------------------
      /// \brief                                             Get rules
      /// \param [in] ruleId                                 Search rule matching this ID
      /// \param [in] fromName                               Search rules for this name
      /// \param [in] fromInterpreter                        Search rules for this interpreters
      /// \param [in] fromEditor                             Search rules for this editor
      /// \param [in] fromAutostart                          Search auto-starting rules
      /// \param [in] fromState                              Search rules matching one of these states
      /// \return                                            The rules list
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRule>> getRules(
         const boost::optional<int>& ruleId,
         const boost::optional<std::string>& fromName,
         const std::set<std::string>& fromInterpreter,
         const boost::optional<std::string>& fromEditor,
         bool fromAutostart,
         const std::set<entities::ERuleState>& fromState) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all rules handled by a spcific interpreter
      /// \param[in] interpreterName Interpreter name
      /// \return          List of rules handled by this interpreter
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CRule>> getRules(const std::string& interpreterName) const = 0;

      //--------------------------------------------------------------
      /// \brief           Get rule informations
      /// \param[in] ruleId Rule ID
      /// \return          Rule information
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CRule> getRule(int ruleId) const = 0;

      //--------------------------------------------------------------
      /// \brief           Add a new rule
      /// \param[in] ruleData  Rule data
      /// \return          Rule ID
      //--------------------------------------------------------------
      virtual int addRule(const entities::CRule& ruleData) = 0;

      //--------------------------------------------------------------
      /// \brief           Update rule data
      /// \param[in] ruleData  New rule data
      //--------------------------------------------------------------
      virtual void updateRule(const entities::CRule& ruleData) = 0;

      //--------------------------------------------------------------
      /// \brief           Delete rule
      /// \param[in] ruleId Rule ID
      //--------------------------------------------------------------
      virtual void deleteRule(int ruleId) = 0;
   };
} //namespace database 
