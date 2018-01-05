#pragma once
#include "WorkerTools.h"
#include "IUpdateChecker.h"
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
                             const std::string& downloadUrl,
                             boost::shared_ptr<IUpdateChecker> updateChecker);

         //---------------------------------------------
         ///\brief                        Update a widget
         //---------------------------------------------
         static void update(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& widgetName,
                            const std::string& downloadUrl,
                            boost::shared_ptr<IUpdateChecker> updateChecker);

         //---------------------------------------------
         ///\brief                        Remove a widget
         //---------------------------------------------
         static void remove(CWorkerTools::WorkerProgressFunc progressCallback,
                            const std::string& widgetName,
                            boost::shared_ptr<IUpdateChecker> updateChecker);

         //---------------------------------------------
         ///\brief                        Get installed widgets
         //---------------------------------------------
         typedef std::map<std::string, boost::shared_ptr<const IWidgetInformation>> AvailableWidgetMap;
         static AvailableWidgetMap getWidgetList();
      };
   } // namespace worker
} // namespace update
