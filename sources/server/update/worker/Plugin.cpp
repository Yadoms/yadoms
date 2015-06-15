#include "stdafx.h"
#include "Plugin.h"
#include <shared/Log.h>

#include <shared/ServiceLocator.h>
#include "IApplicationStopHandler.h"
#include "update/info/UpdateSite.h"

#include "WorkerTools.h"
#include <Poco/File.h>

#include "pluginSystem/Manager.h"
#include "i18n/ClientStrings.h"

namespace update {
   namespace worker {

      CPlugin::CPlugin(CWorkerTools::WorkerProgressFunc progressCallback)
         :m_progressCallback(progressCallback)
      {

      }


      CPlugin::~CPlugin()
      {

      }


      void CPlugin::install(const std::string & downloadUrl)
      {
         YADOMS_LOG(information) << "Installing new plugin from " << downloadUrl;

         shared::CDataContainer callbackData;
         callbackData.set("downloadUrl", downloadUrl);
         
         m_progressCallback(true, 0.0f, i18n::CClientStrings::UpdatePluginInstall, shared::CStringExtension::EmptyString, shared::CDataContainer::EmptyContainer);

         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {
            YADOMS_LOG(information) << "Downloading package";
            m_progressCallback(true, 0.0f, i18n::CClientStrings::UpdatePluginDownload, shared::CStringExtension::EmptyString, callbackData);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, m_progressCallback, i18n::CClientStrings::UpdatePluginDownload, 0.0, 50.0);
            YADOMS_LOG(information) << "Downloading package with sucess";

            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy package " << downloadedPackage.toString();
               m_progressCallback(true, 50.0f, i18n::CClientStrings::UpdatePluginDeploy, shared::CStringExtension::EmptyString, callbackData);
               Poco::Path pluginPath = CWorkerTools::deployPluginPackage(downloadedPackage);
               YADOMS_LOG(information) << "Plugin deployed with success";


               YADOMS_LOG(information) << "Refresh plugin list";

               m_progressCallback(true, 90.0f, i18n::CClientStrings::UpdatePluginFinalize, shared::CStringExtension::EmptyString, callbackData);
               boost::shared_ptr<pluginSystem::CManager> pluginManager = shared::CServiceLocator::instance().get<pluginSystem::CManager>();
               if (pluginManager)
                  pluginManager->updatePluginList();

               YADOMS_LOG(information) << "Plugin installed with success";

               m_progressCallback(true, 100.0f, i18n::CClientStrings::UpdatePluginSuccess, shared::CStringExtension::EmptyString, callbackData);
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy package : " << ex.what();
               m_progressCallback(false, 100.0f, i18n::CClientStrings::UpdatePluginDeployFailed, ex.what(), callbackData);
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download package : " << ex.what();
            m_progressCallback(false, 100.0f, i18n::CClientStrings::UpdatePluginDownloadFailed, ex.what(), callbackData);
         }
      }

      void CPlugin::update(const std::string & pluginName, const std::string & downloadUrl)
      {
         YADOMS_LOG(information) << "Updating plugin " << pluginName << " from " << downloadUrl;

         shared::CDataContainer callbackData;
         callbackData.set("pluginName", pluginName);
         callbackData.set("downloadUrl", downloadUrl);

         m_progressCallback(true, 0.0f, i18n::CClientStrings::UpdatePluginUpdate, shared::CStringExtension::EmptyString, callbackData);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {
            YADOMS_LOG(information) << "Downloading package";
            m_progressCallback(true, 0.0f, i18n::CClientStrings::UpdatePluginDownload, shared::CStringExtension::EmptyString, callbackData);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, m_progressCallback, i18n::CClientStrings::UpdatePluginDownload, 0.0, 50.0);
            YADOMS_LOG(information) << "Downloading package with sucess";

            /////////////////////////////////////////////
            //2. stop any instance
            /////////////////////////////////////////////
            boost::shared_ptr<pluginSystem::CManager> pluginManager = shared::CServiceLocator::instance().get<pluginSystem::CManager>();
            if (pluginManager)
               pluginManager->stopAllInstancesOfPlugin(pluginName);

            /////////////////////////////////////////////
            //3. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy package " << downloadedPackage.toString();
               m_progressCallback(true, 50.0f, i18n::CClientStrings::UpdatePluginDeploy, shared::CStringExtension::EmptyString, callbackData);
               Poco::Path pluginPath = CWorkerTools::deployPluginPackage(downloadedPackage);
               YADOMS_LOG(information) << "Plugin deployed with success";


               YADOMS_LOG(information) << "Start instances";
               m_progressCallback(true, 90.0f, i18n::CClientStrings::UpdatePluginFinalize, shared::CStringExtension::EmptyString, callbackData);
               if (pluginManager)
                  pluginManager->startAllInstancesOfPlugin(pluginName);

               YADOMS_LOG(information) << "Plugin installed with success";
               m_progressCallback(true, 100.0f, i18n::CClientStrings::UpdatePluginSuccess, shared::CStringExtension::EmptyString, callbackData);
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy package : " << ex.what();
               m_progressCallback(false, 100.0f, i18n::CClientStrings::UpdatePluginDeployFailed, ex.what(),callbackData);
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download package : " << ex.what();
            m_progressCallback(false, 100.0f, i18n::CClientStrings::UpdatePluginDownloadFailed, ex.what(), callbackData);
         }
      }

      void CPlugin::remove(const std::string & pluginName)
      {
         YADOMS_LOG(information) << "Removing plugin " << pluginName ;

         shared::CDataContainer callbackData;
         callbackData.set("pluginName", pluginName);

         m_progressCallback(true, 0.0f, i18n::CClientStrings::UpdatePluginRemove, shared::CStringExtension::EmptyString, callbackData);

         try
            {
            /////////////////////////////////////////////
            //1. stop any instance
            /////////////////////////////////////////////
            boost::shared_ptr<pluginSystem::CManager> pluginManager = shared::CServiceLocator::instance().get<pluginSystem::CManager>();
            if (pluginManager)
               pluginManager->stopAllInstancesOfPlugin(pluginName);

            /////////////////////////////////////////////
            //2. remove plugin folder
            /////////////////////////////////////////////
            boost::shared_ptr<startupOptions::IStartupOptions> startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();
            Poco::Path pluginPath(startupOptions->getPluginsPath());
            pluginPath.append(pluginName);

            Poco::File toDelete(pluginPath);
            if (toDelete.exists())
               toDelete.remove(true);

            /////////////////////////////////////////////
            //3. update plugin manager
            /////////////////////////////////////////////
            if (pluginManager)
               pluginManager->updatePluginList();

            m_progressCallback(true, 100.0f, i18n::CClientStrings::UpdatePluginSuccess, shared::CStringExtension::EmptyString, callbackData);
         }
         catch (std::exception & ex)
         {
            //fail to remove package
            YADOMS_LOG(error) << "Fail to delete plugin : " << pluginName << " : " << ex.what();
            m_progressCallback(false, 100.0f, i18n::CClientStrings::UpdatePluginRemoveFailed, ex.what(), callbackData);
         }
      }









   } // namespace worker
} // namespace update
