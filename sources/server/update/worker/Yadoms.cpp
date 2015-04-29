#include "stdafx.h"
#include "Yadoms.h"

#include <shared/Log.h>
#include <shared/web/FileDownloader.h>
#include <shared/DataContainer.h>
#include "tools/Version.h"
#include <boost/functional/hash.hpp>
#include <shared/encryption/md5.h>
#include <fstream>
#include <Poco/String.h>
#include "RunningInformation.h"
#include <Poco/Zip/Decompress.h>
#include <Poco/Delegate.h>
#include <shared/ServiceLocator.h>
#include <Poco/Process.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <Poco/URI.h>
#include "IApplicationStopHandler.h"

#include "update/info/UpdateSite.h"
#include <shared/compression/Extract.h>

namespace update {
   namespace worker {

      CYadoms::CYadoms(boost::shared_ptr<update::source::CYadoms> source)
         :m_source(source)
      {

      }

      CYadoms::~CYadoms()
      {

      }
      
      void CYadoms::checkForUpdateAsync(WorkerProgressFunc progressCallback)
      {
         progressCallback(true, 0.0f, "Checking for a new update");

         //make some initializations
         boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
         boost::shared_ptr<startupOptions::IStartupOptions> startupOptions(shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>());

         update::info::CUpdateSite updateSite(startupOptions, runningInformation);

         //////////////////////////////////////////////////////////
         // STEP1 : download lastVersion.json file
         //////////////////////////////////////////////////////////
         shared::CDataContainer lastVersionInformation = shared::web::CFileDownloader::downloadInMemoryJsonFile(updateSite.getYadomsLastVersionUri(), boost::bind(&CYadoms::onDownloadReportProgress, this, _1, _2));

         //check for update
         tools::CVersion availableVersion(lastVersionInformation.get<std::string>("yadoms.information.version"));
         tools::CVersion currentVersion = runningInformation->getSoftwareVersion();

         if (availableVersion <= currentVersion)
         {
            progressCallback(true, 100.0f, "System is up to date");
         }
         else
         {
            progressCallback(true, 100.0f, "A new update is available");
         }
      }


      


      void CYadoms::updateAsync(WorkerProgressFunc progressCallback)
      {
         progressCallback(true, 0.0f, "Checking for a new update");

         //make some initializations
         boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
         boost::shared_ptr<startupOptions::IStartupOptions> startupOptions(shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>());

         update::info::CUpdateSite updateSite(startupOptions, runningInformation);

         //////////////////////////////////////////////////////////
         // STEP1 : download lastVersion.json file
         //////////////////////////////////////////////////////////
         shared::CDataContainer lastVersionInformation = shared::web::CFileDownloader::downloadInMemoryJsonFile(updateSite.getYadomsLastVersionUri(), boost::bind(&CYadoms::onDownloadReportProgress, this, _1, _2));

         //check for update
         tools::CVersion availableVersion(lastVersionInformation.get<std::string>("yadoms.information.version"));
         tools::CVersion currentVersion = runningInformation->getSoftwareVersion();

         if (availableVersion <= currentVersion)
         {
            progressCallback(true, 100.0f, "System is up to date");
         }
         else
         {
            progressCallback(true, 0.0f, "A new update is available");

            //////////////////////////////////////////////////////////
            // STEP2 : download package file
            //////////////////////////////////////////////////////////
            Poco::Path tempFolder = getTemporaryFolder();
            YADOMS_LOG(information) << "Temporary update folder :" << tempFolder.toString();

            std::string packageName = lastVersionInformation.get<std::string>("yadoms.information.softwarePackage");
            std::string md5HashExpected = lastVersionInformation.get<std::string>("yadoms.information.md5Hash");

            //determine local path
            Poco::Path downloadedPackage(tempFolder);
            downloadedPackage.setFileName(packageName);

            progressCallback(true, 0.0f, "Downloading package");
            shared::web::CFileDownloader::downloadFileAndVerify(updateSite.getYadomsPackageUri(packageName), downloadedPackage, md5HashExpected, boost::bind(&CYadoms::onDownloadReportProgress, this, _1, _2));
            progressCallback(true, 50.0f, "Package " + packageName + " successfully downloaded");

            //////////////////////////////////////////////////////////
            // STEP3 : extract package
            //////////////////////////////////////////////////////////

            progressCallback(true, 50.0f, "Extracting package " + packageName);
            shared::compression::CExtract unZipper;
            Poco::Path extractedPackageLocation = unZipper.here(downloadedPackage);

            //////////////////////////////////////////////////////////
            // STEP4 : run updater command
            //////////////////////////////////////////////////////////
            progressCallback(true, 90.0f, "Running updater");
            std::string commandToRun = lastVersionInformation.get<std::string>("yadoms.information.commandToRun");
            step4RunUpdaterProcess(extractedPackageLocation, commandToRun, runningInformation);
            
            //////////////////////////////////////////////////////////
            // STEP5 : exit yadoms
            //////////////////////////////////////////////////////////

            //exit yadoms
            progressCallback(true, 90.0f, "Exiting Yadoms");

            //demande de fermeture de l'application
            boost::shared_ptr<IApplicationStopHandler> stopHandler = shared::CServiceLocator::instance().get<IApplicationStopHandler>();
            if (stopHandler)
               stopHandler->requestToStop(IApplicationStopHandler::kYadomsOnly);
         }
      }


      void CYadoms::onDownloadReportProgress(const std::string & filename, float progression)
      {
         YADOMS_LOG(information) << "Downloading " << filename << ": " << progression << "%";
      }

      Poco::Path CYadoms::getTemporaryFolder()
      {
         //get the computer temp folder
         Poco::Path p(Poco::Path::temp());

         //append directory yadoms
         p.pushDirectory("yadoms");

         //remove directory if exists
         Poco::File f(p);
         if (f.exists())
            f.remove(true);

         //create directories
         f.createDirectories();

         return p;
      }


      void CYadoms::step4RunUpdaterProcess(Poco::Path & extractedPackageLocation, const std::string & commandtoRun, boost::shared_ptr<IRunningInformation> & runningInformation)
      {
         //append the command lien request to the extracted path
         Poco::Path executablePath(extractedPackageLocation);
         executablePath.setFileName(commandtoRun);

         //create the argument list
         Poco::Process::Args args;
         Poco::Path p(runningInformation->getExecutablePath());
         args.push_back(p.parent().toString());

         //run updater script
         Poco::ProcessHandle handle = Poco::Process::launch(executablePath.toString(), args);

         //the update command is running, wait for 5 seconds and ensure it is always running
         boost::this_thread::sleep(boost::posix_time::seconds(5));

         if (!Poco::Process::isRunning(handle))
            throw shared::exception::CException("The update command script terminated prematurely");
      }


  
   } // namespace worker
} // namespace update
