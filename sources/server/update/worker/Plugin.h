#pragma once
#include "WorkerTools.h"
#include "pluginSystem/Manager.h"

namespace update
{
   namespace worker
   {
      class CPlugin
      {
      public:
         //---------------------------------------------
         ///\brief                        Install a new plugin
         ///\param [in] progressCallback  The progress callback
         ///\param [in] downloadUrl       The plugin package url
         ///\param [in] pluginManager     The plugin manager
         //---------------------------------------------
         static void install(CWorkerTools::WorkerProgressFunc progressCallback,
                             const std::string& downloadUrl,
                             boost::shared_ptr<pluginSystem::CManager> pluginManager);

         //---------------------------------------------
         ///\brief                        Update a plugin
         ///\param [in] progressCallback  The progress callback
         ///\param [in] pluginName        The plugin name
         ///\param [in] downloadUrl       The plugin package url
         ///\param [in] pluginManager     The plugin manager
         //---------------------------------------------
         static void update(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& pluginName,
                            const std::string& downloadUrl,
                            boost::shared_ptr<pluginSystem::CManager> pluginManager);

         //---------------------------------------------
         ///\brief                        Remove a plugin
         ///\param [in] progressCallback  The progress callback
         ///\param [in] pluginName        The plugin name
         ///\param [in] pluginManager     The plugin manager
         //---------------------------------------------
         static void remove(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& pluginName,
                            boost::shared_ptr<pluginSystem::CManager> pluginManager);
      };
   } // namespace worker
} // namespace update


