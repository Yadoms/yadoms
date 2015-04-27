#include "stdafx.h"
#include "Widget.h"


namespace update {
   namespace worker {

      CWidget::CWidget(boost::shared_ptr<update::source::CWidget> source)
         :m_source(source)
      {

      }

      CWidget::~CWidget()
      {

      }

      void CWidget::checkForUpdateAsync(WorkerProgressFunc callback)
      {

      }

      void CWidget::updateAsync(WorkerProgressFunc callback)
      {

      }

   } // namespace worker
} // namespace update
