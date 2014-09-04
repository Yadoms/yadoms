#include "stdafx.h"
#include "ITask.h"

#include "TaskEvent.h"

namespace task { 

   CTaskEvent::CTaskEvent(const std::string & guid)
      : m_guid(guid)
   {
   }

   CTaskEvent::~CTaskEvent()
   {
   }

   const std::string & CTaskEvent::getGuid()
   {
      return m_guid;
   }

} //namespace task
