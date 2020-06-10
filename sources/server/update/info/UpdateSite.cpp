#include "stdafx.h"
#include "UpdateSite.h"
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


      boost::shared_ptr<shared::CDataContainer> CUpdateSite::getAllYadomsVersions()
      {
         return callDistantScript(DistantYadomsListScript,
                                  true,
                                  DistantYadomsScriptResultField);
      }

      boost::shared_ptr<shared::CDataContainer> CUpdateSite::getAllPluginVersions()
      {
         return callDistantScript(DistantPluginsListScript,
                                  true,
                                  DistantPluginsScriptResultField);
      }


      boost::shared_ptr<shared::CDataContainer> CUpdateSite::getAllScriptInterpreterVersions()
      {
         return callDistantScript(DistantScriptInterpretersListScript,
                                  true,
                                  DistantScriptInterpretersScriptResultField);
      }


      boost::shared_ptr<shared::CDataContainer> CUpdateSite::getAllWidgetVersions()
      {
         return callDistantScript(DistantWidgetsListScript,
                                  false,
                                  DistantWidgetsScriptResultField);
      }

      boost::shared_ptr<shared::CDataContainer> CUpdateSite::callDistantScript(const std::string& script,
                                                                               bool includeOsAndArch,
                                                                               const std::string& resultFieldToReturn)
      {
         const auto startupOptions(shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>());
         const auto url(startupOptions->getUpdateSiteUri() + script);

         try
         {
            std::map<std::string, std::string> parameters;
            if (includeOsAndArch)
            {
               parameters[DistantScriptParamOs] = Poco::Environment::osName();
               parameters[DistantScriptParamArch] = Poco::Environment::osArchitecture();
            }
            if (startupOptions->getDeveloperMode())
            {
               parameters[DistantScriptParamDevMode] = "1";
            }

            const std::map<std::string, std::string> headerParameters = {
               {"User-Agent", "yadoms"},
               {"Accept", "application/json"},
               {"Connection", "close"}
            };
            const auto lastVersionInformation(shared::CHttpMethods::sendJsonGetRequest(url,
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
