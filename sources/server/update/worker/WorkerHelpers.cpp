#include "stdafx.h"
#include "WorkerHelpers.h"

#include <Poco/File.h>
#include <Poco/URI.h>

#include <shared/Log.h>
#include <shared/DataContainer.h>
#include "tools/FileSystem.h"
#include <shared/http/UrlHelpers.h>
#include <shared/http/FileDownloader.h>
#include <shared/compression/Extract.h>

#include "startupOptions/IStartupOptions.h"

#include <shared/tools/Random.h>
#include <shared/exception/Extract.hpp>
#include <utility>


namespace update
{
   namespace worker
   {
      boost::filesystem::path CWorkerHelpers::downloadPackage(const std::string& downloadUrl,
                                                              WorkerProgressFunc callback,
                                                              const std::string& function,
                                                              float min,
                                                              float max)
      {
         return downloadPackage(downloadUrl,
                                [&](const std::string& message, float progress)
                                {
                                   reportDownloadProgress(message,
                                                          progress,
                                                          callback,
                                                          function,
                                                          min,
                                                          max);
                                });
      }

      boost::filesystem::path CWorkerHelpers::downloadPackageAndVerify(const std::string& downloadUrl,
                                                                       const std::string& md5Hash,
                                                                       WorkerProgressFunc callback,
                                                                       const std::string& function,
                                                                       float min,
                                                                       float max)
      {
         return downloadPackageAndVerify(downloadUrl,
                                         md5Hash,
                                         [&](const std::string& message, float progress)
                                         {
                                            reportDownloadProgress(message,
                                                                   progress,
                                                                   callback,
                                                                   function,
                                                                   min,
                                                                   max);
                                         });
      }

      boost::filesystem::path CWorkerHelpers::downloadPackage(const std::string& downloadUrl)
      {
         return downloadPackage(downloadUrl,
                                [](const std::string& message, float progress)
                                {
                                   YADOMS_LOG(information) << "Downloading " << message << " : " << boost::
                                      format("%11.0f") % progress << " %";
                                });
      }

      boost::filesystem::path CWorkerHelpers::downloadPackage(const std::string& downloadUrl,
                                                              shared::http::CFileDownloader::onProgressFunc
                                                              progressReporter)
      {
         auto packageName = shared::http::CUrlHelpers::getFileName(downloadUrl);
         if (packageName.empty())
            packageName = "temp.zip";

         auto targetPath(tools::CFileSystem::createTemporaryFolder());
         targetPath /= packageName;

         shared::http::CFileDownloader::downloadFile(downloadUrl,
                                                     targetPath,
                                                     std::move(progressReporter));
         return targetPath;
      }


      boost::filesystem::path CWorkerHelpers::downloadPackageAndVerify(const std::string& downloadUrl,
                                                                       const std::string& md5Hash,
                                                                       shared::http::CFileDownloader::onProgressFunc
                                                                       progressReporter)
      {
         auto packageName = shared::http::CUrlHelpers::getFileName(downloadUrl);
         if (packageName.empty())
            packageName = "temp.zip";

         auto targetPath(tools::CFileSystem::createTemporaryFolder());
         targetPath /= packageName;

         shared::http::CFileDownloader::downloadFileAndVerify(downloadUrl,
                                                              targetPath,
                                                              md5Hash,
                                                              std::move(progressReporter));
         return targetPath;
      }


      boost::filesystem::path CWorkerHelpers::deployPackage(const boost::filesystem::path& downloadedPackage,
                                                            const std::string& outputDirectory)
      {
         /*
         When deploying a plugin we don't know the plugin name

         So deploy the plugin package to a random plugin directory (random is generated using uuid)
         When deployed, find the package.json and get the plugin name
         Rename the plugin directory to the good plugin name
         */

         //determine a random folder name (the folder name is the plugin name; but it it not known here)
         boost::filesystem::path tempPluginFolder(outputDirectory);
         tempPluginFolder.append(shared::tools::CRandom::generateUUID());

         try
         {
            //extract to random pluginName location
            YADOMS_LOG(debug) << "Unzip " << downloadedPackage.string() << " to " << tempPluginFolder.string() <<
               "...";
            shared::compression::CExtract unZipper;
            unZipper.to(downloadedPackage, tempPluginFolder);

            try
            {
               //read package.json file and get the plugin name
               Poco::Path packageJsonPath(tempPluginFolder.string());
               packageJsonPath.append("package.json");

               YADOMS_LOG(debug) << "Read " << packageJsonPath.toString() << "...";

               shared::CDataContainer packageJson;
               const auto packageJsonPathString = packageJsonPath.toString();
               packageJson.deserializeFromFile(packageJsonPathString);

               //retrieve the plugin name
               const auto pluginName = packageJson.get<std::string>("type");


               try
               {
                  boost::filesystem::path realPluginFolder(outputDirectory);
                  realPluginFolder.append(pluginName);

                  //if plugin directory already exists; copy files; else just rename
                  if (tools::CFileSystem::exists(realPluginFolder.string()))
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

      void CWorkerHelpers::reportDownloadProgress(const std::string& file,
                                                  float progress,
                                                  const WorkerProgressFunc callback,
                                                  const std::string& function,
                                                  float min,
                                                  float max)
      {
         auto callbackData = shared::CDataContainer::make();
         callbackData->set("file", file);
         callbackData->set("progress", progress);
         //progress is the progression of pure download (from 0 to 100)
         //so the download progress, will update the task progression between min and max
         const auto fullProgression = min + (((max - min) / 100.0f) * progress);
         callback(true, fullProgression, function, std::string(), callbackData);
      }
   } // namespace worker
} // namespace update
