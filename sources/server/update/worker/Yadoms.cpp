#include "stdafx.h"
#include "Yadoms.h"
#include <shared/Log.h>
#include <shared/ServiceLocator.h>
#include <Poco/Process.h>
#include <shared/compression/Extract.h>
#include "i18n/ClientStrings.h"
#include <shared/process/SoftwareStop.h> 
#include "tools/OperatingSystem.h"
#include <boost/process/environment.hpp>

#include "shared/tools/Filesystem.h"

namespace update
{
   namespace worker
   {
      void CYadoms::update(const CWorkerHelpers::WorkerProgressFunc& progressCallback,
                           const std::string& downloadUrl,
                           const std::string& expectedMd5Hash)
      {
         //make some initializations
         auto runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());

         YADOMS_LOG(information) << "Updating Yadoms from " << downloadUrl;

         auto callbackData = shared::CDataContainer::make();
         callbackData->set("downloadUrl", downloadUrl);

         progressCallback(true, 0.0f, i18n::CClientStrings::UpdateYadomsUpdate, std::string(), callbackData);

         //////////////////////////////////////////////////////////
         // STEP2 : download package file
         //////////////////////////////////////////////////////////

         try
         {
            YADOMS_LOG(information) << "Downloading package " << downloadUrl;
            progressCallback(true, 0.0f, i18n::CClientStrings::UpdateYadomsDownload, std::string(), callbackData);
            auto downloadedPackage = CWorkerHelpers::downloadPackageAndVerify(downloadUrl, expectedMd5Hash, progressCallback,
                                                                            i18n::CClientStrings::UpdateYadomsDownload, 0.0, 50.0);
            YADOMS_LOG(information) << "Package successfully downloaded into " << downloadedPackage.string();

            //////////////////////////////////////////////////////////
            // STEP3 : extract package
            //////////////////////////////////////////////////////////
            try
            {
               YADOMS_LOG(information) << "Extracting downloaded package";

               progressCallback(true, 50.0f, i18n::CClientStrings::UpdateYadomsExtract, std::string(), callbackData);
               shared::compression::CExtract unZipper;
               auto extractedPackageLocation = unZipper.here(downloadedPackage);

               //////////////////////////////////////////////////////////
               // STEP4 : run updater command
               //////////////////////////////////////////////////////////
               try
               {
                  YADOMS_LOG(information) << "Find update command";
                  shared::CDataContainer packageJson;
                  packageJson.deserializeFromFile((extractedPackageLocation / "package.json").string());
                  const auto& commandToRun = packageJson.get<std::string>("yadoms.information.commandToRun");

                  YADOMS_LOG(information) << "Running updater";
                  progressCallback(true, 90.0f, i18n::CClientStrings::UpdateYadomsDeploy, std::string(), callbackData);
                  step4RunUpdaterProcess(extractedPackageLocation, commandToRun, runningInformation);

                  //////////////////////////////////////////////////////////
                  // STEP5 : exit yadoms
                  //////////////////////////////////////////////////////////

                  //exit yadoms
                  YADOMS_LOG(information) << "Exiting Yadoms";
                  progressCallback(true, 100.0f, i18n::CClientStrings::UpdateYadomsExit, std::string(), callbackData);

                  //sleep 1 sec, to ensure clients receive last notification
                  boost::this_thread::sleep(boost::posix_time::seconds(1));

                  //ask to close application (asynchronously as Yadoms will want to free all resources now and CSoftwareStop::stop blocks)
                  boost::thread asyncStop(&shared::process::CSoftwareStop::stop);
               }
               catch (std::exception& ex)
               {
                  //fail to run updater
                  YADOMS_LOG(error) << "Fail to run updater : " << ex.what();
                  progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsDeployFailed, ex.what(), callbackData);

                  //remove folder
                  shared::tools::CFilesystem::remove(extractedPackageLocation, true);
               }
            }
            catch (std::exception& ex)
            {
               //fail to download package
               YADOMS_LOG(error) << "Fail to extract package : " << ex.what();
               progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsExtractFailed, ex.what(), callbackData);
            }

            //no more need downloaded zip package
            shared::tools::CFilesystem::remove(downloadedPackage, false);
         }
         catch (std::exception& ex)
         {
            //fail to download package
            YADOMS_LOG(error) << "Fail to download package : " << ex.what();
            progressCallback(false, 100.0f, i18n::CClientStrings::UpdateYadomsDownloadFailed, ex.what(), callbackData);
         }
      }


      void CYadoms::step4RunUpdaterProcess(const boost::filesystem::path& extractedPackageLocation,
                                           const std::string& commandToRun,
                                           boost::shared_ptr<IRunningInformation>& runningInformation)
      {
         //append the command line request to the extracted path
         const auto executablePath(extractedPackageLocation / commandToRun);

         //create the argument list
         Poco::Process::Args args;
         args.push_back(std::to_string(boost::this_process::get_id()));
         args.push_back(Poco::Path(runningInformation->getExecutablePath()).parent().toString());

         //run updater script
         YADOMS_LOG(debug) << "Launch script \"" << executablePath.string() << "\" with args " << boost::algorithm::join(args, ", ");
         const auto handle = tools::COperatingSystem::launchNativeScript(executablePath.string(), args);

         //the update command is running, wait for 5 seconds and ensure it is always running
         boost::this_thread::sleep(boost::posix_time::seconds(5));

         if (!Poco::Process::isRunning(handle))
            throw shared::exception::CException("The update command script terminated prematurely");
      }
   } // namespace worker
} // namespace update
