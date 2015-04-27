#pragma once
#include "IWorker.h"
#include "../source/Yadoms.h"
#include "IRunningInformation.h"
#include <shared/DataContainer.h>
#include <Poco/Zip/ZipLocalFileHeader.h>
#include <Poco/URI.h>

namespace update {
   namespace worker {

      //---------------------------------------------
      ///\brief   Class which checkForUpdate or Update Yadoms software
      //---------------------------------------------
      class CYadoms : public IWorker
      {
      public:
         //---------------------------------------------
         ///\brief   Constructor
         ///\param [in] source               The update source
         //---------------------------------------------
         CYadoms(boost::shared_ptr<update::source::CYadoms> source);

         //---------------------------------------------
         ///\brief   Destructor
         //---------------------------------------------
         virtual ~CYadoms();

         // IWorker implementation 
         void checkForUpdateAsync(WorkerProgressFunc callback);
         void updateAsync(WorkerProgressFunc callback);
         // [END] - IWorker implementation 
         

      private:
         const std::string getPlatformFolder(boost::shared_ptr<IRunningInformation> & runningInfo);
         void onDownloadReportProgress(const std::string & filename, float progression);
         Poco::Path getTemporaryFolder();

         shared::CDataContainer step1DownloadLastVersionInfo(const std::string & platform);

         Poco::Path step2DownloadPackage(const std::string & platform, const std::string & packageName, Poco::Path & tempFolder, const std::string & md5HashExcpected);

         Poco::Path step3ExtractPackage(Poco::Path & downloadedPackage);

         void step4RunUpdaterProcess(Poco::Path & extractedPackageLocation, const std::string & commandtoRun, boost::shared_ptr<IRunningInformation> & runningInfo);


         //------------------------------------------
         ///\brief   A method for receivving unzip errors
         //------------------------------------------
         void onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info);

         //------------------------------------------
         ///\brief   Indicate if an error occured during package extraction
         //------------------------------------------
         bool m_unzipError;

         //------------------------------------------
         ///\brief   Indicate the error message if ab error occured during package extraction
         //------------------------------------------
         std::string m_unzipErrorMessage;

         //------------------------------------------
         ///\brief   The base URL for downloading files
         //------------------------------------------
         static Poco::URI m_baseDownloadUri;


         //---------------------------------------------
         ///\brief   Update source
         //---------------------------------------------
         boost::shared_ptr<source::CYadoms> m_source;
      };

   } // namespace worker
} // namespace update
