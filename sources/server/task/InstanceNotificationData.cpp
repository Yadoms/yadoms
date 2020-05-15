#include "stdafx.h"
#include "InstanceNotificationData.h"

namespace task {

   CInstanceNotificationData::CInstanceNotificationData(const IInstance & instance)
      :m_guid(instance.getGuid()),
      m_currentProgression(instance.getProgression()),
      m_currentMessage(instance.getMessage()),
      m_currentException(instance.getExceptionMessage()),
      m_currentStatus(instance.getStatus()),
      m_taskData(instance.getTaskData()),
      m_name(instance.getName()),
      m_creationDate(instance.getCreationDate())
   {
   }

   CInstanceNotificationData::~CInstanceNotificationData()
   {
   }

   std::string CInstanceNotificationData::getGuid() const
   {
      return m_guid;
   }

   boost::optional<float> CInstanceNotificationData::getProgression() const
   {
      return m_currentProgression;
   }

   std::string CInstanceNotificationData::getMessage() const
   {
      return m_currentMessage;
   }

   std::string CInstanceNotificationData::getExceptionMessage() const
   {
      return m_currentException;
   }

   ETaskStatus CInstanceNotificationData::getStatus() const
   {
      return m_currentStatus;
   }

   std::string CInstanceNotificationData::getName() const
   {
      return m_name;
   }

   boost::shared_ptr<shared::CDataContainer> CInstanceNotificationData::getTaskData() const
   {
      return m_taskData;
   }

   boost::posix_time::ptime CInstanceNotificationData::getCreationDate() const
   {
      return m_creationDate;
   }


} //namespace task
