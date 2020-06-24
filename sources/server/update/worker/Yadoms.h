#pragma once
#include "IRunningInformation.h"
#include "WorkerHelpers.h"

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
         ///\param [in] downloadUrl    The version download URL
         ///\param [in] expectedMd5Hash    The expected MD5 Hash to check package validity
         //---------------------------------------------
         static void update(const CWorkerHelpers::WorkerProgressFunc& progressCallback,
                            const std::string& downloadUrl,
                            const std::string& expectedMd5Hash);


      private:
         //---------------------------------------------
         ///\brief   Run the updater script
         ///\param [in] extractedPackageLocation    The location of the package extraction
         ///\param [in] commandToRun                The script command to run
         ///\param [in] runningInfo                 The current yadoms running information
         //---------------------------------------------
         static void step4RunUpdaterProcess(const boost::filesystem::path& extractedPackageLocation,
                                            const std::string& commandToRun,
                                            boost::shared_ptr<IRunningInformation>& runningInfo);
      };
   } // namespace worker
} // namespace update
