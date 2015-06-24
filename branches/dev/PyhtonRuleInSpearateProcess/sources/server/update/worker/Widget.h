#pragma once
#include <shared/DataContainer.h>
#include "WorkerTools.h"


namespace update {
   namespace worker {

      class CWidget
      {
      public:
         //---------------------------------------------
         ///\brief                        Install a new widget
         ///\param [in] progressCallback  The progress callback
         ///\param [in] downloadUrl       The plugin package url
         //---------------------------------------------
         static void install(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief                        Update a widget
         ///\param [in] progressCallback  The progress callback
         ///\param [in] widgetName        The widget name
         ///\param [in] downloadUrl       The widget package url
         //---------------------------------------------
         static void update(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & widgetName, const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief                        Remove a widget
         ///\param [in] progressCallback  The progress callback
         ///\param [in] widgetName        The widget name
         //---------------------------------------------
         static void remove(CWorkerTools::WorkerProgressFunc progressCallback, const std::string & widgetName);
      };

   } // namespace worker
} // namespace update
