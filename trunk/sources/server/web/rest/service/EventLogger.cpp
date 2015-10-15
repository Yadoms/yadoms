#include "stdafx.h"
#include "EventLogger.h"
#include "web/rest/Result.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "shared/Log.h"

namespace web { namespace rest { namespace service {

   CEventLogger::CEventLogger(boost::shared_ptr<dataAccessLayer::IEventLogger> dataProvider)
      :m_dataProvider(dataProvider), m_restKeyword("EventLogger")
   {
   }


   CEventLogger::~CEventLogger()
   {
   }

   const std::string & CEventLogger::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CEventLogger::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword), CEventLogger::getEvents);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("last"), CEventLogger::getLastEvent);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("from")("*"), CEventLogger::getEventsFrom);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("limit")("*")("*"), CEventLogger::getEventsRange);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST",  (m_restKeyword), CEventLogger::addEvent);
   }

   shared::CDataContainer CEventLogger::getEvents(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         std::vector< boost::shared_ptr<database::entities::CEventLogger> > dvList = m_dataProvider->getEvents();
         shared::CDataContainer collection;
         collection.set(getRestKeyword(), dvList);
         return CResult::GenerateSuccess(collection);
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in reading events");
      }
   }





   shared::CDataContainer CEventLogger::getLastEvent(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         boost::shared_ptr<database::entities::CEventLogger> lastEvent = m_dataProvider->getLastEvent();
         return CResult::GenerateSuccess(lastEvent);
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in reading the last events");
      }
   }

   shared::CDataContainer CEventLogger::getEventsFrom(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>2)
         {
            int eventIdFrom = boost::lexical_cast<int>(parameters[2]);
            std::vector< boost::shared_ptr<database::entities::CEventLogger> > eventList = m_dataProvider->getEventsFrom(eventIdFrom);
            shared::CDataContainer collection;
            collection.set(getRestKeyword(), eventList);
            return CResult::GenerateSuccess(collection);
         }
         return CResult::GenerateError("invalid parameter. Can not retreive Id in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in reading events");
      }
   }

   shared::CDataContainer CEventLogger::getEventsRange(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>3)
         {
            int offset = boost::lexical_cast<int>(parameters[2]);
            int count = boost::lexical_cast<int>(parameters[3]);
            std::vector< boost::shared_ptr<database::entities::CEventLogger> > eventList = m_dataProvider->getEventsRange(offset, count);
            shared::CDataContainer collection;
            collection.set(getRestKeyword(), eventList);
            return CResult::GenerateSuccess(collection);
         }
         return CResult::GenerateError("invalid parameter. Can not retreive Id in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in reading events");
      }
   }






   shared::CDataContainer CEventLogger::addEvent(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         database::entities::CEventLogger entityToAdd;
         entityToAdd.fillFromContent(requestContent);
         m_dataProvider->addEvent(entityToAdd);
         return CResult::GenerateSuccess();
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in creating a new event entry");
      }
   }


} //namespace service
} //namespace rest
} //namespace web 
