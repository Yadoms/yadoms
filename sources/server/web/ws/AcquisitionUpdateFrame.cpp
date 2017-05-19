#include "stdafx.h"
#include "AcquisitionUpdateFrame.h"

namespace web
{
   namespace ws
   {
      CAcquisitionUpdateFrame::CAcquisitionUpdateFrame(boost::shared_ptr<const database::entities::CAcquisition> acquisition)
         : CFrameBase(EFrameType::kAcquisitionUpdate)
      {
         m_internalContainer.set(m_dataFieldName, acquisition);
      }

      CAcquisitionUpdateFrame::~CAcquisitionUpdateFrame()
      {
      }
   } //namespace ws
} //namespace web


