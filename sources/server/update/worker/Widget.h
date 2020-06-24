#pragma once
#include "WorkerHelpers.h"
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
         static void install(CWorkerHelpers::WorkerProgressFunc progressCallback,
                             const std::string& downloadUrl,
                             const boost::filesystem::path& widgetsPath);

         //---------------------------------------------
         ///\brief                        Update a widget
         //---------------------------------------------
         static void update(CWorkerHelpers::WorkerProgressFunc progressCallback,
                            const std::string& widgetName,
                            const std::string& downloadUrl,
                            const boost::filesystem::path& widgetsPath);

         //---------------------------------------------
         ///\brief                        Remove a widget
         //---------------------------------------------
         static void remove(CWorkerHelpers::WorkerProgressFunc progressCallback,
                            const std::string& widgetName,
                            const boost::filesystem::path& widgetsPath);

         //---------------------------------------------
         ///\brief                        Get installed widgets
         //---------------------------------------------
         typedef std::map<std::string, boost::shared_ptr<const IWidgetInformation>> AvailableWidgetMap;
         static AvailableWidgetMap getWidgetList(const boost::filesystem::path& widgetsPath);
      };
   } // namespace worker
} // namespace update
