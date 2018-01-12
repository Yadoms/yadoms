#pragma once

#include <shared/DataContainer.h>

namespace update
{
   namespace info
   {
      //---------------------------------------------
      ///\brief   Provide information about update site
      //---------------------------------------------
      class CUpdateSite
      {
      private:
         //---------------------------------------------
         ///\brief  Private ctor to disallow it
         //---------------------------------------------
         CUpdateSite();

      public:
         //---------------------------------------------
         ///\brief   Get all the available versions of Yadoms
         ///\param [in] displayLanguage The language used for display
         ///\return the list of Yadoms versions
         //---------------------------------------------
         static shared::CDataContainer getAllYadomsVersions();

         //---------------------------------------------
         ///\brief   Get all the available versions for a plugin
         ///\param [in] displayLanguage The language used for display
         ///\return the list of plugin version
         //---------------------------------------------
         static shared::CDataContainer getAllPluginVersions();

         //---------------------------------------------
         ///\brief   Get all the available versions for widgets
         ///\param [in] displayLanguage The language used for display
         ///\return the list of widgets version
         //---------------------------------------------
         static shared::CDataContainer getAllWidgetVersions();

         //---------------------------------------------
         ///\brief   Get all the available versions for a scriptInterpreter
         ///\param [in] displayLanguage The language used for display
         ///\return the list of scriptInterpreter version
         //---------------------------------------------
         static shared::CDataContainer getAllScriptInterpreterVersions();

      private:
         //---------------------------------------------
         ///\brief                           Call the distant script
         ///\param [in] script               The script to call (i.e.: yadoms.php)
         ///\param [in] includeOsAndArch     If true : parameters LANG, OS and ARCH are passed to script, otherwise only LANG is passed
         ///\param [in] displayLanguage      The language used for display
         ///\param [in] resultFieldToReturn  The field to return
         ///\return                          The field in script result json
         //---------------------------------------------
         static shared::CDataContainer callDistantScript(const std::string& script,
                                                         bool includeOsAndArch,
                                                         const std::string& resultFieldToReturn);

         //---------------------------------------------
         ///\brief   The script to list available yadoms versions
         //---------------------------------------------
         static std::string m_distantYadomsListScript;
         //---------------------------------------------
         ///\brief   The script to list available plugins versions
         //---------------------------------------------
         static std::string m_distantPluginsListScript;
         //---------------------------------------------
         ///\brief   The script to list available script interpreters versions
         //---------------------------------------------
         static std::string m_distantScriptInterpretersListScript;
         //---------------------------------------------
         ///\brief   The script to list available widgets versions
         //---------------------------------------------
         static std::string m_distantWidgetsListScript;

         //---------------------------------------------
         ///\brief   The result field for yadoms script
         //---------------------------------------------
         static std::string m_distantYadomsScriptResultField;
         //---------------------------------------------
         ///\brief   The result field for plugins script
         //---------------------------------------------
         static std::string m_distantPluginsScriptResultField;
         //---------------------------------------------
         ///\brief   The result field for script interpreters script
         //---------------------------------------------
         static std::string m_distantScriptInterpretersScriptResultField;
         //---------------------------------------------
         ///\brief   The result field for widgets script
         //---------------------------------------------
         static std::string m_distantWidgetsScriptResultField;

         //---------------------------------------------
         ///\brief   The parameter OS for distant scripts
         //---------------------------------------------
         static std::string m_distantScriptParamOs;
         //---------------------------------------------
         ///\brief   The parameter ARCH for distant scripts
         //---------------------------------------------
         static std::string m_distantScriptParamArch;
         //---------------------------------------------
         ///\brief   The parameter DEMMODE for distant scripts
         //---------------------------------------------
         static std::string m_distantScriptParamDevMode;
         //---------------------------------------------
         ///\brief   The parameter LANG for distant scripts
         //---------------------------------------------
         static std::string m_distantScriptParamLang;

         //---------------------------------------------
         ///\brief   The result field for distant scripts
         //---------------------------------------------
         static std::string m_distantScriptResult;
      };
   }
} // namespace update::info


