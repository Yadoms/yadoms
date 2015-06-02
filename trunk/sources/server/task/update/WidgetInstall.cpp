#include "stdafx.h"
#include "WidgetInstall.h"
#include <shared/Log.h>
#include "update/worker/Widget.h"

namespace task { namespace update {

   std::string CWidgetInstall::m_taskName = "widget.install";

   CWidgetInstall::CWidgetInstall(const std::string & downloadUrl)
      :m_downloadUrl(downloadUrl)
   {
   }

   CWidgetInstall::~CWidgetInstall()
   {
   }

   const std::string & CWidgetInstall::getName()
   {
      return m_taskName;
   }

   bool CWidgetInstall::doWork(TaskProgressFunc pFunctor)
   {

      YADOMS_LOG(information) << "Start installing widget...";

      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CWidget::WorkerProgressFunc adapter = boost::bind(pFunctor, _1, _2, _3, _4);

      ::update::worker::CWidget worker(adapter);
      worker.install(m_downloadUrl);

      YADOMS_LOG(information) << "End of installing widget";
      return true;
   }

} //namespace update
} //namespace task
