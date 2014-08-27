#include "stdafx.h"
#include "Yadoms.h"
#include <shared/Log.h>
#include "server/tools/tools/web/FileDownloader.h"
#include <shared/DataContainer.h>
#include "tools/Version.h"
#include "System.h"
//TODO passer les includes de boost dans stdafx
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/functional/hash.hpp>
#include <shared/encryption/MD5.h>

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
      YADOMS_LOG(info) << "Downloading " << filename << ": " << progression << "%";
   }

   bool CYadoms::doWork(TaskProgressFunc pFunctor)
   {
      try
      {
         pFunctor(0.0f, "Checking for a new update");

         std::string baseUrl = "http://10.10.109.26/files/NHI/yadoms.com";

         //TODO : recuperer plateforme dynamiquement
         std::string platform = "windows";

         std::ostringstream lastVersion;
         tools::web::CFileDownloader::downloadFile(baseUrl + "/" + platform + "/lastversion.json.txt",
                                                   lastVersion, 
                                                   boost::bind(&CYadoms::onDownloadReportProgress, this, _1, _2));
         std::string data = lastVersion.str();
         shared::CDataContainer lv(data);

         std::string versionAsString = lv.get<std::string>("yadoms.information.version");
         tools::CVersion availableVersion(versionAsString);
         //tools::CVersion currentVersion = CSystem().getSoftwareVersion();
         //TODO : recuperer la version du soft en utilisant CSystem
         tools::CVersion currentVersion(1, 0, 0, 0);

         if (availableVersion <= currentVersion)
         {
            pFunctor(100.0f, "System is up to date");
         }
         else
         {
            pFunctor(0.0f, "A new update is available");
            pFunctor(0.0f, "Downloading package");
            std::string fileToDownload = lv.get<std::string>("yadoms.information.softwarePackage");
            
            //TODO : get temp folder from a global system provider
            boost::filesystem::path tempFolder("tmp/");
            //TODO : this part must be managed by the global folder before returning it
            if (!boost::filesystem::exists(tempFolder))
            {
               boost::filesystem::create_directory(tempFolder);
            }
            
            boost::filesystem::path packageLocalFilePath = tempFolder / fileToDownload;

            std::ofstream packageFile(packageLocalFilePath.string(), std::ofstream::binary);

            tools::web::CFileDownloader::downloadFile(baseUrl + "/" + platform + "/" + fileToDownload,
               packageFile,
               boost::bind(&CYadoms::onDownloadReportProgress, this, _1, _2));

            packageFile.close();

            if (!boost::filesystem::exists(packageLocalFilePath))
            {
               pFunctor(100.0f, "Error during downloading package");
               return false;
            }
            
            //we re-read the file and compute the md5 (the md5 can be generated online using ie http://onlinemd5.com/)
            CMD5 md5Hasher;
            std::string md5HashCalculated = md5Hasher.digestFile(packageLocalFilePath.string().c_str());

            YADOMS_LOG(info) << "MD5 Hash calculated: " << md5HashCalculated;
            
            std::string md5HashExcpected = lv.get<std::string>("yadoms.information.md5Hash");

            if (!boost::iequals(md5HashCalculated, md5HashExcpected))
            {
               pFunctor(100.0f, "Error during downloading package");
               return false;
            }

            pFunctor(50.0f, "Package " + fileToDownload + "successfully downloaded");
            
            //verification of the extension
            std::string extension = boost::filesystem::extension(packageLocalFilePath);
            if ((!boost::iequals(extension, "zip")) && (!boost::iequals(extension, "tar.gz")))
            {
               pFunctor(100.0f, "Invalid extension package: " + fileToDownload);
               return false;
            }

            //extract zip package
            pFunctor(50.0f, "Extracting package " + fileToDownload);

            //pour l'instant on prend ce qu'il y a dans temp sans faire l'extraction en attendant poco

            //run updater
            pFunctor(90.0f, "Running updater");

            //attente de poco

            //exit yadoms
            pFunctor(90.0f, "Exiting Yadoms");

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

} //namespace update
} //namespace task
