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

#include "i18n/ClientStrings.h"

namespace update {
   namespace worker {

     

      void CYadoms::checkForUpdate(CWorkerTools::WorkerProgressFunc progressCallback)
      {
         try
         {
            //make some initializations
            boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
            boost::shared_ptr<startupOptions::IStartupOptions> startupOptions(shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>());
            update::info::CUpdateSite updateSite(startupOptions, runningInformation);


            YADOMS_LOG(information) << "Checking for a new update";
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateYadomsCheckForUpdate, shared::CStringExtension::EmptyString, shared::CDataContainer::EmptyContainer);
         
            //////////////////////////////////////////////////////////
            // STEP1 : download lastVersion.json file
            //////////////////////////////////////////////////////////
            shared::CDataContainer lastVersionInformationFromUpdateSite = shared::web::CFileDownloader::downloadInMemoryJsonFile(updateSite.getYadomsLastVersionUri(), boost::bind(&shared::web::CFileDownloader::reportProgressToLog, _1, _2));

            //check for update
            tools::CVersion availableVersion(lastVersionInformationFromUpdateSite.get<std::string>("yadoms.information.version"));
            tools::CVersion currentVersion = runningInformation->getSoftwareVersion();

            if (availableVersion <= currentVersion)
            {
               YADOMS_LOG(information) << "System is up to date";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdateYadomsUpToDate , shared::CStringExtension::EmptyString, lastVersionInformationFromUpdateSite);
            }
            else
            {
               YADOMS_LOG(information) << "A new update is available";
               progressCallback(true, 100.0f, i18n::CClientStrings::UpdateYadomsUpdateAvailable, shared::CStringExtension::EmptyString, lastVersionInformationFromUpdateSite);
            }
         }
         catch (std::exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to check for update : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsCheckForUpdateFailed, ex.what(), shared::CDataContainer::EmptyContainer);
         }
      }
            


      void CYadoms::update(CWorkerTools::WorkerProgressFunc progressCallback, const shared::CDataContainer & versionToUpdateParam)
      {
         shared::CDataContainer  versionToUpdate = versionToUpdateParam;

         //make some initializations
         boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
         boost::shared_ptr<startupOptions::IStartupOptions> startupOptions(shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>());

         update::info::CUpdateSite updateSite(startupOptions, runningInformation);

         YADOMS_LOG(information) << "Updating Yadoms : " << versionToUpdate.serialize();


         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateYadomsUpdate, shared::CStringExtension::EmptyString, versionToUpdate);

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

         try
         {
            YADOMS_LOG(information) << "Downloading package" << updateSite.getYadomsPackageUri(packageName).toString();
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateYadomsDownload, shared::CStringExtension::EmptyString, versionToUpdate);
            Poco::Path downloadedPackage = CWorkerTools::downloadPackageAndVerify(updateSite.getYadomsPackageUri(packageName).toString(), md5HashExpected,  progressCallback, i18n::CClientStrings::UpdateYadomsDownload, 0.0, 50.0);
            YADOMS_LOG(information) << "Package " << packageName << " successfully downloaded";

            //////////////////////////////////////////////////////////
            // STEP3 : extract package
            //////////////////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Extracting package " << packageName;

               progressCallback(true, 50.0f, i18n::CClientStrings::UpdateYadomsExtract, shared::CStringExtension::EmptyString, versionToUpdate);
               shared::compression::CExtract unZipper;
               Poco::Path extractedPackageLocation = unZipper.here(downloadedPackage);

               //////////////////////////////////////////////////////////
               // STEP4 : run updater command
               //////////////////////////////////////////////////////////
               try
               {
                  YADOMS_LOG(information) << "Running updater";
                  progressCallback(true, 90.0f, i18n::CClientStrings::UpdateYadomsRunUpdate, shared::CStringExtension::EmptyString, versionToUpdate);
                  std::string commandToRun = versionToUpdate.get<std::string>("yadoms.information.commandToRun");
                  step4RunUpdaterProcess(extractedPackageLocation, commandToRun, runningInformation);

                  //////////////////////////////////////////////////////////
                  // STEP5 : exit yadoms
                  //////////////////////////////////////////////////////////

                  //exit yadoms
                  YADOMS_LOG(information) << "Exiting Yadoms";
                  progressCallback(true, 100.0f, i18n::CClientStrings::UpdateYadomsExit, shared::CStringExtension::EmptyString, versionToUpdate);

                  //demande de fermeture de l'application
                  boost::shared_ptr<IApplicationStopHandler> stopHandler = shared::CServiceLocator::instance().get<IApplicationStopHandler>();
                  if (stopHandler)
                     stopHandler->requestToStop(IApplicationStopHandler::kYadomsOnly);
               }
               catch (std::exception & ex)
               {
                  //fail to run updater
                  YADOMS_LOG(error) << "Fail to run updater : " << ex.what();
                  progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsRunUpdateFailed, ex.what(), versionToUpdate);

                  //remove folder
                  tools::CFileSystem::remove(extractedPackageLocation, true);
               }
            }
            catch (std::exception & ex)
            {
               //fail to download package
               YADOMS_LOG(error) << "Fail to extract package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsExtractFailed, ex.what(), versionToUpdate);
            }

            //no more need downloaded zip package
            tools::CFileSystem::remove(downloadedPackage, false);
         }
         catch (std::exception & ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download package : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsDownloadFailed, ex.what(), versionToUpdate);
         }

         
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
