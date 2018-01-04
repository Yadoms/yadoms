#pragma once
#include "IRunningInformation.h"
#include <Poco/Path.h>
#include "WorkerTools.h"
#include "IUpdateChecker.h"

namespace update
{
   namespace worker
   {
      //---------------------------------------------
      ///\brief   Class which checkForUpdate or Update Yadoms software
      //---------------------------------------------
      class CYadoms
      {
      public:
         //---------------------------------------------
         ///\brief   Update Yadoms to another version
         ///\param [in] progressCallback The progress callback
         ///\param [in] versionInfo    The version information to use (can be upgrade or downgrade)
         //---------------------------------------------
         static void update(CWorkerTools::WorkerProgressFunc progressCallback,
                            const shared::CDataContainer& versionInfo,
                            boost::shared_ptr<IUpdateChecker> updateChecker);


      private:
         //---------------------------------------------
         ///\brief   Run the updater script
         ///\param [in] extractedPackageLocation    The location of the package extraction
         ///\param [in] commandtoRun                The script command to run
         ///\param [in] runningInfo                 The current yadoms running information
         //---------------------------------------------
         static void step4RunUpdaterProcess(Poco::Path& extractedPackageLocation,
                                            const std::string& commandtoRun,
                                            boost::shared_ptr<IRunningInformation>& runningInfo);
      };
   } // namespace worker
} // namespace update
