#include "stdafx.h"
#include "Yadoms.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>
#include "RunningInformation.h"
#include <shared/ServiceLocator.h>
#include <Poco/Process.h>
#include "IApplicationStopHandler.h"
#include <shared/compression/Extract.h>
#include "tools/FileSystem.h"
#include "i18n/ClientStrings.h"

namespace update {
   namespace worker {

    
      void CYadoms::update(CWorkerTools::WorkerProgressFunc progressCallback, const shared::CDataContainer & versionToUpdateParam)
      {
         shared::CDataContainer  versionToUpdate = versionToUpdateParam;

         //make some initializations
         boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());

         YADOMS_LOG(information) << "Updating Yadoms : " << versionToUpdate.serialize();

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateYadomsUpdate, shared::CStringExtension::EmptyString, versionToUpdate);

         //////////////////////////////////////////////////////////
         // STEP2 : download package file
         //////////////////////////////////////////////////////////
         Poco::Path tempFolder = tools::CFileSystem::getTemporaryFolder();

         YADOMS_LOG(information) << "Temporary update folder :" << tempFolder.toString();

         //check all needed parameters are included
         if (!versionToUpdate.containsValue("downloadUrl"))
         {
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsRunUpdateFailed, "Bad info (downloadUrl) to update to", versionToUpdate);
         }
         else if (!versionToUpdate.containsValue("md5Hash"))
         {
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsRunUpdateFailed, "Bad info (md5Hash) to update to", versionToUpdate);
         }
         else if (!versionToUpdate.containsValue("commandToRun"))
         {
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsRunUpdateFailed, "Bad info (commandToRun) to update to", versionToUpdate);
         }
         else
         {
            std::string downloadUrl = versionToUpdate.get<std::string>("downloadUrl");
            std::string md5HashExpected = versionToUpdate.get<std::string>("md5Hash");

            try
            {
               YADOMS_LOG(information) << "Downloading package" << downloadUrl;
               progressCallback(true, 0.0f, i18n::CClientStrings::UpdateYadomsDownload, shared::CStringExtension::EmptyString, versionToUpdate);
               Poco::Path downloadedPackage = CWorkerTools::downloadPackageAndVerify(downloadUrl, md5HashExpected, progressCallback, i18n::CClientStrings::UpdateYadomsDownload, 0.0, 50.0);
               YADOMS_LOG(information) << "Package " << downloadUrl << " successfully downloaded";

               //////////////////////////////////////////////////////////
               // STEP3 : extract package
               //////////////////////////////////////////////////////////
               try
               {
                  YADOMS_LOG(information) << "Extracting downloaded package ";

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
                     std::string commandToRun = versionToUpdate.get<std::string>("commandToRun");
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
