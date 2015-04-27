#include "stdafx.h"
#include "Widget.h"


namespace update {
   namespace source {

      CWidget::CWidget()
      {

      }

      CWidget::~CWidget()
      {

      }

      const std::string CWidget::getUpdateLocation() const
      {
         return "widget";
      }

      const tools::CVersion CWidget::getInstalledVersion() const
      {
         return tools::CVersion();
      }

      const EUpdateType CWidget::getUpdateType() const
      {
         return EUpdateType::kWidget;
      }

   } // namespace source
} // namespace update
