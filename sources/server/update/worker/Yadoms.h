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
         void onDownloadReportProgress(const std::string & filename, float progression);
         Poco::Path getTemporaryFolder();
         
         void step4RunUpdaterProcess(Poco::Path & extractedPackageLocation, const std::string & commandtoRun, boost::shared_ptr<IRunningInformation> & runningInfo);

         //---------------------------------------------
         ///\brief   Update source
         //---------------------------------------------
         boost::shared_ptr<source::CYadoms> m_source;
      };

   } // namespace worker
} // namespace update
