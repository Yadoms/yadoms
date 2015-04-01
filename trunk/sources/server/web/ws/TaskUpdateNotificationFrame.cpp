#include "stdafx.h"
#include "TaskUpdateNotificationFrame.h"

namespace web { namespace ws {

   const std::string CTaskUpdateNotificationFrame::m_taskName = "name";
   const std::string CTaskUpdateNotificationFrame::m_taskGuid = "uuid";
   const std::string CTaskUpdateNotificationFrame::m_taskState = "state";
   const std::string CTaskUpdateNotificationFrame::m_taskProgression = "progression";
   const std::string CTaskUpdateNotificationFrame::m_taskMessage = "message";
   

   CTaskUpdateNotificationFrame::CTaskUpdateNotificationFrame(const task::IInstance& taskProgression)
      :CFrameBase(EFrameType::kTaskUpdateNotification)
   {
      m_internalContainer.set(m_taskName, taskProgression.getName());
      m_internalContainer.set(m_taskGuid, taskProgression.getGuid());
      m_internalContainer.set(m_taskState, taskProgression.getStatus());
      if (taskProgression.getProgression() != NULL)
         m_internalContainer.set(m_taskProgression, taskProgression.getProgression().value());
      m_internalContainer.set(m_taskMessage, taskProgression.getMessage());
   }

   CTaskUpdateNotificationFrame::~CTaskUpdateNotificationFrame()
   {

   }

} //namespace ws
} //namespace web