#include "stdafx.h"
#include "UpdateSite.h"
#include <shared/web/UriHelpers.h>
#include <shared/web/FileDownloader.h>
#include <Poco/Environment.h>
#include "startupOptions/IStartupOptions.h"
#include <shared/ServiceLocator.h>

namespace update
{
   namespace info
   {
      std::string CUpdateSite::m_distantYadomsListScript("yadoms.php");
      std::string CUpdateSite::m_distantPluginsListScript("plugins.php");
      std::string CUpdateSite::m_distantScriptInterpretersListScript("scriptInterpreters.php");
      std::string CUpdateSite::m_distantWidgetsListScript("widgets.php");

      std::string CUpdateSite::m_distantYadomsScriptResultField("data.yadoms");
      std::string CUpdateSite::m_distantPluginsScriptResultField("data.plugins");
      std::string CUpdateSite::m_distantScriptInterpretersScriptResultField("data.scriptInterpreters");
      std::string CUpdateSite::m_distantWidgetsScriptResultField("data.widgets");

      std::string CUpdateSite::m_distantScriptParamOs("os");
      std::string CUpdateSite::m_distantScriptParamArch("arch");
      std::string CUpdateSite::m_distantScriptParamLang("lang");
      std::string CUpdateSite::m_distantScriptParamDevMode("devMode");

      std::string CUpdateSite::m_distantScriptResult("result");

      CUpdateSite::CUpdateSite()
      {
      }

      shared::CDataContainer CUpdateSite::getAllYadomsVersions(const std::string& displayLanguage)
      {
         return callDistantScript(m_distantYadomsListScript, true, displayLanguage, m_distantYadomsScriptResultField);
      }

      shared::CDataContainer CUpdateSite::getAllPluginVersions(const std::string& displayLanguage)
      {
         return callDistantScript(m_distantPluginsListScript, true, displayLanguage, m_distantPluginsScriptResultField);
      }


      shared::CDataContainer CUpdateSite::getAllScriptInterpreterVersions(const std::string& displayLanguage)
      {
         return callDistantScript(m_distantScriptInterpretersListScript, true, displayLanguage, m_distantScriptInterpretersScriptResultField);
      }


      shared::CDataContainer CUpdateSite::getAllWidgetsVersions(const std::string& displayLanguage)
      {
         return callDistantScript(m_distantWidgetsListScript, false, displayLanguage, m_distantWidgetsScriptResultField);
      }


      shared::CDataContainer CUpdateSite::callDistantScript(const std::string& script,
                                                            bool includeOsAndArch,
                                                            const std::string& displayLanguage,
                                                            const std::string& resultFieldToReturn)
      {
         auto startupOptions(shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>());

         //get the script address
         Poco::URI base(startupOptions->getUpdateSiteUri());
         shared::web::CUriHelpers::appendPath(base, script);

         //include script parameters
         if (includeOsAndArch)
         {
            base.addQueryParameter(m_distantScriptParamOs, Poco::Environment::osName());
            base.addQueryParameter(m_distantScriptParamArch, Poco::Environment::osArchitecture());
         }
         base.addQueryParameter(m_distantScriptParamLang, displayLanguage);
         if (shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>()->getDeveloperMode())
            base.addQueryParameter(m_distantScriptParamDevMode);

         //call script
         auto lastVersionInformation = shared::web::CFileDownloader::downloadInMemoryJsonFile(base,
                                                                                              boost::bind(&shared::web::CFileDownloader::reportProgressToLog, _1, _2));

         if (!lastVersionInformation.containsValue(m_distantScriptResult))
            throw shared::exception::CException("Error in calling script. Fail to get data from " + base.toString());

         if (!lastVersionInformation.get<bool>(m_distantScriptResult))
            throw shared::exception::CException("Error in calling script : " + lastVersionInformation.get<std::string>("message"));

         return lastVersionInformation.get<shared::CDataContainer>(resultFieldToReturn);
      }
   }
} // namespace update::info


