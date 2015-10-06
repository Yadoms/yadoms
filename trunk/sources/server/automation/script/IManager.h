#pragma once
#include <shared/script/IRunner.h>
#include "IProperties.h"
#include "../../database/sqlite/requesters/Rule.h"
#include <shared/script/ILogger.h>
#include <shared/script/IStopNotifier.h>
#include <server/automation/IRuleStateHandler.h>

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The script factory interface
   //-----------------------------------------------------
   class IManager
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IManager() {}

      //-----------------------------------------------------
      ///\brief               Get available interpreters (as user-friendly names)
      ///\return              The list of available interpreters
      //-----------------------------------------------------
      virtual std::vector<std::string> getAvailableInterpreters() = 0;

      //-----------------------------------------------------
      ///\brief               Unload an interpreter (do nothing if interpreter not loaded)
      ///\param[in] interpreterName Interpreter to unload
      //-----------------------------------------------------
      virtual void unloadInterpreter(const std::string& interpreterName) = 0;


      //-----------------------------------------------------
      ///\brief               Create the properties for a script
      ///\param[in] ruleData  The rule raw data
      ///\return              A new script properties instance
      //-----------------------------------------------------
      virtual boost::shared_ptr<IProperties> createScriptProperties(boost::shared_ptr<const database::entities::CRule> ruleData) = 0;

      //-----------------------------------------------------
      ///\brief               Get the script file
      ///\param[in] ruleData  The rule raw data
      ///\return              The rule code
      ///\throw CInvalidParameter if rule ID or file not found
      //-----------------------------------------------------
      virtual std::string getScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData) = 0;
      
      //-----------------------------------------------------
      ///\brief               Update the script file (create if necessary)
      ///\param[in] ruleData  The rule raw data
      ///\return              The rule code
      //-----------------------------------------------------
      virtual void updateScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code) = 0;

      //-----------------------------------------------------
      ///\brief               Delete the script file
      ///\param[in] ruleData  The rule raw data
      ///\param[in] doBackup  Do a backup if true
      //-----------------------------------------------------
      virtual void deleteScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData, bool doBackup = true) = 0;

      //-----------------------------------------------------
      ///\brief               Get the script log file
      ///\param[in] ruleData  The rule raw data
      ///\return              The rule log file, empty if not available
      ///\throw CInvalidParameter if rule ID not found
      //-----------------------------------------------------
      virtual std::string getScriptLogFile(boost::shared_ptr<const database::entities::CRule> ruleData) = 0;
      
      //-----------------------------------------------------
      ///\brief               Create a script runner
      ///\param[in] scriptProperties      The script properties
      ///\param[in] scriptLogger          The rule logger
      ///\param[in] yScriptApi            The rule context
      ///\param[in] stopNotifier          The rule stop notifier
      ///\return              A new script runner instance
      ///\throw CInvalidParameter if unable to create script runner
      //-----------------------------------------------------
      virtual boost::shared_ptr<shared::script::IRunner> createScriptRunner(
         boost::shared_ptr<const IProperties> scriptProperties,
         boost::shared_ptr<shared::script::ILogger> scriptLogger,
         boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
         boost::shared_ptr<shared::script::IStopNotifier> stopNotifier) = 0;

      //-----------------------------------------------------
      ///\brief               Create the script logger
      ///\param[in] scriptPath The script path where to log in
      ///\return              A script logger instance
      //-----------------------------------------------------
      virtual boost::shared_ptr<shared::script::ILogger> createScriptLogger(const std::string& scriptPath) = 0;

      //-----------------------------------------------------
      ///\brief               Create the script context (IYScriptApi implementation)
      ///\param[in] scriptLogger The logger used for rule
      ///\return              A script context instance
      //-----------------------------------------------------
      virtual boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> createScriptContext(boost::shared_ptr<shared::script::ILogger> scriptLogger) = 0;

      //-----------------------------------------------------
      ///\brief               Create the stop notifier
      ///\param[in] ruleStateHandler   The global state handler
      ///\param[in] ruleId    The rule ID
      ///\return              A stop notifier instance
      //-----------------------------------------------------
      virtual boost::shared_ptr<shared::script::IStopNotifier> createStopNotifier(boost::shared_ptr<IRuleStateHandler> ruleStateHandler, int ruleId) = 0;
   };

} } // namespace automation::script

