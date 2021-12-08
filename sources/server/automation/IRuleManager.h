#pragma once
#include "../database/entities/Entities.h"
#include "shared/plugin/yPluginApi/historization/PluginState.h"

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rules manager interface
   //-----------------------------------------------------
   class IRuleManager
   {
   public:
      virtual ~IRuleManager() = default;

      //-----------------------------------------------------
      ///\brief               Start the Rule manager (start all rules)
      //-----------------------------------------------------
      virtual void start() = 0;

      //-----------------------------------------------------
      ///\brief               Stop the Rule manager (stop all rules)
      //-----------------------------------------------------
      virtual void stop() = 0;

      //-----------------------------------------------------
      ///\brief               Get all loaded interpreters (re-scan interpreters)
      ///\return              The interpreter list
      //-----------------------------------------------------
      virtual std::vector<std::string> getLoadedInterpreters() = 0;

      //-----------------------------------------------------
      ///\brief               Get all available interpreters (re-scan interpreters)
      ///\return              The interpreter list
      //-----------------------------------------------------
      virtual std::vector<std::string> getAvailableInterpreters() = 0;

      //-----------------------------------------------------
      ///\brief               Get all available interpreters (re-scan interpreters)
      ///\param[in] includeNotAvailable   Include not available interpreters (interpreter marking itself as not runnable)
      ///\return              The interpreter list (interpreter name as key, available status as value)
      //-----------------------------------------------------
      virtual std::map<std::string, bool> getInterpreters(bool includeNotAvailable) = 0;

      //-----------------------------------------------------
      ///\brief               Get all rules
      ///\return              The rule list
      //-----------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CRule>> getRules() const = 0;

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
      virtual std::vector<boost::shared_ptr<database::entities::CRule>> getRules(
         const boost::optional<int>& ruleId,
         const boost::optional<std::string>& fromName,
         const std::set<std::string>& fromInterpreter,
         const boost::optional<std::string>& fromEditor,
         bool fromAutostart,
         const std::set<database::entities::ERuleState>& fromState) = 0;

      //-----------------------------------------------------
      ///\brief               Create a new rule
      /// \param[in] ruleData Data of the rule to create (name, configuration, etc...)
      /// \param[in] code     Rule code
      /// \param[in] startNow Indicate if the rule must be started after creation
      ///\return              The new rule id
      //-----------------------------------------------------
      virtual int createRule(const database::entities::CRule& ruleData,
                             const std::string& code,
                             bool startNow = true) = 0;

      //--------------------------------------------------------------
      /// \brief           Get the rule informations
      /// \param[in] id    Rule Id
      /// \return          The rule informations, if available (empty string if not)
      /// \throw           CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<database::entities::CRule> getRule(int id) const = 0;

      //--------------------------------------------------------------
      /// \brief           Get the log of the rule
      /// \param[in] id    Rule Id
      /// \return          The code of the rule, if available (empty string if not)
      /// \throw           CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::string getRuleLog(int id) const = 0;

      //--------------------------------------------------------------
      /// \brief           Delete the log of the rule
      /// \param[in] id    Rule Id
      /// \throw           CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void deleteRuleLog(int id) const = 0;

      //--------------------------------------------------------------
      /// \brief           Get the code of the rule
      /// \param[in] id    Rule Id
      /// \return          The code of the rule, if available (empty string if not)
      /// \throw           CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::string getRuleCode(int id) const = 0;

      //--------------------------------------------------------------
      /// \brief           Get the template code for an interpreter
      /// \param[in] interpreterName      The script interpreter name
      /// \return          The template code for the interpreter, if available (empty string if not)
      //--------------------------------------------------------------
      virtual std::string getRuleTemplateCode(const std::string& interpreterName) const = 0;

      //--------------------------------------------------------------
      /// \brief           Update rule informations
      /// \param[in] ruleData The rule new configuration
      /// \throw           CNotSupported if request to apply unsupported modifications
      /// \throw           CDatabaseException if fails
      //--------------------------------------------------------------
      virtual void updateRule(const database::entities::CRule& ruleData) = 0;

      //--------------------------------------------------------------
      /// \brief           Update rule informations
      /// \param[in] id    Rule Id
      /// \param[in] code  Rule code
      //--------------------------------------------------------------
      virtual void updateRuleCode(int id, const std::string& code) = 0;

      //--------------------------------------------------------------
      /// \brief                          Delete a rule
      /// \param[in] id                   Rule Id to delete
      /// \throw                          CInvalidParameter if rule id is unknown
      //--------------------------------------------------------------
      virtual void deleteRule(int id) = 0;

      //--------------------------------------------------------------
      /// \brief                          Duplicate a rule
      /// \param[in] idToDuplicate        Rule Id to duplicate
      /// \param[in] newName              The new rule name
      /// \return                         The new rule id
      /// \throw                          CInvalidParameter if rule id is unknown
      //--------------------------------------------------------------
      virtual int duplicateRule(int idToDuplicate, const std::string& newName) = 0;

      //--------------------------------------------------------------
      /// \brief                          Start all rules using a script interpreter
      /// \param[in] interpreterName      The script interpreter name
      //--------------------------------------------------------------
      virtual void startAllRulesMatchingInterpreter(const std::string& interpreterName) = 0;

      //--------------------------------------------------------------
      /// \brief                          Stop all rules using a script interpreter
      /// \param[in] interpreterName      The script interpreter name
      //--------------------------------------------------------------
      virtual void stopAllRulesMatchingInterpreter(const std::string& interpreterName) = 0;

      //--------------------------------------------------------------
      /// \brief                          Delete all rules using a script interpreter
      /// \param[in] interpreterName      The script interpreter name
      //--------------------------------------------------------------
      virtual void deleteAllRulesMatchingInterpreter(const std::string& interpreterName) = 0;

      //-----------------------------------------------------
      ///\brief               Start a rule from configured rule data
      ///\param[in] ruleId    The rule ID
      ///\throw CRuleException Error starting rule
      //-----------------------------------------------------
      virtual void startRule(int ruleId) = 0;

      //-----------------------------------------------------
      ///\brief               Stop a rule
      ///\param[in] ruleId    The rule ID
      ///\return              true if rule is stopping, false if rule was not running
      //-----------------------------------------------------
      virtual bool stopRule(int ruleId) = 0;

      //-----------------------------------------------------
      ///\brief               Stop a rule
      ///\param[in] ruleId    The rule ID
      ///\param timeout       The timeout (default 20sec)
      ///\throw               CRuleException if timeout
      //-----------------------------------------------------
      virtual void stopRuleAndWaitForStopped(int ruleId,
                                             const boost::posix_time::time_duration& timeout = boost::posix_time::seconds(20)) = 0;
   };
} // namespace automation	
