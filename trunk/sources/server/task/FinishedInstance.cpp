#include "stdafx.h"
#include "ITask.h"

#include "FinishedInstance.h"

namespace task { 

   CFinishedInstance::CFinishedInstance(const std::string & guid, const std::string & name, ETaskStatus status, boost::optional< float > progression, const std::string & message, const shared::CDataContainer data, const boost::posix_time::ptime & creationDate)
      : m_name(name), m_guid(guid), m_message(message), m_progression(progression), m_status(status), m_data(data),  m_creationDate(creationDate)
   {
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

   shared::CDataContainer CFinishedInstance::getTaskData() const
   {
      return m_data;
   }

   boost::optional< float > CFinishedInstance::getProgression() const
   {
      return m_progression;
   }

   ETaskStatus CFinishedInstance::getStatus() const
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
