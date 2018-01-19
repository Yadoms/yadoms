#pragma once
#include "WorkerTools.h"

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
         static void install(CWorkerTools::WorkerProgressFunc progressCallback,
                             const std::string& downloadUrl);

         //---------------------------------------------
         ///\brief                              Update a scriptInterpreter
         //---------------------------------------------
         static void update(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& scriptInterpreterName,
                            const std::string& downloadUrl);

         //---------------------------------------------
         ///\brief                              Remove a scriptInterpreter
         //---------------------------------------------
         static void remove(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& scriptInterpreterName);
      };
   } // namespace worker
} // namespace update
