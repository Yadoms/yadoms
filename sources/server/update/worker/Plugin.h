#pragma once
#include <Poco/Path.h>
#include "WorkerTools.h"

namespace update {
   namespace worker {

      class CPlugin
      {
      public:
         //---------------------------------------------
         ///\brief                        Install a new plugin
         ///\param [in] progressCallback  The progress callback
         ///\param [in] downloadUrl       The plugin package url
         //---------------------------------------------
         static void install(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief                        Update a plugin
         ///\param [in] progressCallback  The progress callback
         ///\param [in] pluginName        The plugin name
         ///\param [in] downloadUrl       The plugin package url
         //---------------------------------------------
         static void update(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & pluginName, const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief                        Remove a plugin
         ///\param [in] progressCallback  The progress callback
         ///\param [in] pluginName        The plugin name
         //---------------------------------------------
         static void remove(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & pluginName);
      };

   } // namespace worker
} // namespace update
