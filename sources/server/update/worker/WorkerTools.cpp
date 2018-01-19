#include "stdafx.h"
#include "WorkerTools.h"


#include <Poco/File.h>
#include <Poco/URI.h>

#include <shared/Log.h>
#include <shared/DataContainer.h>
#include "tools/FileSystem.h"
#include <shared/web/UriHelpers.h>
#include <shared/web/FileDownloader.h>
#include <shared/compression/Extract.h>

#include "startupOptions/IStartupOptions.h"

#include <shared/ServiceLocator.h>
#include <shared/tools/Random.h>
#include <shared/exception/Extract.hpp>


namespace update
{
   namespace worker
   {
      Poco::Path CWorkerTools::downloadPackage(const std::string& downloadUrl, WorkerProgressFunc callback, const std::string& function, float min,
                                               float max)
      {
         return downloadPackage(downloadUrl, boost::bind(&CWorkerTools::reportDownloadProgress, _1, _2, callback, function, min, max));
      }

      Poco::Path CWorkerTools::downloadPackageAndVerify(const std::string& downloadUrl, const std::string& md5Hash, WorkerProgressFunc callback,
                                                        const std::string& function, float min, float max)
      {
         return downloadPackageAndVerify(downloadUrl, md5Hash,
                                         boost::bind(&CWorkerTools::reportDownloadProgress, _1, _2, callback, function, min, max));
      }

      Poco::Path CWorkerTools::downloadPackage(const std::string& downloadUrl)
      {
         return downloadPackage(downloadUrl, boost::bind(&shared::web::CFileDownloader::reportProgressToLog, _1, _2));
      }

      Poco::Path CWorkerTools::downloadPackage(const std::string& downloadUrl, shared::web::CFileDownloader::ProgressFunc progressReporter)
      {
         //determine the filename to download
         Poco::URI toDownload(downloadUrl);
         auto packageName = shared::web::CUriHelpers::getFile(toDownload);
         if (packageName.empty())
            packageName = "temp.zip";

         //determine local path
         auto downloadedPackage(tools::CFileSystem::createTemporaryFolder(std::string(), true));
         downloadedPackage.setFileName(packageName);

         shared::web::CFileDownloader::downloadFile(toDownload, downloadedPackage, progressReporter);
         return downloadedPackage;
      }


      Poco::Path CWorkerTools::downloadPackageAndVerify(const std::string& downloadUrl, const std::string& md5Hash,
                                                        shared::web::CFileDownloader::ProgressFunc progressReporter)
      {
         //determine the filename to download
         Poco::URI toDownload(downloadUrl);
         auto packageName = shared::web::CUriHelpers::getFile(toDownload);
         if (packageName.empty())
            packageName = "temp.zip";

         //determine local path
         auto downloadedPackage(tools::CFileSystem::createTemporaryFolder(std::string(), true));
         downloadedPackage.setFileName(packageName);

         shared::web::CFileDownloader::downloadFileAndVerify(toDownload, downloadedPackage, md5Hash, progressReporter);
         return downloadedPackage;
      }


