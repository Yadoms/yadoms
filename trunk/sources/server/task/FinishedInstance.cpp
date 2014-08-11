#include "stdafx.h"
#include "ITask.h"

#include "FinishedInstance.h"

namespace task { 

   CFinishedInstance::CFinishedInstance(const std::string & guid, const std::string & name, ITask::EStatus status, boost::optional< float > progression, const std::string & message, const boost::posix_time::ptime & creationDate)
      :m_creationDate(creationDate)
   {
      m_guid = guid;
      m_name = name;
      m_status = status;
      m_progression = progression;
      m_message = message;

   }

   CFinishedInstance::~CFinishedInstance()
   {
   }

   std::string CFinishedInstance::getGuid() const
   {
      return m_guid;
   }

   std::string CFinishedInstance::getMessage() const
   {
      return m_message;
   }

   boost::optional< float > CFinishedInstance::getProgression() const
   {
      return m_progression;
   }

   ITask::EStatus CFinishedInstance::getStatus() const
   {
      return m_status;
   }

   std::string CFinishedInstance::getName() const
   {
      return m_name;
   }

   boost::posix_time::ptime CFinishedInstance::getCreationDate() const
   {
      return m_creationDate;
   }

} //namespace task
