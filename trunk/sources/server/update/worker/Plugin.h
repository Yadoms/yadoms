#pragma once
#include <Poco/Path.h>
#include "WorkerTools.h"

namespace update {
   namespace worker {

      class CPlugin
      {
      public:
         //---------------------------------------------
         ///\brief   Constructor
         ///\param [in] progressCallback The progress callback
         //---------------------------------------------
         CPlugin(CWorkerTools::WorkerProgressFunc progressCallback);

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
         CWorkerTools::WorkerProgressFunc m_progressCallback;
      };

   } // namespace worker
} // namespace update