      Poco::Path CWorkerTools::deployPackage(Poco::Path downloadedPackage, const std::string& outputDirectory)
      {
         /*
         When deploying a plugin we dont know the plugin name

         So deploy the plugin package to a random plugin directory (random is generated using uuid)
         When deployed, find the package.json and get the plugin name
         Rename the plugin directory to the good plugin name
         */

         //determine a random folder name (the folder name is the plugin name; but it it not known here)
         Poco::Path tempPluginFolder(outputDirectory);
         tempPluginFolder.append(shared::tools::CRandom::generateUUID());

         try
         {
            //extract to random pluginName location
            YADOMS_LOG(debug) << "Unzip " << downloadedPackage.toString() << " to " << tempPluginFolder.toString() << "...";
            shared::compression::CExtract unZipper;
            unZipper.to(downloadedPackage, tempPluginFolder);

            try
            {
               //read package.json file and get the pluginname
               Poco::Path packageJsonPath(tempPluginFolder.toString());
               packageJsonPath.append("package.json");

               YADOMS_LOG(debug) << "Read " << packageJsonPath.toString() << "...";

               shared::CDataContainer packageJson;
               const auto packageJsonPathString = packageJsonPath.toString();
               packageJson.deserializeFromFile(packageJsonPathString);

               //retreive the plugin name
               const auto pluginName = packageJson.get<std::string>("type");


               try
               {
                  Poco::Path realPluginFolder(outputDirectory);
                  realPluginFolder.append(pluginName);

                  //if plugin directory already exists; copy files; else just rename
                  if (tools::CFileSystem::exists(realPluginFolder))
                  {
                     //replace all files
                     tools::CFileSystem::copyDirectoryContentTo(tempPluginFolder, realPluginFolder);

                     //delete folder tempPluginFolder
                     tools::CFileSystem::remove(tempPluginFolder, true);
                  }
                  else
                  {
                     //rename random plugin folder to good plugin name
                     tools::CFileSystem::rename(tempPluginFolder, realPluginFolder);
                  }
                  return realPluginFolder;
               }
               catch (Poco::FileException& fileException)
               {
                  YADOMS_LOG(error) << "Operation fail :" << fileException.displayText();
                  throw;
               }
               catch (std::exception& ex)
               {
                  //delete folder tempPluginFolder
                  YADOMS_LOG(error) << "Operation fail (std::exception):" << ex.what();
                  tools::CFileSystem::remove(tempPluginFolder, true);
                  throw;
               }
            }
            catch (std::exception& ex)
            {
               //delete folder tempPluginFolder
               YADOMS_LOG(error) << "Operation fail (read json):" << ex.what();
               tools::CFileSystem::remove(tempPluginFolder, true);
               throw;
            }
         }
         catch (shared::exception::CExtract& ex)
         {
            YADOMS_LOG(error) << "Cannot extract package :" << ex.what();
            tools::CFileSystem::remove(tempPluginFolder, true);
            throw;
         }
      }

      std::string CWorkerTools::getWidgetBasePath() //TODO récupérer du IPathProvider
      {
         //retreive startup options
         const auto startupOptions = shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>();
         Poco::Path websiteFolder(startupOptions->getWebServerInitialPath());
         websiteFolder.append("widgets");
         return websiteFolder.toString();
      }

      std::string CWorkerTools::getPluginBasePath() //TODO récupérer du IPathProvider
      {
         //retreive startup options
         const auto startupOptions = shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>();
         return startupOptions->getPluginsPath();
      }

      std::string CWorkerTools::getScriptInterpreterBasePath() //TODO récupérer du IPathProvider
      {
         //retreive startup options
         const auto startupOptions = shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>();
         return startupOptions->getScriptInterpretersPath();
      }


      Poco::Path CWorkerTools::deployPluginPackage(Poco::Path downloadedPackage)
      {
         return deployPackage(downloadedPackage, getPluginBasePath());
      }

      Poco::Path CWorkerTools::deployWidgetPackage(Poco::Path downloadedPackage)
      {
         return deployPackage(downloadedPackage, getWidgetBasePath());
      }

      Poco::Path CWorkerTools::deployScriptInterpreterPackage(Poco::Path downloadedPackage)
      {
         return deployPackage(downloadedPackage, getScriptInterpreterBasePath());
      }

      void CWorkerTools::reportDownloadProgress(const std::string& file, float progress, WorkerProgressFunc callback, const std::string& function,
                                                float min, float max)
      {
         shared::CDataContainer callbackData;
         callbackData.set("file", file);
         callbackData.set("progress", progress);
         //progress is the progression of pure download (from 0 to 100)
         //so the download progress, will update the task progression between min and max
         const float fullProgression = min + (((max - min) / 100.0f) * progress);
         callback(true, fullProgression, function, std::string(), callbackData);
      }
   } // namespace worker
} // namespace update
