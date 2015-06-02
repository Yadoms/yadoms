#pragma once
#include <Poco/Path.h>
#include <shared/DataContainer.h>

namespace update {
   namespace worker {

      class CPlugin
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
         CPlugin(WorkerProgressFunc progressCallback);

         //---------------------------------------------
         ///\brief   Destructor
         //---------------------------------------------
         virtual ~CPlugin();

         
         //---------------------------------------------
         ///\brief   Install a new plugin
         ///\param [in] downloadUrl The plugin package url
         //---------------------------------------------
         void install(const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Update a plugin
         ///\param [in] pluginName  The plugin name
         ///\param [in] downloadUrl The plugin package url
         //---------------------------------------------
         void update(const std::string & pluginName, const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Remove a plugin
         ///\param [in] pluginName  The plugin name
         //---------------------------------------------
         void remove(const std::string & pluginName);

      private:
         //---------------------------------------------
         ///\brief   The progress callback
         //---------------------------------------------
         WorkerProgressFunc m_progressCallback;
      };

   } // namespace worker
} // namespace update
