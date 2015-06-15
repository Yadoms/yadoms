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

   void CWidgetRemove::doWork(TaskProgressFunc pFunctor)
   {
      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CWidget worker(boost::bind(pFunctor, _1, _2, _3, _4, _5));
      worker.remove(m_widgetName);
   }

} //namespace update
} //namespace task
