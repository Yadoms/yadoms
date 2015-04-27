#pragma once
#include "task/ITask.h"
#include "task/IUnique.h"
#include <Poco/Zip/ZipLocalFileHeader.h>
#include "IRunningInformation.h"
#include <shared/DataContainer.h>
#include <Poco/URI.h>
#include "update/source/Yadoms.h"

namespace task { namespace update {

   //------------------------------------------
   ///\brief   Plugin update task. The aim si to update a plugin
   //-----------------------------------------
   class CYadoms : public ITask, public IUnique
   {
   public:
      //------------------------------------------
      ///\brief   Constructor
      ///\param [in] updateSource   The update source
      ///\param [in] onlyCheckForUpdate   If true will perform a check for update; if false will perform a complete update
      //------------------------------------------
      CYadoms(boost::shared_ptr<::update::source::CYadoms> updateSource, bool onlyCheckForUpdate); // "::update" is needed to avoid confusion with task::update namespace

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

      //------------------------------------------
      ///\brief   The update source
      /// "::update" is needed to avoid confusion with task::update namespace
      //------------------------------------------
      boost::shared_ptr<::update::source::CYadoms> m_updateSource;

      //------------------------------------------
      ///\brief   Indicate if check for update is required (true) or a full update (false)
      //------------------------------------------
      bool m_onlyCheckForUpdate;
   };

} //namespace update
} //namespace task
