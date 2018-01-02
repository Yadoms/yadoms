#include "stdafx.h"
#include "Yadoms.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/ServiceLocator.h>
#include <Poco/Process.h>
#include <shared/compression/Extract.h>
#include "tools/FileSystem.h"
#include "i18n/ClientStrings.h"
#include <shared/process/SoftwareStop.h> 
#include "tools/OperatingSystem.h"
#include <boost/process/environment.hpp>

namespace update
{
   namespace worker
   {
      void CYadoms::update(CWorkerTools::WorkerProgressFunc progressCallback,
                           const shared::CDataContainer& versionToUpdateParam)
      {
         auto versionToUpdate = versionToUpdateParam;

         //make some initializations
         auto runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());

         YADOMS_LOG(information) << "Updating Yadoms : " << versionToUpdate.serialize();

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateYadomsUpdate, std::string(), versionToUpdate);

         //////////////////////////////////////////////////////////
         // STEP2 : download package file
         //////////////////////////////////////////////////////////

         //check all needed parameters are included
         if (!versionToUpdate.containsValue("downloadUrl"))
         {
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsUpdateFailed, "Bad info (downloadUrl) to update to", versionToUpdate);
            return;
         }

         if (!versionToUpdate.containsValue("md5Hash"))
         {
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsUpdateFailed, "Bad info (md5Hash) to update to", versionToUpdate);
            return;
         }

         if (!versionToUpdate.containsValue("commandToRun"))
         {
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsUpdateFailed, "Bad info (commandToRun) to update to", versionToUpdate);
            return;
         }


         const auto downloadUrl = versionToUpdate.get<std::string>("downloadUrl");
         const auto md5HashExpected = versionToUpdate.get<std::string>("md5Hash");

         try
         {
            YADOMS_LOG(information) << "Downloading package " << downloadUrl;
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateYadomsDownload, std::string(), versionToUpdate);
            auto downloadedPackage = CWorkerTools::downloadPackageAndVerify(downloadUrl, md5HashExpected, progressCallback, i18n::CClientStrings::UpdateYadomsDownload, 0.0, 50.0);
            YADOMS_LOG(information) << "Package successfully downloaded into " << downloadedPackage.toString();

            //////////////////////////////////////////////////////////
            // STEP3 : extract package
            //////////////////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Extracting downloaded package";

               progressCallback(true, 50.0f, i18n::CClientStrings::UpdateYadomsExtract, std::string(), versionToUpdate);
               shared::compression::CExtract unZipper;
               auto extractedPackageLocation = unZipper.here(downloadedPackage);

               //////////////////////////////////////////////////////////
               // STEP4 : run updater command
               //////////////////////////////////////////////////////////
               try
               {
                  YADOMS_LOG(information) << "Running updater";
                  progressCallback(true, 90.0f, i18n::CClientStrings::UpdateYadomsDeploy, std::string(), versionToUpdate);
                  const auto commandToRun = versionToUpdate.get<std::string>("commandToRun");
                  step4RunUpdaterProcess(extractedPackageLocation, commandToRun, runningInformation);

                  //////////////////////////////////////////////////////////
                  // STEP5 : exit yadoms
                  //////////////////////////////////////////////////////////

                  //exit yadoms
                  YADOMS_LOG(information) << "Exiting Yadoms";
                  progressCallback(true, 100.0f, i18n::CClientStrings::UpdateYadomsExit, std::string(), versionToUpdate);

                  //sleep 1 sec, to ensure clients receive last notification
                  boost::this_thread::sleep(boost::posix_time::seconds(1));

                  //ask to close application (asynchronously as Yadoms will want to free all resources now and CSoftwareStop::stop blocks)
                  boost::thread asyncStop(&shared::process::CSoftwareStop::stop);
               }
               catch (std::exception& ex)
               {
                  //fail to run updater
                  YADOMS_LOG(error) << "Fail to run updater : " << ex.what();
                  progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsDeployFailed, ex.what(), versionToUpdate);

                  //remove folder
                  tools::CFileSystem::remove(extractedPackageLocation, true);
               }
            }
            catch (std::exception& ex)
            {
               //fail to download package
               YADOMS_LOG(error) << "Fail to extract package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsExtractFailed, ex.what(), versionToUpdate);
            }

            //no more need downloaded zip package
            tools::CFileSystem::remove(downloadedPackage, false);
         }
         catch (std::exception& ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download package : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsDownloadFailed, ex.what(), versionToUpdate);
         }
      }


      void CYadoms::step4RunUpdaterProcess(Poco::Path& extractedPackageLocation,
                                           const std::string& commandtoRun,
                                           boost::shared_ptr<IRunningInformation>& runningInformation)
      {
         //append the command line request to the extracted path
         auto executablePath(extractedPackageLocation);
         executablePath.setFileName(commandtoRun);

         //create the argument list
         Poco::Process::Args args;
         args.push_back(std::to_string(boost::this_process::get_id()));
         args.push_back(Poco::Path(runningInformation->getExecutablePath()).parent().toString());

         //run updater script
         YADOMS_LOG(debug) << "Launch script \"" << executablePath.toString() << "\" with args " << boost::algorithm::join(args, ", ");
         const auto handle = tools::COperatingSystem::launchNativeScript(executablePath.toString(), args);

         //the update command is running, wait for 5 seconds and ensure it is always running
         boost::this_thread::sleep(boost::posix_time::seconds(5));

         if (!Poco::Process::isRunning(handle))
            throw shared::exception::CException("The update command script terminated prematurely");
      }
   } // namespace worker
} // namespace update


