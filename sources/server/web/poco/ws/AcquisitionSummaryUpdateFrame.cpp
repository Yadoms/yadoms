#include "stdafx.h"
#include "AcquisitionSummaryUpdateFrame.h"

namespace web
{
   namespace poco
   {
      namespace ws
      {
         CAcquisitionSummaryUpdateFrame::CAcquisitionSummaryUpdateFrame(
            const std::vector<boost::shared_ptr<database::entities::CAcquisitionSummary>>& acquisitionSummaries)
            : CFrameBase(EFrameType::kAcquisitionSummaryUpdate)
         {
            m_internalContainer.set(DataFieldName, acquisitionSummaries);
         }

         CAcquisitionSummaryUpdateFrame::~CAcquisitionSummaryUpdateFrame()
         {
         }
      } //namespace ws
   } //namespace poco
} //namespace web
