#include "stdafx.h"
#include "ScriptInterpreter.h"
#include <shared/Log.h>

#include <shared/ServiceLocator.h>

#include <Poco/File.h>

#include "automation/IRuleManager.h"
#include "i18n/ClientStrings.h"

namespace update
{
   namespace worker
   {
      void CScriptInterpreter::install(CWorkerHelpers::WorkerProgressFunc progressCallback,
                                       const std::string& downloadUrl,
                                       const boost::filesystem::path& scriptInterpretersPath)
      {
         YADOMS_LOG(information) << "Installing new scriptInterpreter from " << downloadUrl;
         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateScriptInterpreterInstall, std::string(), shared::CDataContainer::make());

         auto callbackData = shared::CDataContainer::make();
         callbackData->set("downloadUrl", downloadUrl);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         boost::filesystem::path downloadedPackage;
         try
         {
            YADOMS_LOG(information) << "Downloading scriptInterpreter package";
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateScriptInterpreterDownload, std::string(), callbackData);
            downloadedPackage = CWorkerHelpers::downloadPackage(downloadUrl, progressCallback,
                                                                         i18n::CClientStrings::UpdateScriptInterpreterDownload, 0.0, 50.0);
            YADOMS_LOG(information) << "Downloading scriptInterpreter package with success";

            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy scriptInterpreter package " << downloadedPackage.string();
               progressCallback(true, 50.0f, i18n::CClientStrings::UpdateScriptInterpreterDeploy, std::string(), callbackData);
               const auto pluginPath = CWorkerHelpers::deployPackage(downloadedPackage, scriptInterpretersPath.string());
               YADOMS_LOG(information) << "ScriptInterpreter deployed with success";

               // Refresh scriptInterpreter list
               YADOMS_LOG(information) << "Refresh scriptInterpreter list";
               progressCallback(true, 90.0f, i18n::CClientStrings::UpdateScriptInterpreterFinalize, std::string(), callbackData);
               auto automationRuleManager = shared::CServiceLocator::instance().get<automation::IRuleManager>();
               if (automationRuleManager)
                  automationRuleManager->getLoadedInterpreters(); //as seen in comments, refresh interpreters list

