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

namespace task { namespace update {

   std::string CYadoms::m_taskName = "system.yadomsUpdate";

   CYadoms::CYadoms()
   {
   }

   CYadoms::~CYadoms()
   {
   }

   const std::string & CYadoms::getName()
   {
      return m_taskName;
   }

   void CYadoms::onDownloadReportProgress(const std::string & filename, float progression)
   {
      YADOMS_LOG(information) << "Downloading " << filename << ": " << progression << "%";
   }

   bool CYadoms::doWork(TaskProgressFunc progressCallback)
   {
      try
      {
         progressCallback(true, 0.0f, "Checking for a new update");

         std::string baseUrl = "http://www.yadoms.com/download";

         std::string platform = Poco::toLower(CRunningInformation().getOperatingSystemName());

         std::ostringstream lastVersion;
         tools::web::CFileDownloader::downloadFile(baseUrl + "/" + platform + "/lastversion.json",
                                                   lastVersion, 
                                                   boost::bind(&CYadoms::onDownloadReportProgress, this, _1, _2));
         std::string data = lastVersion.str();
         shared::CDataContainer lv(data);

         std::string versionAsString = lv.get<std::string>("yadoms.information.version");
         tools::CVersion availableVersion(versionAsString);
         tools::CVersion currentVersion = CRunningInformation().getSoftwareVersion();

         if (availableVersion <= currentVersion)
         {
            progressCallback(true, 100.0f, "System is up to date");
         }
         else
         {
            progressCallback(true, 0.0f, "A new update is available");
            progressCallback(true, 0.0f, "Downloading package");
            std::string fileToDownload = lv.get<std::string>("yadoms.information.softwarePackage");
            
            //get temp folder from a global system provider
            Poco::Path p(Poco::Path::temp());
            p.pushDirectory("yadoms");
            boost::filesystem::path tempFolder(p.toString());
            
            //this part must be managed by the global folder before returning it
            if (!boost::filesystem::exists(tempFolder))
            {
               boost::filesystem::create_directory(tempFolder);
            }
            
            boost::filesystem::path packageLocalFilePath = tempFolder / fileToDownload;

            std::ofstream packageFile(packageLocalFilePath.string(), std::ios::binary);

            tools::web::CFileDownloader::downloadFile(baseUrl + "/" + platform + "/" + fileToDownload,
               packageFile,
               boost::bind(&CYadoms::onDownloadReportProgress, this, _1, _2));

            packageFile.close();

            if (!boost::filesystem::exists(packageLocalFilePath))
            {
               progressCallback(false, 100.0f, "Error during downloading package");
               return false;
            }
            
            //we re-read the file and compute the md5 (the md5 can be generated online using ie http://onlinemd5.com/)
            shared::encryption::CMD5 md5Hasher;
            std::string md5HashCalculated = md5Hasher.digestFile(packageLocalFilePath.string().c_str());

            YADOMS_LOG(information) << "MD5 Hash calculated: " << md5HashCalculated;
            
            std::string md5HashExcpected = lv.get<std::string>("yadoms.information.md5Hash");

            if (!boost::iequals(md5HashCalculated, md5HashExcpected))
            {
               progressCallback(false, 100.0f, "Error during downloading package (invalid checksum)");
               return false;
            }

            progressCallback(true, 50.0f, "Package " + fileToDownload + "successfully downloaded");
            
            //verification of the extension
            std::string extension = boost::filesystem::extension(packageLocalFilePath);
            if ((!boost::iequals(extension, "zip")) && (!boost::iequals(extension, "tar.gz")))
            {
               progressCallback(false, 100.0f, "Invalid extension package: " + fileToDownload);
               return false;
            }

            //extract zip package
            progressCallback(true, 50.0f, "Extracting package " + fileToDownload);

            //pour l'instant on prend ce qu'il y a dans temp sans faire l'extraction en attendant poco
            Poco::Path extractPath(p);
            extractPath.pushDirectory("extract");
            
            std::ifstream inp(packageLocalFilePath.string().c_str(), std::ios::binary);
            
            // decompress to current working dir
            Poco::Zip::Decompress dec(inp, Poco::Path());
            // if an error happens invoke the ZipTest::onDecompressError method
            m_unzipError = false;
            dec.EError += Poco::Delegate<CYadoms, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &CYadoms::onDecompressError);
            dec.decompressAllFiles();
            dec.EError -= Poco::Delegate<CYadoms, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &CYadoms::onDecompressError);

            if (m_unzipError)
            {
               progressCallback(false, 100.0f, "Fail to uncompress pakcgae");
               return false;
            }

            //run updater
            progressCallback(true, 90.0f, "Running updater");

            //attente de poco

            //exit yadoms
            progressCallback(true, 90.0f, "Exiting Yadoms");

            //demande de fermeture de l'application
         }

         return true;
      }
      catch (std::exception & e)
      {
         YADOMS_LOG(error) << "Error getting lastversion file in yadoms update task " << e.what();
      }

      return false;
   }

   void CYadoms::onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
   {
      m_unzipError = true;
      m_unzipErrorMessage = info.first.getFileName() + " : " + info.second;
   }


} //namespace update
} //namespace task
