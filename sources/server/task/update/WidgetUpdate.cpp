#include "stdafx.h"
#include "WidgetUpdate.h"
#include <shared/Log.h>
#include "update/worker/Widget.h"

namespace task { namespace update {

   std::string CWidgetUpdate::m_taskName = "widget.update";

   CWidgetUpdate::CWidgetUpdate(const std::string & widgetName, const std::string & downloadUrl)
      :m_widgetName(widgetName), m_downloadUrl(downloadUrl)
   {
   }

   CWidgetUpdate::~CWidgetUpdate()
   {
   }

   const std::string & CWidgetUpdate::getName()
   {
      return m_taskName;
   }

   bool CWidgetUpdate::doWork(TaskProgressFunc pFunctor)
   {
      YADOMS_LOG(information) << "Start updating widget...";

      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CWidget::WorkerProgressFunc adapter = boost::bind(pFunctor, _1, _2, _3, _4);

      ::update::worker::CWidget worker(adapter);
      worker.update(m_widgetName, m_downloadUrl);

      YADOMS_LOG(information) << "End of updating widget";
      return true;
   }

} //namespace update
} //namespace task
