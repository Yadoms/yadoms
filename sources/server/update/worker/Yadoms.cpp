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
#include <Poco/Delegate.h>
#include <shared/ServiceLocator.h>
#include <Poco/Process.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <Poco/URI.h>
#include "IApplicationStopHandler.h"

#include "update/info/UpdateSite.h"
#include <shared/compression/Extract.h>
#include "tools/FileSystem.h"

namespace update {
   namespace worker {

      CYadoms::CYadoms()
      {

      }

      CYadoms::~CYadoms()
      {

      }
      
      void CYadoms::checkForUpdate(WorkerProgressFunc progressCallback)
      {
         progressCallback(true, 0.0f, "Checking for a new update", shared::CDataContainer::EmptyContainer);

         //make some initializations
         boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
         boost::shared_ptr<startupOptions::IStartupOptions> startupOptions(shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>());

         update::info::CUpdateSite updateSite(startupOptions, runningInformation);

         //////////////////////////////////////////////////////////
         // STEP1 : download lastVersion.json file
         //////////////////////////////////////////////////////////
         shared::CDataContainer versionToUpdate = shared::web::CFileDownloader::downloadInMemoryJsonFile(updateSite.getYadomsLastVersionUri(), boost::bind(&shared::web::CFileDownloader::reportProgressToLog, _1, _2));

         //check for update
         tools::CVersion availableVersion(versionToUpdate.get<std::string>("yadoms.information.version"));
         tools::CVersion currentVersion = runningInformation->getSoftwareVersion();

         if (availableVersion <= currentVersion)
         {
            progressCallback(true, 100.0f, "System is up to date", shared::CDataContainer::EmptyContainer);
         }
         else
         {
            progressCallback(true, 100.0f, "A new update is available", shared::CDataContainer::EmptyContainer);
         }
      }


      


      void CYadoms::update(shared::CDataContainer & versionToUpdate, WorkerProgressFunc progressCallback)
      {
         progressCallback(true, 0.0f, "Checking for a new update", shared::CDataContainer::EmptyContainer);

         //make some initializations
         boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
         boost::shared_ptr<startupOptions::IStartupOptions> startupOptions(shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>());

         update::info::CUpdateSite updateSite(startupOptions, runningInformation);


         progressCallback(true, 0.0f, "A new update is available", shared::CDataContainer::EmptyContainer);

         //////////////////////////////////////////////////////////
         // STEP2 : download package file
         //////////////////////////////////////////////////////////
         Poco::Path tempFolder = tools::CFileSystem::getTemporaryFolder();

         YADOMS_LOG(information) << "Temporary update folder :" << tempFolder.toString();

         //if versionToUpdate do not contains valid version information; get last version from UpdateSite
         if (!versionToUpdate.containsValue("yadoms.information.version"))
            versionToUpdate = shared::web::CFileDownloader::downloadInMemoryJsonFile(updateSite.getYadomsLastVersionUri(), boost::bind(&shared::web::CFileDownloader::reportProgressToLog, _1, _2));

         std::string packageName = versionToUpdate.get<std::string>("yadoms.information.softwarePackage");
         std::string md5HashExpected = versionToUpdate.get<std::string>("yadoms.information.md5Hash");

         //determine local path
         Poco::Path downloadedPackage(tempFolder);
         downloadedPackage.setFileName(packageName);

         progressCallback(true, 0.0f, "Downloading package", shared::CDataContainer::EmptyContainer);
         shared::web::CFileDownloader::downloadFileAndVerify(updateSite.getYadomsPackageUri(packageName), downloadedPackage, md5HashExpected, boost::bind(&shared::web::CFileDownloader::reportProgressToLog, _1, _2));
         progressCallback(true, 50.0f, "Package " + packageName + " successfully downloaded", shared::CDataContainer::EmptyContainer);

         //////////////////////////////////////////////////////////
         // STEP3 : extract package
         //////////////////////////////////////////////////////////

         progressCallback(true, 50.0f, "Extracting package " + packageName, shared::CDataContainer::EmptyContainer);
         shared::compression::CExtract unZipper;
         Poco::Path extractedPackageLocation = unZipper.here(downloadedPackage);

         //////////////////////////////////////////////////////////
         // STEP4 : run updater command
         //////////////////////////////////////////////////////////
         progressCallback(true, 90.0f, "Running updater", shared::CDataContainer::EmptyContainer);
         std::string commandToRun = versionToUpdate.get<std::string>("yadoms.information.commandToRun");
         step4RunUpdaterProcess(extractedPackageLocation, commandToRun, runningInformation);
            
         //////////////////////////////////////////////////////////
         // STEP5 : exit yadoms
         //////////////////////////////////////////////////////////

         //exit yadoms
         progressCallback(true, 90.0f, "Exiting Yadoms", shared::CDataContainer::EmptyContainer);

         //demande de fermeture de l'application
         boost::shared_ptr<IApplicationStopHandler> stopHandler = shared::CServiceLocator::instance().get<IApplicationStopHandler>();
         if (stopHandler)
            stopHandler->requestToStop(IApplicationStopHandler::kYadomsOnly);
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
