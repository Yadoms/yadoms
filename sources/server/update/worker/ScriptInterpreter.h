#pragma once
#include <Poco/Path.h>
#include "WorkerTools.h"

namespace update {
   namespace worker {

      class CScriptInterpreter
      {
      public:

         //---------------------------------------------
         ///\brief   Constructor
         ///\param [in] progressCallback The progress callback
         //---------------------------------------------
         CScriptInterpreter(CWorkerTools::WorkerProgressFunc progressCallback);

         //---------------------------------------------
         ///\brief   Destructor
         //---------------------------------------------
         virtual ~CScriptInterpreter();

         
         //---------------------------------------------
         ///\brief   Install a new scriptInterpreter
         ///\param [in] downloadUrl The scriptInterpreter package url
         //---------------------------------------------
         void install(const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Update a scriptInterpreter
         ///\param [in] scriptInterpreterName  The scriptInterpreter name
         ///\param [in] downloadUrl The scriptInterpreter package url
         //---------------------------------------------
         void update(const std::string & scriptInterpreterName, const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Remove a scriptInterpreter
         ///\param [in] scriptInterpreterName  The scriptInterpreter name
         //---------------------------------------------
         void remove(const std::string & scriptInterpreterName);

      private:
         //---------------------------------------------
         ///\brief   The progress callback
         //---------------------------------------------
         CWorkerTools::WorkerProgressFunc m_progressCallback;
      };

   } // namespace worker
} // namespace update
