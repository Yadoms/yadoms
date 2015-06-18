#include "stdafx.h"
#include "ScriptInterpreter.h"
#include <shared/Log.h>

#include <shared/ServiceLocator.h>
#include "IApplicationStopHandler.h"
#include "update/info/UpdateSite.h"

#include "WorkerTools.h"
#include <Poco/File.h>

#include "automation/IRuleManager.h"
#include "i18n/ClientStrings.h"

namespace update {
   namespace worker {


      void CScriptInterpreter::install(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & downloadUrl)
      {
         YADOMS_LOG(information) << "Installing new scriptInterpreter from " << downloadUrl;
         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateScriptInterpreterInstall, shared::CStringExtension::EmptyString, shared::CDataContainer::EmptyContainer);

         shared::CDataContainer callbackData;
         callbackData.set("downloadUrl", downloadUrl);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {
            YADOMS_LOG(information) << "Downloading scriptInterpreter package";
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateScriptInterpreterDownload, shared::CStringExtension::EmptyString, callbackData);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, progressCallback, i18n::CClientStrings::UpdateScriptInterpreterDownload, 0.0, 50.0);
            YADOMS_LOG(information) << "Downloading scriptInterpreter package with sucess";

            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy scriptInterpreter package " << downloadedPackage.toString();
               progressCallback(true, 50.0f, i18n::CClientStrings::UpdateScriptInterpreterDeploy, shared::CStringExtension::EmptyString, callbackData);
               Poco::Path pluginPath = CWorkerTools::deployScriptInterpreterPackage(downloadedPackage);
               YADOMS_LOG(information) << "ScriptInterpreter deployed with success";


               YADOMS_LOG(information) << "Refresh scriptInterpreter list";
               progressCallback(true, 90.0f, i18n::CClientStrings::UpdateScriptInterpreterFinalize, shared::CStringExtension::EmptyString, callbackData);

               //force refresh of script interpreters
               boost::shared_ptr<automation::IRuleManager> automationRuleManager = shared::CServiceLocator::instance().get<automation::IRuleManager>();
               if (automationRuleManager)
                  automationRuleManager->getAvailableInterpreters(); //as seen in comments, refresh interpreters list

               YADOMS_LOG(information) << "ScriptInterpreter installed with success";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterSuccess, shared::CStringExtension::EmptyString, shared::CDataContainer::EmptyContainer);

            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy scriptInterpreter package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterDeployFailed, ex.what(), callbackData);

            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download scriptInterpreter package : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterDownloadFailed, ex.what(), callbackData);
         }

      }

      void CScriptInterpreter::update(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & scriptInterpreterName, const std::string & downloadUrl)
      {
         YADOMS_LOG(information) << "Updating scriptInterpreter " << scriptInterpreterName << " from " << downloadUrl;

         shared::CDataContainer callbackData;
         callbackData.set("scriptInterpreterName", scriptInterpreterName);
         callbackData.set("downloadUrl", downloadUrl);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateScriptInterpreterUpdate, shared::CStringExtension::EmptyString, callbackData);

         
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {
            YADOMS_LOG(information) << "Downloading scriptInterpreter package";
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateScriptInterpreterDownload, shared::CStringExtension::EmptyString, callbackData);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, progressCallback, i18n::CClientStrings::UpdateScriptInterpreterDownload, 0.0, 50.0);
            YADOMS_LOG(information) << "Downloading scriptInterpreter package with sucess";


            /////////////////////////////////////////////
            //2. stop any rule
            /////////////////////////////////////////////

            //stop all rules using this scriptInterpreter
            boost::shared_ptr<automation::IRuleManager> automationRuleManager = shared::CServiceLocator::instance().get<automation::IRuleManager>();
            if (automationRuleManager)
               automationRuleManager->stopAllRulesMatchingInterpreter(scriptInterpreterName);

            /////////////////////////////////////////////
            //3. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy scriptInterpreter package " << downloadedPackage.toString();
               progressCallback(true, 50.0f, i18n::CClientStrings::UpdateScriptInterpreterDeploy, shared::CStringExtension::EmptyString, callbackData);
               Poco::Path scriptInterpreterPath = CWorkerTools::deployScriptInterpreterPackage(downloadedPackage);
               YADOMS_LOG(information) << "ScriptInterpreter deployed with success";


               YADOMS_LOG(information) << "Start instances";
               progressCallback(true, 90.0f, i18n::CClientStrings::UpdateScriptInterpreterFinalize, shared::CStringExtension::EmptyString, callbackData);

               //start all rules using this scriptInterpreter
               if (automationRuleManager)
                  automationRuleManager->startAllRulesMatchingInterpreter(scriptInterpreterName);

               progressCallback(true, 100.0f, "ScriptInterpreter updated with success", shared::CStringExtension::EmptyString, shared::CDataContainer::EmptyContainer);
               YADOMS_LOG(information) << "ScriptInterpreter installed with success";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterSuccess, shared::CStringExtension::EmptyString, callbackData);

            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy scriptInterpreter package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterDeployFailed, ex.what(), callbackData);
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download scriptInterpreter package : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterDownloadFailed, ex.what(), callbackData);
         }
      }

      void CScriptInterpreter::remove(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & scriptInterpreterName)
      {
         YADOMS_LOG(information) << "Removing scriptInterpreter " << scriptInterpreterName;

         shared::CDataContainer callbackData;
         callbackData.set("scriptInterpreterName", scriptInterpreterName);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateScriptInterpreterRemove, shared::CStringExtension::EmptyString, callbackData);

         try
         {
            /////////////////////////////////////////////
            //1. stop any instance
            /////////////////////////////////////////////

            //stop all rules using this scriptInterpreter
            boost::shared_ptr<automation::IRuleManager> automationRuleManager = shared::CServiceLocator::instance().get<automation::IRuleManager>();
            if (automationRuleManager)
            {
               automationRuleManager->deleteAllRulesMatchingInterpreter(scriptInterpreterName);
            }

            /////////////////////////////////////////////
            //2. remove scriptInterpreter folder
            /////////////////////////////////////////////
            boost::shared_ptr<startupOptions::IStartupOptions> startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();
            Poco::Path scriptInterpreterPath(startupOptions->getScriptInterpretersPath());
            scriptInterpreterPath.append(scriptInterpreterName);

            Poco::File toDelete(scriptInterpreterPath);
            if (toDelete.exists())
               toDelete.remove(true);

            /////////////////////////////////////////////
            //3. update scriptInterpreter manager
            /////////////////////////////////////////////
            //TODO : update scriptInterpreter list
            if (automationRuleManager)
               automationRuleManager->getAvailableInterpreters(); //as seen in comments, refresh interpreters list
         }
         catch (std::exception & ex)
         {
            //fail to remove package
            YADOMS_LOG(error) << "Fail to delete scriptInterpreter : " << scriptInterpreterName << " : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateScriptInterpreterRemoveFailed, ex.what(), callbackData);
         }
      }









   } // namespace worker
} // namespace update
