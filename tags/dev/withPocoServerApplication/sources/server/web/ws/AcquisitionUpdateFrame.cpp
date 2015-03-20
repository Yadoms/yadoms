#include "stdafx.h"
#include "AcquisitionUpdateFrame.h"

namespace web { namespace ws {

   const std::string CAcquisitionUpdateFrame::m_acquition = "acquisition";
   const std::string CAcquisitionUpdateFrame::m_acquitionDay = "summaryDay";
   const std::string CAcquisitionUpdateFrame::m_acquitionHour = "summaryHour";


   CAcquisitionUpdateFrame::CAcquisitionUpdateFrame(const database::entities::CAcquisition & content)
      :CFrameBase(CFrameBase::EFrameType::kAcquisitionUpdate)
   {
      m_internalContainer.set(CFrameBase::m_dataFieldName, content);
   }
   

   CAcquisitionUpdateFrame::CAcquisitionUpdateFrame(boost::shared_ptr<notifications::CNewAcquisitionNotification> notificationData)
      :CFrameBase(CFrameBase::EFrameType::kAcquisitionUpdate)
   {
      shared::CDataContainer local;
      local.set(m_acquition, notificationData->getAcquisition());
      if (notificationData->getSummaryDay())
         local.set(m_acquitionDay, notificationData->getSummaryDay());
      if (notificationData->getSummaryHour())
         local.set(m_acquitionHour, notificationData->getSummaryHour());

      m_internalContainer.set(CFrameBase::m_dataFieldName, local);
   }
   
   CAcquisitionUpdateFrame::~CAcquisitionUpdateFrame()
   {

   }

} //namespace ws
} //namespace web