#pragma once
#include "IWorker.h"
#include "../source/Widget.h"

namespace update {
   namespace worker {

      class CWidget : public IWorker
      {
      public:
         //---------------------------------------------
         ///\brief   Constructor
         ///\param [in] source               The update source
         //---------------------------------------------
         CWidget(boost::shared_ptr<update::source::CWidget> source);

         //---------------------------------------------
         ///\brief   Destructor
         //---------------------------------------------
         virtual ~CWidget();

         // IWorker implementation 
         void checkForUpdateAsync(WorkerProgressFunc callback);
         void updateAsync(WorkerProgressFunc callback);
         // [END] - IWorker implementation 

      private:
         //---------------------------------------------
         ///\brief   Update source
         //---------------------------------------------
         boost::shared_ptr<update::source::CWidget> m_source;
      };

   } // namespace worker
} // namespace update
