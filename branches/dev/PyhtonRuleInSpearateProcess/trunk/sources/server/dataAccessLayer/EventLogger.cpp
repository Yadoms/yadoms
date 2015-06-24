#include "stdafx.h"
#include "EventLogger.h"
#include "notification/Helpers.hpp"
#include <shared/exception/EmptyResult.hpp>

namespace dataAccessLayer
{

   CEventLogger::CEventLogger(boost::shared_ptr<database::IEventLoggerRequester> requester)
      :m_requester(requester)
   {
   }

   CEventLogger::~CEventLogger()
   {
   }

   int CEventLogger::addEvent(const database::entities::ESystemEventCode& eventType, const std::string& who, const std::string& what)
   {
      int eventId = m_requester->addEvent(eventType, who, what);
      
      // Post notification
      notification::CHelpers::postNotification(m_requester->getEvent(eventId));

      return eventId;
   }

   int CEventLogger::addEvent(const database::entities::CEventLogger& logEntry)
   {
      if(!logEntry.Code.isDefined())
         throw shared::exception::CEmptyResult("The event code must be filled");      

      return addEvent(logEntry.Code(), logEntry.Who(), logEntry.What());
   }
   
   std::vector<boost::shared_ptr<database::entities::CEventLogger> > CEventLogger::getEvents()
   {
      return m_requester->getEvents();
   }

   boost::shared_ptr<database::entities::CEventLogger> CEventLogger::getEvent(const int eventId)
   {
      return m_requester->getEvent(eventId); 
   }

   boost::shared_ptr<database::entities::CEventLogger> CEventLogger::getLastEvent()
   {
      return m_requester->getLastEvent(); 
   }

   std::vector<boost::shared_ptr<database::entities::CEventLogger> > CEventLogger::getEventsFrom(const int eventId)
   {
      return m_requester->getEventsFrom(eventId);
   }

   std::vector<boost::shared_ptr<database::entities::CEventLogger> > CEventLogger::getEventsRange(const int offset, const int count)
   {
      return m_requester->getEventsRange(offset, count);
   }

} //namespace dataAccessLayer 
