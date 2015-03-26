#pragma once
#include "task/ITask.h"
#include "task/IUnique.h"
#include <Poco/Zip/ZipLocalFileHeader.h>
#include "IRunningInformation.h"
#include <shared/DataContainer.h>
#include <Poco/URI.h>

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Plugin update task. The aim si to update a plugin
   //-----------------------------------------
   class CYadoms : public ITask, public IUnique
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      //------------------------------------------
      CYadoms();

      //------------------------------------------
      ///\brief   Destructor
      //------------------------------------------
      virtual ~CYadoms();

   public:
      // ITask implementation
      virtual const std::string & getName();
      bool doWork(TaskProgressFunc pFunctor);
      // ITask implementation

      void onDownloadReportProgress(const std::string & filename, float progression);
      
   private:
      const std::string getPlatformFolder(boost::shared_ptr<IRunningInformation> & runningInfo);
      Poco::Path getTemporaryFolder();

      shared::CDataContainer step1DownloadLastVersionInfo(const std::string & platform);

      Poco::Path step2DownloadPackage(const std::string & platform, const std::string & packageName, Poco::Path & tempFolder, const std::string & md5HashExcpected);

      Poco::Path step3ExtractPackage(Poco::Path & downloadedPackage);
      
      void step4RunUpdaterProcess(Poco::Path & extractedPackageLocation, const std::string & commandtoRun, boost::shared_ptr<IRunningInformation> & runningInfo);

      //------------------------------------------
      ///\brief   The task name
      //------------------------------------------
      static std::string m_taskName;

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
   };

} //namespace update
} //namespace task
