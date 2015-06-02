#include "stdafx.h"
#include "ScriptInterpreter.h"
#include <shared/Log.h>

#include <shared/ServiceLocator.h>
#include "IApplicationStopHandler.h"
#include "update/info/UpdateSite.h"

#include "WorkerTools.h"
#include <Poco/File.h>

#include "automation/IRuleManager.h"

namespace update {
   namespace worker {

      CScriptInterpreter::CScriptInterpreter(WorkerProgressFunc progressCallback)
         :m_progressCallback(progressCallback)
      {

      }


      CScriptInterpreter::~CScriptInterpreter()
      {

      }


      void CScriptInterpreter::install(const std::string & downloadUrl)
      {
         m_progressCallback(true, 0.0f, "Installing new scriptInterpreter from " + downloadUrl, shared::CDataContainer::EmptyContainer);

         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {

            m_progressCallback(true, 0.0f, "Downloading package", shared::CDataContainer::EmptyContainer);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl);
            m_progressCallback(true, 50.0f, "Package downloaded with success", shared::CDataContainer::EmptyContainer);

            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               m_progressCallback(true, 50.0f, "Deploy package " + downloadedPackage.toString(), shared::CDataContainer::EmptyContainer);
               Poco::Path scriptInterpreterPath = CWorkerTools::deployScriptInterpreterPackage(downloadedPackage);
               m_progressCallback(true, 90.0f, "ScriptInterpreter deployed with success", shared::CDataContainer::EmptyContainer);

               m_progressCallback(true, 90.0f, "Refresh scriptInterpreter list", shared::CDataContainer::EmptyContainer);

               //force refresh of script interpreters
               boost::shared_ptr<automation::IRuleManager> automationRuleManager = shared::CServiceLocator::instance().get<automation::IRuleManager>();
               if (automationRuleManager)
                  automationRuleManager->getAvailableInterpreters(); //as seen in comments, refresh interpreters list

               m_progressCallback(true, 100.0f, "ScriptInterpreter installed with success", shared::CDataContainer::EmptyContainer);
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               m_progressCallback(false, 100.0f, std::string("Fail to deploy package : ") + ex.what(), shared::CDataContainer::EmptyContainer);
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to download package : ") + ex.what(), shared::CDataContainer::EmptyContainer);
         }

      }

      void CScriptInterpreter::update(const std::string & scriptInterpreterName, const std::string & downloadUrl)
      {
         m_progressCallback(true, 0.0f, "Updating scriptInterpreter " + scriptInterpreterName + " from " + downloadUrl, shared::CDataContainer::EmptyContainer);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {

            m_progressCallback(true, 0.0f, "Downloading package", shared::CDataContainer::EmptyContainer);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl);
            m_progressCallback(true, 50.0f, "Package downloaded with success", shared::CDataContainer::EmptyContainer);

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
               m_progressCallback(true, 50.0f, "Deploy package " + downloadedPackage.toString(), shared::CDataContainer::EmptyContainer);
               Poco::Path scriptInterpreterPath = CWorkerTools::deployScriptInterpreterPackage(downloadedPackage);
               m_progressCallback(true, 90.0f, "ScriptInterpreter deployed with success", shared::CDataContainer::EmptyContainer);

               m_progressCallback(true, 90.0f, "Start rules", shared::CDataContainer::EmptyContainer);

               //start all rules using this scriptInterpreter
               if (automationRuleManager)
                  automationRuleManager->startAllRulesMatchingInterpreter(scriptInterpreterName);

               m_progressCallback(true, 100.0f, "ScriptInterpreter updated with success", shared::CDataContainer::EmptyContainer);
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               m_progressCallback(false, 100.0f, std::string("Fail to deploy package : ") + ex.what(), shared::CDataContainer::EmptyContainer);
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to download package : ") + ex.what(), shared::CDataContainer::EmptyContainer);
         }
      }

      void CScriptInterpreter::remove(const std::string & scriptInterpreterName)
      {
         m_progressCallback(true, 0.0f, "Removing scriptInterpreter " + scriptInterpreterName, shared::CDataContainer::EmptyContainer);

         try
         {
            /////////////////////////////////////////////
            //1. stop any instance
            /////////////////////////////////////////////

            //stop all rules using this scriptInterpreter
            boost::shared_ptr<automation::IRuleManager> automationRuleManager = shared::CServiceLocator::instance().get<automation::IRuleManager>();
            if (automationRuleManager)
            {
               automationRuleManager->stopAllRulesMatchingInterpreter(scriptInterpreterName);
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
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to delete scriptInterpreter : ") + scriptInterpreterName + " : " + ex.what(), shared::CDataContainer::EmptyContainer);
         }
      }









   } // namespace worker
} // namespace update
