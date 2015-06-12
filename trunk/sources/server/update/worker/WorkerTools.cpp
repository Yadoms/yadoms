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
#include <shared/exception/Parse.hpp>


namespace update {
   namespace worker {

      //nested enum with specific strings
      DECLARE_ENUM_IMPLEMENTATION_NESTED(CWorkerTools::EUpdateState, EUpdateState,
         ((Success))
         ((Fail))
         ((Running))
      );



      Poco::Path CWorkerTools::downloadPackage(const std::string & downloadUrl, WorkerProgressFunc callback, const std::string & function, float min, float max)
      {
         return downloadPackage(downloadUrl, boost::bind(&CWorkerTools::reportDownloadProgress, _1, _2, callback, function, min, max));
      }

      Poco::Path CWorkerTools::downloadPackage(const std::string & downloadUrl)
      {
         return downloadPackage(downloadUrl, boost::bind(&shared::web::CFileDownloader::reportProgressToLog, _1, _2));
      }

      Poco::Path CWorkerTools::downloadPackage(const std::string & downloadUrl, shared::web::CFileDownloader::ProgressFunc progressReporter)
      {
         //determine the filename to download
         Poco::URI toDownload(downloadUrl);
         std::string packageName = shared::web::CUriHelpers::getFile(toDownload);
         if (packageName.empty())
            packageName = "temp.zip";

         //determine local path
         Poco::Path downloadedPackage(tools::CFileSystem::getTemporaryFolder());
         downloadedPackage.setFileName(packageName);

         shared::web::CFileDownloader::downloadFile(toDownload, downloadedPackage, progressReporter);
         return downloadedPackage;
      }


      Poco::Path CWorkerTools::deployPackage(Poco::Path downloadedPackage, const std::string & outputDirectory)
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
            catch (Poco::FileException & fileException)
            {
               YADOMS_LOG(error) << "Operation fail :" << fileException.displayText();
               throw fileException;
            }
            catch (std::exception & ex)
            {
               //delete folder tempPluginFolder
               YADOMS_LOG(error) << "Operation fail (std::exception):" << ex.what();
               tools::CFileSystem::remove(tempPluginFolder, true);
               throw;
            }
         }
         catch (std::exception & ex)
         {
            //delete folder tempPluginFolder
            YADOMS_LOG(error) << "Operation fail (read json):" << ex.what();
            tools::CFileSystem::remove(tempPluginFolder, true);
            throw;
         }
      }
      
      std::string CWorkerTools::getWidgetBasePath()
      {
         //retreive startup options
         boost::shared_ptr<startupOptions::IStartupOptions> startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();
         Poco::Path websiteFolder(startupOptions->getWebServerInitialPath());
         websiteFolder.append("widgets");
         return websiteFolder.toString();
      }

      std::string CWorkerTools::getPluginBasePath()
      {
         //retreive startup options
         boost::shared_ptr<startupOptions::IStartupOptions> startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();
         return startupOptions->getPluginsPath();
      }

      std::string CWorkerTools::getScriptInterpreterBasePath()
      {
         //retreive startup options
         boost::shared_ptr<startupOptions::IStartupOptions> startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();
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

      void CWorkerTools::reportDownloadProgress(const std::string & file, float progress, WorkerProgressFunc callback, const std::string & function, float min, float max)
      {
         shared::CDataContainer callbackData;
         callbackData.set("file", file);
         callbackData.set("progress", progress);
         //progress is the progression of pure download (from 0 to 100)
         //so the download progress, will update the task progression between min and max
         float fullProgression = min + (((max - min) / 100.0f) * progress);
         callback(true, fullProgression, function, callbackData);
      }

   } // namespace worker
} // namespace update
