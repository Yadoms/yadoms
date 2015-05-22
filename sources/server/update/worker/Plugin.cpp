#include "stdafx.h"
#include "Plugin.h"
#include <shared/Log.h>

#include <shared/ServiceLocator.h>
#include "IApplicationStopHandler.h"
#include "update/info/UpdateSite.h"

#include "tools/FileSystem.h"
#include <shared/web/UriHelpers.h>
#include <shared/web/FileDownloader.h>
#include <shared/compression/Extract.h>

#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>
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
         m_progressCallback(true, 0.0f, "Installing new plugin from " + downloadUrl);

         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {

            m_progressCallback(true, 0.0f, "Downloading package");
            Poco::Path downloadedPackage = downloadPackage(downloadUrl);
            m_progressCallback(true, 50.0f, "Package downloaded with success");

            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               m_progressCallback(true, 50.0f, "Deploy package " + downloadedPackage.toString());
               Poco::Path pluginPath = deployPackage(downloadedPackage);
               m_progressCallback(true, 90.0f, "Plugin deployed with success");

               m_progressCallback(true, 90.0f, "Refresh plugin list");
               boost::shared_ptr<pluginSystem::CManager> pluginManager = shared::CServiceLocator::instance().get<pluginSystem::CManager>();
               if (pluginManager)
                  pluginManager->updatePluginList();
               m_progressCallback(true, 100.0f, "Plugin installed with success");
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               m_progressCallback(false, 100.0f, std::string("Fail to deploy package : ") + ex.what());
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to download package : ") + ex.what());
         }
  
      }

      void CPlugin::update(const std::string & pluginName, const std::string & downloadUrl)
      {
         m_progressCallback(true, 0.0f, "Updating plugin " + pluginName + " from " + downloadUrl);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {

            m_progressCallback(true, 0.0f, "Downloading package");
            Poco::Path downloadedPackage = downloadPackage(downloadUrl);
            m_progressCallback(true, 50.0f, "Package downloaded with success");

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
               m_progressCallback(true, 50.0f, "Deploy package " + downloadedPackage.toString());
               Poco::Path pluginPath = deployPackage(downloadedPackage);
               m_progressCallback(true, 90.0f, "Plugin deployed with success");

               m_progressCallback(true, 90.0f, "Start instances");
               if (pluginManager)
                  pluginManager->startAllInstancesOfPlugin(pluginName);
               m_progressCallback(true, 100.0f, "Plugin updated with success");
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               m_progressCallback(false, 100.0f, std::string("Fail to deploy package : ") + ex.what());
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to download package : ") + ex.what());
         }
      }

      void CPlugin::remove(const std::string & pluginName)
      {
         m_progressCallback(true, 0.0f, "Removing plugin " + pluginName);

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
            m_progressCallback(false, 100.0f, std::string("Fail to delete plugin : ") + pluginName + " : " + ex.what());
         }
      }







      Poco::Path CPlugin::downloadPackage(const std::string & downloadUrl)
      {
         //determine the filename to download
         Poco::URI toDownload(downloadUrl);
         std::string packageName = shared::web::CUriHelpers::getFile(toDownload);
         if (packageName.empty())
            packageName = "temp.zip";

         //determine local path
         Poco::Path downloadedPackage(tools::CFileSystem::getTemporaryFolder());
         downloadedPackage.setFileName(packageName);

         shared::web::CFileDownloader::downloadFile(toDownload, downloadedPackage, boost::bind(&shared::web::CFileDownloader::reportProgressToLog, _1, _2));
         return downloadedPackage;
      }

      Poco::Path CPlugin::deployPackage(Poco::Path downloadedPackage)
      {
         /*
         When deploying a plugin we dont know the plugin name

         So deploy the plugin package to a random plugin directory (random is generated using uuid)
         When deployed, find the package.json and get the plugin name
         Rename the plugin directory to the good plugin name
         */

         //retreive startup options
         boost::shared_ptr<startupOptions::IStartupOptions> startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();
         const std::string pluginsPath = startupOptions->getPluginsPath();

         //determine a random folder name (the folder name is the plugin name; but it it not known here)
         Poco::UUIDGenerator& generator = Poco::UUIDGenerator::defaultGenerator();
         Poco::UUID uuid(generator.createRandom());

         Poco::Path tempPluginFolder(startupOptions->getPluginsPath());
         tempPluginFolder.append(uuid.toString());

         //extract to random pluginName location
         shared::compression::CExtract unZipper;
         unZipper.to(downloadedPackage, tempPluginFolder);

         try
         {
            //read package.json file and get the pluginname
            Poco::Path packageJsonPath(tempPluginFolder.toString());
            packageJsonPath.append("package.json");

            shared::CDataContainer packageJson;
            std::string packageJsonPathString = packageJsonPath.toString();
            packageJson.deserializeFromFile(packageJsonPathString);

            //retreive the plugin name
            std::string pluginName = packageJson.get<std::string>("name");


            try
            {
               Poco::Path realPluginFolder(startupOptions->getPluginsPath());
               realPluginFolder.append(pluginName);

               //if plugin directory already exists; delete it
               Poco::File targetPluginDir(realPluginFolder.toString());
               if (targetPluginDir.exists())
                  targetPluginDir.remove(true);

               //rename random plugin folder to good plugin name
               Poco::File realPluginFolderInfo(packageJsonPath.toString());
               realPluginFolderInfo.renameTo(realPluginFolder.toString());

               return realPluginFolder;
            }
            catch (std::exception & ex)
            {
               //delete folder tempPluginFolder
               Poco::File toDelete(tempPluginFolder.toString());
               if (toDelete.exists())
                  toDelete.remove(true);

               throw ex;
            }
         }
         catch (std::exception & ex)
         {
            //delete folder tempPluginFolder
            Poco::File toDelete(tempPluginFolder.toString());
            if (toDelete.exists())
               toDelete.remove(true);

            throw ex;
         }
      }



   } // namespace worker
} // namespace update
