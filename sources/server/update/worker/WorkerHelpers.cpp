#include "stdafx.h"
#include "WorkerHelpers.h"

#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/http/UrlHelpers.h>
#include <shared/http/FileDownloader.h>
#include <shared/compression/Extract.h>

#include "startupOptions/IStartupOptions.h"

#include <shared/tools/Random.h>
#include <shared/exception/Extract.hpp>
#include <utility>

#include "shared/tools/Filesystem.h"


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

         auto targetPath(shared::tools::CFilesystem::createTemporaryFolder());
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

         auto targetPath(shared::tools::CFilesystem::createTemporaryFolder());
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
         When deploying an item package we don't know the item name

         So deploy the package to a random plugin directory (random is generated using uuid)
         When deployed, find the package.json and get the item name
         Rename the item directory to the good item name
         */

         //determine a random folder name (the folder name is the item name; but it it not known here)
         boost::filesystem::path tempItemFolder(outputDirectory);
         tempItemFolder.append(shared::tools::CRandom::generateUUID());

         try
         {
            //extract to random itemName location
            YADOMS_LOG(debug) << "Unzip " << downloadedPackage.string() << " to " << tempItemFolder.string() <<
               "...";
            shared::compression::CExtract unZipper;
            unZipper.to(downloadedPackage, tempItemFolder);

            try
            {
               //read package.json file and get the item name
               Poco::Path packageJsonPath(tempItemFolder.string());
               packageJsonPath.append("package.json");

               YADOMS_LOG(debug) << "Read " << packageJsonPath.toString() << "...";

               shared::CDataContainer packageJson;
               const auto packageJsonPathString = packageJsonPath.toString();
               packageJson.deserializeFromFile(packageJsonPathString);

               //retrieve the plugin name
               const auto itemName = packageJson.get<std::string>("type");


               try
               {
                  boost::filesystem::path realItemFolder(outputDirectory);
                  realItemFolder.append(itemName);

                  //if item directory already exists, remove it
                  if (shared::tools::CFilesystem::exists(realItemFolder.string()))
                     shared::tools::CFilesystem::remove(realItemFolder.string(), true);

                  //rename random item folder to good item name
                  shared::tools::CFilesystem::rename(tempItemFolder, realItemFolder);

                  return realItemFolder;
               }
               catch (Poco::FileException& fileException)
               {
                  YADOMS_LOG(error) << "Operation fail :" << fileException.displayText();
                  throw;
               }
               catch (std::exception& ex)
               {
                  //delete folder tempItemFolder
                  YADOMS_LOG(error) << "Operation fail (std::exception):" << ex.what();
                  shared::tools::CFilesystem::remove(tempItemFolder, true);
                  throw;
               }
            }
            catch (std::exception& ex)
            {
               //delete folder tempItemFolder
               YADOMS_LOG(error) << "Operation fail (read json):" << ex.what();
               shared::tools::CFilesystem::remove(tempItemFolder, true);
               throw;
            }
         }
         catch (shared::exception::CExtract& ex)
         {
            YADOMS_LOG(error) << "Cannot extract package :" << ex.what();
            shared::tools::CFilesystem::remove(tempItemFolder, true);
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
