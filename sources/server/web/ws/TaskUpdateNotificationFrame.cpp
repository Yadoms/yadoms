#include "stdafx.h"
#include "TaskUpdateNotificationFrame.h"

namespace web { namespace ws {

   const std::string CTaskUpdateNotificationFrame::m_taskName = "name";
   const std::string CTaskUpdateNotificationFrame::m_taskGuid = "uuid";
   const std::string CTaskUpdateNotificationFrame::m_taskState = "state";
   const std::string CTaskUpdateNotificationFrame::m_taskProgression = "progression";
   const std::string CTaskUpdateNotificationFrame::m_taskMessage = "message";
   

      
   CTaskUpdateNotificationFrame::CTaskUpdateNotificationFrame(boost::shared_ptr<notifications::CTaskProgressionNotification> notificationData)
      :CFrameBase(CFrameBase::EFrameType::kTaskUpdateNotification)
   {
      if (notificationData && notificationData->getTaskInstance())
      {
         m_internalContainer.set(m_taskName, notificationData->getTaskInstance()->getName());
         m_internalContainer.set(m_taskGuid, notificationData->getTaskInstance()->getGuid());
         m_internalContainer.set(m_taskState, notificationData->getTaskInstance()->getStatus());
         if (notificationData->getTaskInstance()->getProgression() != NULL)
            m_internalContainer.set(m_taskProgression, notificationData->getTaskInstance()->getProgression().value());
         m_internalContainer.set(m_taskMessage, notificationData->getTaskInstance()->getMessage());
      }
   }
   
   CTaskUpdateNotificationFrame::~CTaskUpdateNotificationFrame()
   {

   }

} //namespace ws
} //namespace web