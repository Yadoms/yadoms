#pragma once
#include <shared/DataContainer.h>
#include "WorkerTools.h"


namespace update {
   namespace worker {

      class CWidget
      {
      public:
         //---------------------------------------------
         ///\brief   Constructor
         ///\param [in] progressCallback The progress callback
         //---------------------------------------------
         CWidget(CWorkerTools::WorkerProgressFunc progressCallback);

         //---------------------------------------------
         ///\brief   Destructor
         //---------------------------------------------
         virtual ~CWidget();

         //---------------------------------------------
         ///\brief   Install a new widget
         ///\param [in] downloadUrl The plugin package url
         //---------------------------------------------
         void install(const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Update a widget
         ///\param [in] widgetName  The widget name
         ///\param [in] downloadUrl The widget package url
         //---------------------------------------------
         void update(const std::string & widgetName, const std::string & downloadUrl);

         //---------------------------------------------
         ///\brief   Remove a widget
         ///\param [in] widgetName  The widget name
         //---------------------------------------------
         void remove(const std::string & widgetName);

      private:

         //---------------------------------------------
         ///\brief   The progress callback
         //---------------------------------------------
         CWorkerTools::WorkerProgressFunc m_progressCallback;
      };

   } // namespace worker
} // namespace update
