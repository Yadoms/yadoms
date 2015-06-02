#pragma once
#include <Poco/Path.h>
#include <shared/DataContainer.h>

namespace update {
   namespace worker {

      class CScriptInterpreter
      {
      public:
         //---------------------------------
         ///\brief Define a function prototype for updating the worker progress
         //---------------------------------
         typedef boost::function4<void, bool, boost::optional<float>, std::string, shared::CDataContainer > WorkerProgressFunc;

         //---------------------------------------------
         ///\brief   Constructor
         ///\param [in] progressCallback The progress callback
         //---------------------------------------------
         CScriptInterpreter(WorkerProgressFunc progressCallback);

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
         WorkerProgressFunc m_progressCallback;
      };

   } // namespace worker
} // namespace update
