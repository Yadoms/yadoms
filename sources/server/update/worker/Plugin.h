#pragma once
#include "WorkerHelpers.h"
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
         //---------------------------------------------
         static void install(CWorkerHelpers::WorkerProgressFunc progressCallback,
                             const std::string& downloadUrl,
                             boost::shared_ptr<pluginSystem::CManager> pluginManager,
                             const boost::filesystem::path& pluginsPath);

         //---------------------------------------------
         ///\brief                        Update a plugin
         //---------------------------------------------
         static void update(CWorkerHelpers::WorkerProgressFunc progressCallback,
                            const std::string& pluginName,
                            const std::string& downloadUrl,
                            boost::shared_ptr<pluginSystem::CManager> pluginManager,
                            const boost::filesystem::path& pluginsPath);

         //---------------------------------------------
         ///\brief                        Remove a plugin
         //---------------------------------------------
         static void remove(CWorkerHelpers::WorkerProgressFunc progressCallback,
                            const std::string& pluginName,
                            boost::shared_ptr<pluginSystem::CManager> pluginManager,
                            const boost::filesystem::path& pluginsPath);
      };
   } // namespace worker
} // namespace update
