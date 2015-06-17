#pragma once
#include <Poco/Path.h>
#include "WorkerTools.h"

namespace update {
   namespace worker {

      class CScriptInterpreter
      {
      public:
         //---------------------------------------------
         ///\brief                        Install a new scriptInterpreter
         ///\param [in] progressCallback  The progress callback
         ///\param [in] downloadUrl       The scriptInterpreter package url
         //---------------------------------------------
         static void install(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief                              Update a scriptInterpreter
         ///\param [in] progressCallback        The progress callback
         ///\param [in] scriptInterpreterName   The scriptInterpreter name
         ///\param [in] downloadUrl             The scriptInterpreter package url
         //---------------------------------------------
         static void update(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & scriptInterpreterName, const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief                              Remove a scriptInterpreter
         ///\param [in] progressCallback        The progress callback
         ///\param [in] scriptInterpreterName   The scriptInterpreter name
         //---------------------------------------------
         static void remove(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & scriptInterpreterName);
      };

   } // namespace worker
} // namespace update
