#include "stdafx.h"
#include "AcquisitionSummaryUpdateFrame.h"

namespace web
{
   namespace ws
   {
      CAcquisitionSummaryUpdateFrame::CAcquisitionSummaryUpdateFrame(const std::vector<boost::shared_ptr<database::entities::CAcquisitionSummary>>& acquisitionSummaries)
         : CFrameBase(EFrameType::kAcquisitionSummaryUpdate)
      {
         m_internalContainer.set(m_dataFieldName, acquisitionSummaries);
      }

      CAcquisitionSummaryUpdateFrame::~CAcquisitionSummaryUpdateFrame()
      {
      }
   } //namespace ws
} //namespace web


