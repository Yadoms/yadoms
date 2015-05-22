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

      const EUpdateType CWidget::getUpdateType() const
      {
         return EUpdateType::kWidget;
      }

   } // namespace source
} // namespace update
