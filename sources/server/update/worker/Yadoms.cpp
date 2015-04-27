#include "stdafx.h"
#include "Yadoms.h"

#include <shared/Log.h>
#include "server/tools/tools/web/FileDownloader.h"
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

namespace update {
   namespace worker {

      Poco::URI CYadoms::m_baseDownloadUri("http://www.yadoms.com/downloads/update/");

      CYadoms::CYadoms(boost::shared_ptr<update::source::CYadoms> source)
         :m_source(source)
      {

      }

      CYadoms::~CYadoms()
      {

      }
      
      void CYadoms::checkForUpdateAsync(WorkerProgressFunc progressCallback)
      {

      }

      const std::string CYadoms::getPlatformFolder(boost::shared_ptr<IRunningInformation> & runningInformation)
      {
         std::string osName = Poco::toLower(runningInformation->getOperatingSystemName());

         //determine the main site folder
         std::string platform;
         if (boost::icontains(osName, "win"))
            platform = "windows";
         if (boost::icontains(osName, "darwin"))
            platform = "mac";
         if (boost::icontains(osName, "linux"))
         {
            if (boost::icontains(osName, "raspberrypi"))
               platform = "raspberrypi";
            else
               platform = "linux";
         }
         return platform;
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

      shared::CDataContainer CYadoms::step1DownloadLastVersionInfo(const std::string & platform)
      {
         std::ostringstream lastVersion;

         Poco::URI toDownload(m_baseDownloadUri);
         toDownload.resolve(platform + "/" + "lastVersion.json");

         int fileSize = tools::web::CFileDownloader::downloadFile(toDownload.toString(), lastVersion, boost::bind(&CYadoms::onDownloadReportProgress, this, _1, _2));
         if (fileSize == 0)
         {
            throw shared::exception::CException("Fail to download " + toDownload.toString());
         }
         return shared::CDataContainer(lastVersion.str());
      }

      Poco::Path CYadoms::step2DownloadPackage(const std::string & platform, const std::string & packageName, Poco::Path & tempFolder, const std::string & md5HashExcpected)
      {
         //determine local path
         Poco::Path package(tempFolder);
         package.setFileName(packageName);

         //create stream
         std::ofstream packageLocalFileStream(package.toString(), std::ios::binary);

         //download file
         Poco::URI toDownload(m_baseDownloadUri);
         toDownload.resolve(platform + "/" + packageName);

         int fileSize = tools::web::CFileDownloader::downloadFile(toDownload.toString(), packageLocalFileStream, boost::bind(&CYadoms::onDownloadReportProgress, this, _1, _2));
         packageLocalFileStream.close();
         //check file is downloaded
         Poco::File packageFile(package);
         if (!packageFile.exists() || fileSize == 0)
         {
            throw shared::exception::CException("Fail to download package at " + toDownload.toString());
         }

         //we re-read the file and compute the md5 (the md5 can be generated online using ie http://onlinemd5.com/)
         std::string md5HashCalculated = shared::encryption::CMD5().digestFile(package.toString().c_str());
         if (!boost::iequals(md5HashCalculated, md5HashExcpected))
         {
            //fail to verify checksum
            //remove file
            packageFile.remove();
            throw shared::exception::CException("Error during downloading package(invalid checksum)");
         }

         return package;
      }

      Poco::Path CYadoms::step3ExtractPackage(Poco::Path & downloadedPackage)
      {
         //verification of the extension
         std::string extension = downloadedPackage.getExtension();
         if ((!boost::iends_with(extension, "zip")) && (!boost::iends_with(extension, "tar.gz")))
            throw shared::exception::CException("Invalid extension package : " + downloadedPackage.toString() + ". Only zip or tar.gz supported.");


         //pour l'instant on prend ce qu'il y a dans temp sans faire l'extraction en attendant poco
         Poco::Path extractPath(downloadedPackage.parent());
         extractPath.pushDirectory(downloadedPackage.getBaseName());

         Poco::FileStream inp(downloadedPackage.toString(), std::ios::binary);

         // decompress to current working dir
         Poco::Zip::Decompress dec(inp, extractPath);
         // if an error happens invoke the ZipTest::onDecompressError method
         m_unzipError = false;
         dec.EError += Poco::Delegate<CYadoms, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &CYadoms::onDecompressError);
         dec.decompressAllFiles();
         dec.EError -= Poco::Delegate<CYadoms, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &CYadoms::onDecompressError);
         inp.close();
         if (m_unzipError)
         {
            //fail to unzip
            //ensure folder is removed
            Poco::File toDelete(extractPath);
            if (toDelete.exists() && extractPath.isDirectory())
               toDelete.remove(true);
            throw shared::exception::CException("Fail to uncompress package");
         }

         //extraction done with success, now delete downloaded file
         Poco::File toDelete(downloadedPackage);
         if (toDelete.exists())
            toDelete.remove();

         return extractPath;
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

      void CYadoms::onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
      {
         m_unzipError = true;
         m_unzipErrorMessage = info.first.getFileName() + " : " + info.second;
      }



      void CYadoms::updateAsync(WorkerProgressFunc progressCallback)
      {
         progressCallback(true, 0.0f, "Checking for a new update");

         //make some initializations
         boost::shared_ptr<IRunningInformation> runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
         std::string platform = getPlatformFolder(runningInformation);

         //////////////////////////////////////////////////////////
         // STEP1 : download lastVersion.json file
         //////////////////////////////////////////////////////////
         shared::CDataContainer lastVersionInformation = step1DownloadLastVersionInfo(platform);



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
            std::string md5HashExcpected = lastVersionInformation.get<std::string>("yadoms.information.md5Hash");

            progressCallback(true, 0.0f, "Downloading package");
            Poco::Path downloadedPackage = step2DownloadPackage(platform, packageName, tempFolder, md5HashExcpected);
            progressCallback(true, 50.0f, "Package " + packageName + " successfully downloaded");

            //////////////////////////////////////////////////////////
            // STEP3 : extract package
            //////////////////////////////////////////////////////////

            progressCallback(true, 50.0f, "Extracting package " + packageName);
            Poco::Path extractedPackageLocation = step3ExtractPackage(downloadedPackage);

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
  
   } // namespace worker
} // namespace update
