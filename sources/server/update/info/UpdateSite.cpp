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
      const std::string CUpdateSite::DistantYadomsListScript("yadoms3.php");
      const std::string CUpdateSite::DistantPluginsListScript("plugins3.php");
      const std::string CUpdateSite::DistantScriptInterpretersListScript("scriptInterpreters3.php");
      const std::string CUpdateSite::DistantWidgetsListScript("widgets3.php");

      const std::string CUpdateSite::DistantYadomsScriptResultField("data.yadoms");
      const std::string CUpdateSite::DistantPluginsScriptResultField("data.plugins");
      const std::string CUpdateSite::DistantScriptInterpretersScriptResultField("data.scriptInterpreters");
      const std::string CUpdateSite::DistantWidgetsScriptResultField("data.widgets");

      const std::string CUpdateSite::DistantScriptParamOs("os");
      const std::string CUpdateSite::DistantScriptParamArch("arch");
      const std::string CUpdateSite::DistantScriptParamDevMode("devMode");

      const std::string CUpdateSite::DistantScriptResult("result");


      shared::CDataContainerSharedPtr CUpdateSite::getAllYadomsVersions()
      {
         return callDistantScript(DistantYadomsListScript,
                                  true,
                                  DistantYadomsScriptResultField);
      }

      shared::CDataContainerSharedPtr CUpdateSite::getAllPluginVersions()
      {
         return callDistantScript(DistantPluginsListScript,
                                  true,
                                  DistantPluginsScriptResultField);
      }


      shared::CDataContainerSharedPtr CUpdateSite::getAllScriptInterpreterVersions()
      {
         return callDistantScript(DistantScriptInterpretersListScript,
                                  true,
                                  DistantScriptInterpretersScriptResultField);
      }


      shared::CDataContainerSharedPtr CUpdateSite::getAllWidgetVersions()
      {
         return callDistantScript(DistantWidgetsListScript,
                                  false,
                                  DistantWidgetsScriptResultField);
      }

      shared::CDataContainerSharedPtr CUpdateSite::callDistantScript(const std::string& script,
                                                            bool includeOsAndArch,
                                                            const std::string& resultFieldToReturn)
      {
         const auto startupOptions(shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>());
         const auto url(startupOptions->getUpdateSiteUri() + script);

         try
         {
            shared::CDataContainer parameters;
            if (includeOsAndArch)
            {
               parameters.set(DistantScriptParamOs, Poco::Environment::osName());
               parameters.set(DistantScriptParamArch, Poco::Environment::osArchitecture());
            }
            if (startupOptions->getDeveloperMode())
            {
               parameters.set(DistantScriptParamDevMode, "1");
            }

            shared::CDataContainer headerParameters;
            headerParameters.set("User-Agent", "yadoms");
            headerParameters.set("Accept", "application/json");
            headerParameters.set("Connection", "close");
            const auto lastVersionInformation(shared::CHttpMethods::sendGetRequest(url,
                                                                                   headerParameters,
                                                                                   parameters));

            if (!lastVersionInformation->containsValue(DistantScriptResult))
               throw std::runtime_error("Fail to get data from " + url);

            if (!lastVersionInformation->get<bool>(DistantScriptResult))
               throw std::runtime_error(
                  "Error in calling " + url + " : " + lastVersionInformation->get<std::string>("message"));

            return lastVersionInformation->getChild(resultFieldToReturn);
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(error) << "CUpdateSite::callDistantScript : fail to call " << url << " : " <<
               e.what();
            throw std::runtime_error("Fail to call distant script");
         }
         catch (...)
         {
            YADOMS_LOG(error) << "CUpdateSite::callDistantScript : fail to call " << url << " , unknown error";
            throw std::runtime_error("Fail to call distant script");
         }
      }
   }
} // namespace update::info
