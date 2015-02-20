#pragma once
#include <shared/script/IRunner.h>
#include "IProperties.h"
#include "../../database/sqlite/requesters/Rule.h"

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The script factory interface
   //-----------------------------------------------------
   class IFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IFactory() {}

      //-----------------------------------------------------
      ///\brief               Get available interpreters (as user-friendly names)
      ///\return              The list of available interpreters
      //-----------------------------------------------------
      virtual std::vector<std::string> getAvailableInterpreters() = 0;//TODO ajouter le REST permettant d'appeler cette fonction


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
      ///\brief               Create a script runner
      ///\param[in] scriptProperties      The Script properties
      ///\return              A new script runner instance
      ///\throw CInvalidParameter if unable to create script runner
      //-----------------------------------------------------
      virtual boost::shared_ptr<shared::script::IRunner> createScriptRunner(boost::shared_ptr<const IProperties> scriptProperties) = 0;

      //-----------------------------------------------------
      ///\brief               Create the script context (IYScriptApi implementation)
      ///\return              A script context instance
      //-----------------------------------------------------
      virtual boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> createScriptContext() = 0;
   };

} } // namespace automation::script

