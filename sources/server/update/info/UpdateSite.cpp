#include "stdafx.h"
#include "UpdateSite.h"
#include <shared/web/UriHelpers.h>
#include <shared/web/FileDownloader.h>
#include <Poco/Environment.h>
#include "startupOptions/IStartupOptions.h"
#include <shared/http/HttpMethods.h>
#include <shared/ServiceLocator.h>

namespace update
{
   namespace info
   {
      std::string CUpdateSite::m_distantYadomsListScript("yadoms3.php");
      std::string CUpdateSite::m_distantPluginsListScript("plugins3.php");
      std::string CUpdateSite::m_distantScriptInterpretersListScript("scriptInterpreters3.php");
      std::string CUpdateSite::m_distantWidgetsListScript("widgets3.php");

      std::string CUpdateSite::m_distantYadomsScriptResultField("data.yadoms");
      std::string CUpdateSite::m_distantPluginsScriptResultField("data.plugins");
      std::string CUpdateSite::m_distantScriptInterpretersScriptResultField("data.scriptInterpreters");
      std::string CUpdateSite::m_distantWidgetsScriptResultField("data.widgets");

      std::string CUpdateSite::m_distantScriptParamOs("os");
      std::string CUpdateSite::m_distantScriptParamArch("arch");
      std::string CUpdateSite::m_distantScriptParamDevMode("devMode");

      std::string CUpdateSite::m_distantScriptResult("result");


      shared::CDataContainer CUpdateSite::getAllYadomsVersions()
      {
         return callDistantScript(m_distantYadomsListScript, true, m_distantYadomsScriptResultField);
      }

      shared::CDataContainer CUpdateSite::getAllPluginVersions()
      {
         return callDistantScript(m_distantPluginsListScript, true, m_distantPluginsScriptResultField);
      }


      shared::CDataContainer CUpdateSite::getAllScriptInterpreterVersions()
      {
         return callDistantScript(m_distantScriptInterpretersListScript, true,
                                  m_distantScriptInterpretersScriptResultField);
      }


      shared::CDataContainer CUpdateSite::getAllWidgetVersions()
      {
         return callDistantScript(m_distantWidgetsListScript, false, m_distantWidgetsScriptResultField);
      }


      shared::CDataContainer CUpdateSite::callDistantScript(const std::string& script,
                                                            bool includeOsAndArch,
                                                            const std::string& resultFieldToReturn)
      {
         try
         {
            const auto startupOptions(
               shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>());

            //get the script address
            Poco::URI uri(startupOptions->getUpdateSiteUri());
            shared::web::CUriHelpers::appendPath(uri, script);

            //include script parameters
            if (includeOsAndArch)
            {
               uri.addQueryParameter(m_distantScriptParamOs, Poco::Environment::osName());
               uri.addQueryParameter(m_distantScriptParamArch, Poco::Environment::osArchitecture());
            }
            if (shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>()->getDeveloperMode()
            )
               uri.addQueryParameter(m_distantScriptParamDevMode);

            //call script
            shared::CDataContainer headerParameters;
            headerParameters.set("Host", uri.getHost() + ":" + std::to_string(uri.getPort()));
            headerParameters.set("User-Agent", "yadoms");
            headerParameters.set("Accept", "application/json");
            headerParameters.set("Connection", "close");
            auto lastVersionInformation(shared::CHttpMethods::sendGetRequest(uri,
                                                                             headerParameters));

            if (!lastVersionInformation.containsValue(m_distantScriptResult))
               throw std::runtime_error("Error in calling script. Fail to get data from " + uri.toString());

            if (!lastVersionInformation.get<bool>(m_distantScriptResult))
               throw std::runtime_error(
                  "Error in calling script : " + lastVersionInformation.get<std::string>("message"));

            return lastVersionInformation.get<shared::CDataContainer>(resultFieldToReturn);
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(error) << "CUpdateSite::callDistantScript : fail to call " << script << " distant script" <<
               e.
               what();
            throw std::runtime_error("Fail to call distant script");
         }
         catch (...)
         {
            YADOMS_LOG(error) << "CUpdateSite::callDistantScript : fail to call " << script <<
               " distant script, unknown error";
            throw std::runtime_error("Fail to call distant script");
         }
      }
   }
} // namespace update::info
