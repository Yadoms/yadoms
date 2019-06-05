#pragma once
#include "IInstance.h"

namespace automation
{
   namespace interpreter
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
         virtual ~IManager() = default;

         //-----------------------------------------------------
         ///\brief               Get all loaded interpreters (available or not)
         ///\return              The list of loaded interpreters
         //-----------------------------------------------------
         virtual std::vector<std::string> getLoadedInterpreters() = 0;

         //-----------------------------------------------------
         ///\brief               Get loaded interpreters with associated information
         ///\return              The list of loaded interpreters (available or not) with their information
         //-----------------------------------------------------
         virtual std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>> getLoadedInterpretersInformation() = 0;

         //-----------------------------------------------------
         ///\brief               Get available interpreters with associated information
         ///\return              The list of available interpreters with their information
         //-----------------------------------------------------
         virtual std::map<std::string, boost::shared_ptr<const shared::script::yInterpreterApi::IInformation>> getAvailableInterpretersInformation() = 0;

         //-----------------------------------------------------
         ///\brief               Get the available interpreter instance
         ///\param[in] interpreterType The interpreter type
         ///\return              The corresponding interpreter instance
         ///\throw std::runtime_error No corresponding interpreter was found
         //-----------------------------------------------------
         virtual boost::shared_ptr<IInstance> getAvailableInterpreterInstance(const std::string& interpreterType) = 0;

         //-----------------------------------------------------
         ///\brief               Get the loaded interpreter c
         ///\param[in] interpreterType The interpreter type
         ///\return              The corresponding interpreter instance
         ///\throw std::runtime_error No corresponding interpreter was found
         //-----------------------------------------------------
         virtual boost::shared_ptr<IInstance> getLoadedInterpreterInstance(const std::string& interpreterType) = 0;

         //-----------------------------------------------------
         ///\brief               Unload an interpreter (do nothing if interpreter not loaded)
         ///\param[in] interpreterName Interpreter to unload
         //-----------------------------------------------------
         virtual void unloadInterpreter(const std::string& interpreterName) = 0;

         //-----------------------------------------------------
         ///\brief               Get the script file
         ///\param[in] interpreterName Associated interpreter
         ///\param[in] scriptPath      Script path
         ///\return              The script code
         ///\throw CInvalidParameter if rule ID or file not found
         //-----------------------------------------------------
         virtual std::string getScriptContent(const std::string& interpreterName,
                                              const std::string& scriptPath) = 0;

         //-----------------------------------------------------
         ///\brief               Get the script template file
         ///\param[in] interpreterName Interpreter to unload
         ///\return              The template code
         //-----------------------------------------------------
         virtual std::string getScriptTemplateContent(const std::string& interpreterName) = 0;

         //-----------------------------------------------------
         ///\brief               Get the full path of the script log file 
         /// \param[in] ruleId   Rule id
         ///\return              Full path of the script log file
         //-----------------------------------------------------
         virtual boost::filesystem::path getScriptLogFilename(int ruleId) const = 0;

         //-----------------------------------------------------
         ///\brief               Update the script file (create if necessary)
         ///\param[in] interpreterName Associated interpreter
         ///\param[in] scriptPath      Script path
         ///\param[in] code            New script code
         //-----------------------------------------------------
         virtual void updateScriptFile(const std::string& interpreterName,
                                       const std::string& scriptPath,
                                       const std::string& code) = 0;

         //-----------------------------------------------------
         ///\brief               Delete the script file
         ///\param[in] interpreterName Associated interpreter
         ///\param[in] scriptPath      Script path
         ///\param[in] doBackup  Do a backup if true
         //-----------------------------------------------------
         virtual void deleteScriptFile(const std::string& interpreterName,
                                       const std::string& scriptPath,
                                       bool doBackup = true) = 0;

         //-----------------------------------------------------
         ///\brief               Get the script log file
         ///\param[in] ruleId    The rule ID
         ///\return              The rule log file, empty if not available
         ///\throw CInvalidParameter if rule ID not found
         //-----------------------------------------------------
         virtual std::string getScriptLogContent(int ruleId) = 0;

         //-----------------------------------------------------
         ///\brief               Delete the script log file
         ///\param[in] ruleId    The rule ID
         ///\throw CInvalidParameter if rule ID not found
         //-----------------------------------------------------
         virtual void deleteLog(int ruleId) = 0;

         //-----------------------------------------------------
         ///\brief               Set the callback on rule stop notification
         ///\param[in] onScriptStoppedFct    The function to call when rule is notified as stopped
         //-----------------------------------------------------
         virtual void setOnScriptStoppedFct(boost::function2<void, int, const std::string&> onScriptStoppedFct) = 0;
      };
   } // namespace automation::interpreter
}
