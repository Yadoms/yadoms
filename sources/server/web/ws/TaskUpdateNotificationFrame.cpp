#include "stdafx.h"
#include "TaskUpdateNotificationFrame.h"

namespace web
{
   namespace ws
   {
      const std::string CTaskUpdateNotificationFrame::m_taskName = "name";
      const std::string CTaskUpdateNotificationFrame::m_taskGuid = "uuid";
      const std::string CTaskUpdateNotificationFrame::m_taskState = "taskState";
      const std::string CTaskUpdateNotificationFrame::m_taskProgression = "progression";
      const std::string CTaskUpdateNotificationFrame::m_taskMessage = "message";
      const std::string CTaskUpdateNotificationFrame::m_taskException = "exception";
      const std::string CTaskUpdateNotificationFrame::m_taskData = "data";


      CTaskUpdateNotificationFrame::CTaskUpdateNotificationFrame(boost::shared_ptr<task::CInstanceNotificationData> taskProgression)
         : CFrameBase(EFrameType::kTaskUpdateNotification)
      {
         m_internalContainer.set(m_taskName, taskProgression->getName());
         m_internalContainer.set(m_taskGuid, taskProgression->getGuid());
         m_internalContainer.set(m_taskState, taskProgression->getStatus());
         if (taskProgression->getProgression())
            m_internalContainer.set(m_taskProgression, *taskProgression->getProgression());
         m_internalContainer.set(m_taskMessage, taskProgression->getMessage());
         m_internalContainer.set(m_taskException, taskProgression->getExceptionMessage());
         m_internalContainer.set(m_taskData, taskProgression->getTaskData());
      }

      CTaskUpdateNotificationFrame::~CTaskUpdateNotificationFrame()
      {
      }
   } //namespace ws
} //namespace web


