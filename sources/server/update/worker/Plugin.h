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
         //---------------------------------------------
         static void install(CWorkerTools::WorkerProgressFunc progressCallback,
                             const std::string& downloadUrl,
                             boost::shared_ptr<pluginSystem::CManager> pluginManager);

         //---------------------------------------------
         ///\brief                        Update a plugin
         //---------------------------------------------
         static void update(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& pluginName,
                            const std::string& downloadUrl,
                            boost::shared_ptr<pluginSystem::CManager> pluginManager);

         //---------------------------------------------
         ///\brief                        Remove a plugin
         //---------------------------------------------
         static void remove(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& pluginName,
                            boost::shared_ptr<pluginSystem::CManager> pluginManager);
      };
   } // namespace worker
} // namespace update
