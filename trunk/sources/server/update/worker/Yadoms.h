#pragma once
#include "IRunningInformation.h"
#include <shared/DataContainer.h>
#include <Poco/Zip/ZipLocalFileHeader.h>
#include <Poco/URI.h>

namespace update {
   namespace worker {

      //---------------------------------------------
      ///\brief   Class which checkForUpdate or Update Yadoms software
      //---------------------------------------------
      class CYadoms
      {
      public:
         //---------------------------------
         ///\brief Define a function prototype for updating the worker progress
         //---------------------------------
         typedef boost::function3<void, bool, boost::optional<float>, std::string > WorkerProgressFunc;

         //---------------------------------------------
         ///\brief   Constructor
         //---------------------------------------------
         CYadoms();

         //---------------------------------------------
         ///\brief   Destructor
         //---------------------------------------------
         virtual ~CYadoms();

         // IWorker implementation 
         void checkForUpdate(WorkerProgressFunc callback);
         void update(shared::CDataContainer & versionInfo, WorkerProgressFunc callback);
         // [END] - IWorker implementation 
         

      private:
        
         void step4RunUpdaterProcess(Poco::Path & extractedPackageLocation, const std::string & commandtoRun, boost::shared_ptr<IRunningInformation> & runningInfo);
      };

   } // namespace worker
} // namespace update
