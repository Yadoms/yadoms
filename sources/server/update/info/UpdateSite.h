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
         CUpdateSite() = delete;
         virtual ~CUpdateSite() = default;

      public:
         //---------------------------------------------
         ///\brief   Get all the available versions of Yadoms
         ///\return the list of Yadoms versions
         //---------------------------------------------
         static shared::CDataContainerSharedPtr getAllYadomsVersions();

         //---------------------------------------------
         ///\brief   Get all the available versions for a plugin
         ///\return the list of plugin version
         //---------------------------------------------
         static shared::CDataContainerSharedPtr getAllPluginVersions();

         //---------------------------------------------
         ///\brief   Get all the available versions for widgets
         ///\return the list of widgets version
         //---------------------------------------------
         static shared::CDataContainerSharedPtr getAllWidgetVersions();

         //---------------------------------------------
         ///\brief   Get all the available versions for a scriptInterpreter
         ///\return the list of scriptInterpreter version
         //---------------------------------------------
         static shared::CDataContainerSharedPtr getAllScriptInterpreterVersions();

      private:
         //---------------------------------------------
         ///\brief                           Call the distant script
         ///\param [in] script               The script to call (i.e.: yadoms.php)
         ///\param [in] includeOsAndArch     If true : parameters LANG, OS and ARCH are passed to script, otherwise only LANG is passed
         ///\param [in] resultFieldToReturn  The field to return
         ///\return                          The field in script result json
         //---------------------------------------------
         static shared::CDataContainerSharedPtr callDistantScript(const std::string& script,
                                                         bool includeOsAndArch,
                                                         const std::string& resultFieldToReturn);

         //---------------------------------------------
         ///\brief   The script to list available yadoms versions
         //---------------------------------------------
         static const std::string DistantYadomsListScript;
         //---------------------------------------------
         ///\brief   The script to list available plugins versions
         //---------------------------------------------
         static const std::string DistantPluginsListScript;
         //---------------------------------------------
         ///\brief   The script to list available script interpreters versions
         //---------------------------------------------
         static const std::string DistantScriptInterpretersListScript;
         //---------------------------------------------
         ///\brief   The script to list available widgets versions
         //---------------------------------------------
         static const std::string DistantWidgetsListScript;

         //---------------------------------------------
         ///\brief   The result field for yadoms script
         //---------------------------------------------
         static const std::string DistantYadomsScriptResultField;
         //---------------------------------------------
         ///\brief   The result field for plugins script
         //---------------------------------------------
         static const std::string DistantPluginsScriptResultField;
         //---------------------------------------------
         ///\brief   The result field for script interpreters script
         //---------------------------------------------
         static const std::string DistantScriptInterpretersScriptResultField;
         //---------------------------------------------
         ///\brief   The result field for widgets script
         //---------------------------------------------
         static const std::string DistantWidgetsScriptResultField;

         //---------------------------------------------
         ///\brief   The parameter OS for distant scripts
         //---------------------------------------------
         static const std::string DistantScriptParamOs;
         //---------------------------------------------
         ///\brief   The parameter ARCH for distant scripts
         //---------------------------------------------
         static const std::string DistantScriptParamArch;
         //---------------------------------------------
         ///\brief   The parameter DEVMODE for distant scripts
         //---------------------------------------------
         static const std::string DistantScriptParamDevMode;

         //---------------------------------------------
         ///\brief   The result field for distant scripts
         //---------------------------------------------
         static const std::string DistantScriptResult;
      };
   }
} // namespace update::info
