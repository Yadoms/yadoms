#pragma once
#include "WorkerTools.h"
#include "IWidgetInformation.h"


namespace update
{
   namespace worker
   {
      class CWidget
      {
      public:
         //---------------------------------------------
         ///\brief                        Install a new widget
         //---------------------------------------------
         static void install(CWorkerTools::WorkerProgressFunc progressCallback,
                             const std::string& downloadUrl);

         //---------------------------------------------
         ///\brief                        Update a widget
         //---------------------------------------------
         static void update(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& widgetName,
                            const std::string& downloadUrl);

         //---------------------------------------------
         ///\brief                        Remove a widget
         //---------------------------------------------
         static void remove(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& widgetName);

         //---------------------------------------------
         ///\brief                        Get installed widgets
         //---------------------------------------------
         typedef std::map<std::string, boost::shared_ptr<const IWidgetInformation>> AvailableWidgetMap;
         static AvailableWidgetMap getWidgetList();
      };
   } // namespace worker
} // namespace update