               YADOMS_LOG(information) << "ScriptInterpreter installed with success";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterSuccess, std::string(),
                  shared::CDataContainer::make());
            }
            catch (std::exception& ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy scriptInterpreter package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterDeployFailed, ex.what(), callbackData);
            }
         }
         catch (std::exception& ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download scriptInterpreter package : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterDownloadFailed, ex.what(), callbackData);
         }

         //delete downloaded zip file
         if (!downloadedPackage.string().empty())
         {
            Poco::File toDelete(downloadedPackage.string());
            if (toDelete.exists())
               toDelete.remove();
         }
      }

      void CScriptInterpreter::update(CWorkerHelpers::WorkerProgressFunc progressCallback,
                                      const std::string& scriptInterpreterName,
                                      const std::string& downloadUrl,
                                      const boost::filesystem::path& scriptInterpretersPath)
      {
         YADOMS_LOG(information) << "Updating scriptInterpreter " << scriptInterpreterName << " from " << downloadUrl;

         boost::shared_ptr<shared::CDataContainer> callbackData = shared::CDataContainer::make();
         callbackData->set("scriptInterpreterName", scriptInterpreterName);
         callbackData->set("downloadUrl", downloadUrl);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateScriptInterpreterUpdate, std::string(), callbackData);

         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         boost::filesystem::path downloadedPackage;
         try
         {
            YADOMS_LOG(information) << "Downloading scriptInterpreter package";
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateScriptInterpreterDownload, std::string(), callbackData);
            downloadedPackage = CWorkerHelpers::downloadPackage(downloadUrl, progressCallback,
                                                                         i18n::CClientStrings::UpdateScriptInterpreterDownload, 0.0, 50.0);
            YADOMS_LOG(information) << "Downloading scriptInterpreter package with success";

            /////////////////////////////////////////////
            //2. stop any rule
            /////////////////////////////////////////////

            //TOFIX must stop the interpreter itself and wait for stopped
            //stop all rules using this scriptInterpreter
            auto automationRuleManager = shared::CServiceLocator::instance().get<automation::IRuleManager>();
            if (automationRuleManager)
               automationRuleManager->stopAllRulesMatchingInterpreter(scriptInterpreterName);

            /////////////////////////////////////////////
            //3. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy scriptInterpreter package " << downloadedPackage.string();
               progressCallback(true, 50.0f, i18n::CClientStrings::UpdateScriptInterpreterDeploy, std::string(), callbackData);
               const auto scriptInterpreterPath = CWorkerHelpers::deployPackage(downloadedPackage, scriptInterpretersPath.string());
               YADOMS_LOG(information) << "ScriptInterpreter deployed with success";


               // Refresh scriptInterpreter list
               YADOMS_LOG(information) << "Refresh scriptInterpreter list";
               progressCallback(true, 90.0f, i18n::CClientStrings::UpdateScriptInterpreterFinalize, std::string(), callbackData);
               if (automationRuleManager)
                  automationRuleManager->getLoadedInterpreters(); //as seen in comments, refresh interpreters list


               YADOMS_LOG(information) << "Start instances";
               progressCallback(true, 95.0f, i18n::CClientStrings::UpdateScriptInterpreterFinalize, std::string(), callbackData);

               //start all rules using this scriptInterpreter
               if (automationRuleManager)
                  automationRuleManager->startAllRulesMatchingInterpreter(scriptInterpreterName);

               progressCallback(true, 100.0f, "ScriptInterpreter updated with success", std::string(), shared::CDataContainer::make());
               YADOMS_LOG(information) << "ScriptInterpreter installed with success";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterSuccess, std::string(), callbackData);
            }
            catch (std::exception& ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy scriptInterpreter package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterDeployFailed, ex.what(), callbackData);
            }
         }
         catch (std::exception& ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download scriptInterpreter package : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterDownloadFailed, ex.what(), callbackData);
         }

         //delete downloaded zip file
         if (!downloadedPackage.string().empty())
         {
            Poco::File toDelete(downloadedPackage.string());
            if (toDelete.exists())
               toDelete.remove();
         }
      }

      void CScriptInterpreter::remove(CWorkerHelpers::WorkerProgressFunc progressCallback,
                                      const std::string& scriptInterpreterName,
                                      const boost::filesystem::path& scriptInterpretersPath)
      {
         YADOMS_LOG(information) << "Removing scriptInterpreter " << scriptInterpreterName;

         auto callbackData = shared::CDataContainer::make();
         callbackData->set("scriptInterpreterName", scriptInterpreterName);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateScriptInterpreterRemove, std::string(), callbackData);

         try
         {
            /////////////////////////////////////////////
            //1. stop any instance
            /////////////////////////////////////////////

            //TOFIX must stop the interpreter itself and wait for stopped
            //stop all rules using this scriptInterpreter
            auto automationRuleManager = shared::CServiceLocator::instance().get<automation::IRuleManager>();
            if (automationRuleManager)
            {
               automationRuleManager->deleteAllRulesMatchingInterpreter(scriptInterpreterName);
            }

            /////////////////////////////////////////////
            //2. remove scriptInterpreter folder
            /////////////////////////////////////////////
            auto scriptInterpreterPath(scriptInterpretersPath);
            scriptInterpreterPath.append(scriptInterpreterName);

            Poco::File toDelete(scriptInterpreterPath.string());
            if (toDelete.exists())
               toDelete.remove(true);

            /////////////////////////////////////////////
            //3. update scriptInterpreter manager
            /////////////////////////////////////////////
            if (automationRuleManager)
               automationRuleManager->getLoadedInterpreters(); //as seen in comments, refresh interpreters list
         }
         catch (std::exception& ex)
         {
            //fail to remove package
            YADOMS_LOG(error) << "Fail to delete scriptInterpreter : " << scriptInterpreterName << " : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterRemoveFailed, ex.what(), callbackData);
         }
      }
   } // namespace worker
} // namespace update
