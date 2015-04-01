#include "stdafx.h"
#include "AcquisitionUpdateFrame.h"

namespace web { namespace ws {

   const std::string CAcquisitionUpdateFrame::m_acquisition = "acquisition";
   const std::string CAcquisitionUpdateFrame::m_acquisitionDay = "summaryDay";
   const std::string CAcquisitionUpdateFrame::m_acquisitionHour = "summaryHour";


   CAcquisitionUpdateFrame::CAcquisitionUpdateFrame(const database::entities::CAcquisition & acquisition,
      boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary,
      boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary)
      :CFrameBase(EFrameType::kAcquisitionUpdate)
   {
      shared::CDataContainer local;
      local.set(m_acquisition, acquisition);
      if (!!dailySummary)
         local.set(m_acquisitionDay, dailySummary);
      if (!!hourlySummary)
         local.set(m_acquisitionHour, hourlySummary);
      m_internalContainer.set(m_dataFieldName, local);
   }
   
   CAcquisitionUpdateFrame::~CAcquisitionUpdateFrame()
   {

   }

} //namespace ws
} //namespace web