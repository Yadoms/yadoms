#include "stdafx.h"
#include "WidgetRemove.h"
#include <shared/Log.h>

#include "update/worker/Widget.h"

namespace task { namespace update {

   std::string CWidgetRemove::m_taskName = "widget.remove";

   CWidgetRemove::CWidgetRemove(const std::string & widgetName)
      :m_widgetName(widgetName)
   {
   }

   CWidgetRemove::~CWidgetRemove()
   {
   }

   const std::string & CWidgetRemove::getName()
   {
      return m_taskName;
   }

   bool CWidgetRemove::doWork(TaskProgressFunc pFunctor)
   {
      YADOMS_LOG(information) << "Start removing widget...";

      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CWidget::WorkerProgressFunc adapter = boost::bind(pFunctor, _1, _2, _3, _4);

      ::update::worker::CWidget worker(adapter);
      worker.remove(m_widgetName);

      YADOMS_LOG(information) << "End of removing widget";
      return true;
   }

} //namespace update
} //namespace task
