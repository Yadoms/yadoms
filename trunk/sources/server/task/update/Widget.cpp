#include "stdafx.h"
#include <shared/Log.h>
#include "task/ITask.h"
#include "Widget.h"

namespace task { namespace update {

   std::string CWidget::m_taskName = "system.widgetUpdate";

   CWidget::CWidget(boost::shared_ptr<::update::source::CWidget> updateSource, bool onlyCheckForUpdate)
      :m_updateSource(updateSource), m_onlyCheckForUpdate(onlyCheckForUpdate)
   {
   }

   CWidget::~CWidget()
   {
   }

   const std::string & CWidget::getName()
   {
      return m_taskName;
   }

   bool CWidget::doWork(TaskProgressFunc pFunctor)
   {

      YADOMS_LOG(information) << "Start updating widget...";

      for(int i=1; i<=10; ++i)
      {
         boost::this_thread::sleep(boost::posix_time::seconds(1)); 
         pFunctor(true, i*10.0f, "");
      }

      YADOMS_LOG(information) << "End of updating widget";
      return true;
   }

} //namespace update
} //namespace task
