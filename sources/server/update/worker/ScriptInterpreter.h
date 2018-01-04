#pragma once
#include "WorkerTools.h"
#include "IUpdateChecker.h"

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
                             const std::string& downloadUrl,
                             boost::shared_ptr<IUpdateChecker> updateChecker);

         //---------------------------------------------
         ///\brief                              Update a scriptInterpreter
         //---------------------------------------------
         static void update(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& scriptInterpreterName,
                            const std::string& downloadUrl,
                            boost::shared_ptr<IUpdateChecker> updateChecker);

         //---------------------------------------------
         ///\brief                              Remove a scriptInterpreter
         //---------------------------------------------
         static void remove(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& scriptInterpreterName,
                            boost::shared_ptr<IUpdateChecker> updateChecker);
      };
   } // namespace worker
} // namespace update
