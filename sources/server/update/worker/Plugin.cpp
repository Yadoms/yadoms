#include "stdafx.h"
#include "Plugin.h"
#include <shared/Log.h>

#include <shared/ServiceLocator.h>
#include "IApplicationStopHandler.h"
#include "update/info/UpdateSite.h"

#include "WorkerTools.h"
#include <Poco/File.h>

#include "pluginSystem/Manager.h"

namespace update {
   namespace worker {

      CPlugin::CPlugin(WorkerProgressFunc progressCallback)
         :m_progressCallback(progressCallback)
      {

      }


      CPlugin::~CPlugin()
      {

      }


      void CPlugin::install(const std::string & downloadUrl)
      {
         m_progressCallback(true, 0.0f, "Installing new plugin from " + downloadUrl, shared::CDataContainer::EmptyContainer);

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
               Poco::Path pluginPath = CWorkerTools::deployPluginPackage(downloadedPackage);
               m_progressCallback(true, 90.0f, "Plugin deployed with success", shared::CDataContainer::EmptyContainer);

               m_progressCallback(true, 90.0f, "Refresh plugin list", shared::CDataContainer::EmptyContainer);
               boost::shared_ptr<pluginSystem::CManager> pluginManager = shared::CServiceLocator::instance().get<pluginSystem::CManager>();
               if (pluginManager)
                  pluginManager->updatePluginList();
               m_progressCallback(true, 100.0f, "Plugin installed with success", shared::CDataContainer::EmptyContainer);
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

      void CPlugin::update(const std::string & pluginName, const std::string & downloadUrl)
      {
         m_progressCallback(true, 0.0f, "Updating plugin " + pluginName + " from " + downloadUrl, shared::CDataContainer::EmptyContainer);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {

            m_progressCallback(true, 0.0f, "Downloading package", shared::CDataContainer::EmptyContainer);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl);
            m_progressCallback(true, 50.0f, "Package downloaded with success", shared::CDataContainer::EmptyContainer);

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
               m_progressCallback(true, 50.0f, "Deploy package " + downloadedPackage.toString(), shared::CDataContainer::EmptyContainer);
               Poco::Path pluginPath = CWorkerTools::deployPluginPackage(downloadedPackage);
               m_progressCallback(true, 90.0f, "Plugin deployed with success", shared::CDataContainer::EmptyContainer);

               m_progressCallback(true, 90.0f, "Start instances", shared::CDataContainer::EmptyContainer);
               if (pluginManager)
                  pluginManager->startAllInstancesOfPlugin(pluginName);
               m_progressCallback(true, 100.0f, "Plugin updated with success", shared::CDataContainer::EmptyContainer);
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

      void CPlugin::remove(const std::string & pluginName)
      {
         m_progressCallback(true, 0.0f, "Removing plugin " + pluginName, shared::CDataContainer::EmptyContainer);

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
         }
         catch (std::exception & ex)
         {
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to delete plugin : ") + pluginName + " : " + ex.what(), shared::CDataContainer::EmptyContainer);
         }
      }









   } // namespace worker
} // namespace update
