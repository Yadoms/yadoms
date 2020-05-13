#include "stdafx.h"
#include "Plugin.h"
#include <shared/Log.h>
#include <shared/Executable.h>
#include "WorkerTools.h"
#include <Poco/File.h>
#include "pluginSystem/Manager.h"
#include "i18n/ClientStrings.h"
#include "pluginSystem/Information.h"

namespace update
{
   namespace worker
   {
      void CPlugin::install(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& downloadUrl,
                            boost::shared_ptr<pluginSystem::CManager> pluginManager,
                            const boost::filesystem::path& pluginsPath)
      {
         YADOMS_LOG(information) << "Installing new plugin from " << downloadUrl;

         shared::CDataContainerSharedPtr callbackData = new_CDataContainerSharedPtr();
         callbackData->set("downloadUrl", downloadUrl);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdatePluginInstall, std::string(), new_CDataContainerSharedPtr());

         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         Poco::Path downloadedPackage;
         try
         {
            YADOMS_LOG(information) << "Downloading package";
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdatePluginDownload, std::string(), callbackData);
            downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, progressCallback, i18n::CClientStrings::UpdatePluginDownload,
                                                              0.0, 50.0);
            YADOMS_LOG(information) << "Downloading package with success";

            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy package " << downloadedPackage.toString();
               progressCallback(true, 50.0f, i18n::CClientStrings::UpdatePluginDeploy, std::string(), callbackData);
               const auto pluginPath = CWorkerTools::deployPackage(downloadedPackage, pluginsPath.string());
               YADOMS_LOG(information) << "Plugin deployed with success";

               // Change executable file permission to authorize execution
               pluginSystem::CInformation pluginInformation(pluginPath.toString());
               boost::filesystem::permissions(pluginInformation.getPath() / shared::CExecutable::ToFileName(pluginInformation.getType()),
                                              boost::filesystem::perms::add_perms
                                              | boost::filesystem::perms::owner_exe | boost::filesystem::perms::group_exe);

               YADOMS_LOG(information) << "Refresh plugin list";
               progressCallback(true, 90.0f, i18n::CClientStrings::UpdatePluginFinalize, std::string(), callbackData);
               if (pluginManager)
                  pluginManager->updatePluginList();

               YADOMS_LOG(information) << "Plugin installed with success";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdatePluginSuccess, std::string(), callbackData);
            }
            catch (std::exception& ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdatePluginDeployFailed, ex.what(), callbackData);
            }
         }
         catch (std::exception& ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download package : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdatePluginDownloadFailed, ex.what(), callbackData);
         }

         //delete downloaded zip file
         if (!downloadedPackage.toString().empty())
         {
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();
         }
      }

      void CPlugin::update(CWorkerTools::WorkerProgressFunc progressCallback,
                           const std::string& pluginName,
                           const std::string& downloadUrl,
                           boost::shared_ptr<pluginSystem::CManager> pluginManager,
                           const boost::filesystem::path& pluginsPath)
      {
         YADOMS_LOG(information) << "Updating plugin " << pluginName << " from " << downloadUrl;

         shared::CDataContainerSharedPtr callbackData = new_CDataContainerSharedPtr();
         callbackData->set("pluginName", pluginName);
         callbackData->set("downloadUrl", downloadUrl);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdatePluginUpdate, std::string(), callbackData);

         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         Poco::Path downloadedPackage;
         try
         {
            YADOMS_LOG(information) << "Downloading package";
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdatePluginDownload, std::string(), callbackData);
            downloadedPackage = CWorkerTools::downloadPackage(downloadUrl, progressCallback, i18n::CClientStrings::UpdatePluginDownload,
                                                              0.0, 50.0);
            YADOMS_LOG(information) << "Downloading package with success";

            /////////////////////////////////////////////
            //2. stop any instance
            /////////////////////////////////////////////
            if (pluginManager)
               pluginManager->stopAllInstancesOfPluginAndWaitForStopped(pluginName);

            /////////////////////////////////////////////
            //3. deploy package
            /////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Deploy package " << downloadedPackage.toString();
               progressCallback(true, 50.0f, i18n::CClientStrings::UpdatePluginDeploy, std::string(), callbackData);
               Poco::Path pluginPath = CWorkerTools::deployPackage(downloadedPackage, pluginsPath.string());
               YADOMS_LOG(information) << "Plugin deployed with success";


               YADOMS_LOG(information) << "Refresh plugin list";
               progressCallback(true, 90.0f, i18n::CClientStrings::UpdatePluginFinalize, std::string(), callbackData);
               if (pluginManager)
                  pluginManager->updatePluginList();


               YADOMS_LOG(information) << "Start instances";
               progressCallback(true, 95.0f, i18n::CClientStrings::UpdatePluginFinalize, std::string(), callbackData);
               if (pluginManager)
                  pluginManager->startAllInstancesOfPlugin(pluginName);

               YADOMS_LOG(information) << "Plugin installed with success";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdatePluginSuccess, std::string(), callbackData);
            }
            catch (std::exception& ex)
            {
               //fail to extract package file
               YADOMS_LOG(error) << "Fail to deploy package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdatePluginDeployFailed, ex.what(), callbackData);
            }
         }
         catch (std::exception& ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download package : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdatePluginDownloadFailed, ex.what(), callbackData);
         }

         //delete downloaded zip file
         if (!downloadedPackage.toString().empty())
         {
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();
         }
      }

      void CPlugin::remove(CWorkerTools::WorkerProgressFunc progressCallback,
                           const std::string& pluginName,
                           boost::shared_ptr<pluginSystem::CManager> pluginManager,
                           const boost::filesystem::path& pluginsPath)
      {
         YADOMS_LOG(information) << "Removing plugin " << pluginName;

         shared::CDataContainerSharedPtr callbackData = new_CDataContainerSharedPtr();
         callbackData->set("pluginName", pluginName);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdatePluginRemove, std::string(), callbackData);

         try
         {
            /////////////////////////////////////////////
            //1. stop any instance
            /////////////////////////////////////////////
            if (pluginManager)
               pluginManager->stopAllInstancesOfPluginAndWaitForStopped(pluginName);

            /////////////////////////////////////////////
            //2. remove plugin folder
            /////////////////////////////////////////////
            Poco::Path pluginPath(pluginsPath.string());
            pluginPath.append(pluginName);

            Poco::File toDelete(pluginPath);
            if (toDelete.exists())
               toDelete.remove(true);

            /////////////////////////////////////////////
            //3. update plugin manager
            /////////////////////////////////////////////
            if (pluginManager)
               pluginManager->updatePluginList();

            progressCallback(true, 100.0f, i18n::CClientStrings::UpdatePluginSuccess, std::string(), callbackData);
         }
         catch (std::exception& ex)
         {
            //fail to remove package
            YADOMS_LOG(error) << "Fail to delete plugin : " << pluginName << " : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdatePluginRemoveFailed, ex.what(), callbackData);
         }
      }
   } // namespace worker
} // namespace update
