#pragma once
#include "IWorker.h"
#include "../source/Plugin.h"

namespace update {
   namespace worker {

      class CPlugin : public IWorker
      {
      public:
         //---------------------------------------------
         ///\brief   Constructor
         ///\param [in] source               The update source
         //---------------------------------------------
         CPlugin(boost::shared_ptr<update::source::CPlugin> source);

         //---------------------------------------------
         ///\brief   Destructor
         //---------------------------------------------
         virtual ~CPlugin();

         // IWorker implementation 
         void checkForUpdateAsync(WorkerProgressFunc callback);
         void updateAsync(WorkerProgressFunc callback);
         // [END] - IWorker implementation 

      private:
         //---------------------------------------------
         ///\brief   Update source
         //---------------------------------------------
         boost::shared_ptr<update::source::CPlugin> m_source;

         //---------------------------------------------
         ///\brief   Only check or update
         //---------------------------------------------
         bool m_onlyCheckForUpdate;
      };

   } // namespace worker
} // namespace update
