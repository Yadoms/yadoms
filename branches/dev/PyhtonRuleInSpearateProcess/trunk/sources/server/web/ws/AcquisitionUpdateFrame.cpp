#include "stdafx.h"
#include "AcquisitionUpdateFrame.h"

namespace web { namespace ws {

   const std::string CAcquisitionUpdateFrame::m_acquisitionString = "acquisition";
   const std::string CAcquisitionUpdateFrame::m_acquisitionDayString = "summaryDay";
   const std::string CAcquisitionUpdateFrame::m_acquisitionHourString = "summaryHour";


   CAcquisitionUpdateFrame::CAcquisitionUpdateFrame(boost::shared_ptr<const database::entities::CAcquisition> acquisition,
      boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary,
      boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary)
      :CFrameBase(EFrameType::kAcquisitionUpdate)
   {
      shared::CDataContainer local;
      local.set(m_acquisitionString, acquisition);
      if (!!dailySummary)
         local.set(m_acquisitionDayString, dailySummary);
      if (!!hourlySummary)
         local.set(m_acquisitionHourString, hourlySummary);
      m_internalContainer.set(m_dataFieldName, local);
   }
   
   CAcquisitionUpdateFrame::~CAcquisitionUpdateFrame()
   {

   }

} //namespace ws
} //namespace web