#pragma once
#include "WorkerHelpers.h"

namespace update
{
   namespace worker
   {
      class CScriptInterpreter
      {
      public:
         //---------------------------------------------
         ///\brief                        Install a new scriptInterpreter
         //---------------------------------------------
         static void install(CWorkerHelpers::WorkerProgressFunc progressCallback,
                             const std::string& downloadUrl,
                             const boost::filesystem::path& scriptInterpretersPath);

         //---------------------------------------------
         ///\brief                              Update a scriptInterpreter
         //---------------------------------------------
         static void update(CWorkerHelpers::WorkerProgressFunc progressCallback,
                            const std::string& scriptInterpreterName,
                            const std::string& downloadUrl,
                            const boost::filesystem::path& scriptInterpretersPath);

         //---------------------------------------------
         ///\brief                              Remove a scriptInterpreter
         //---------------------------------------------
         static void remove(CWorkerHelpers::WorkerProgressFunc progressCallback,
                            const std::string& scriptInterpreterName,
                            const boost::filesystem::path& scriptInterpretersPath);
      };
   } // namespace worker
} // namespace update
