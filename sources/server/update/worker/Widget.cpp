#include "stdafx.h"
#include "Widget.h"


namespace update {
   namespace worker {

      CWidget::CWidget(WorkerProgressFunc progressCallback)
         :m_progressCallback(progressCallback)
      {

      }

      CWidget::~CWidget()
      {

      }

      void CWidget::install(const std::string & downloadUrl)
      {

      }

      void CWidget::update(const std::string & widgetName, const std::string & downloadUrl)
      {

      }

      void CWidget::remove(const std::string & widgetName)
      {

      }


   } // namespace worker
} // namespace update
