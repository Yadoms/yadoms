#include "stdafx.h"
#include "EventLogger.h"
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/json/JsonResult.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "shared/Log.h"

namespace web { namespace rest { namespace service {

   CEventLogger::CEventLogger(boost::shared_ptr<database::IDataProvider> dataProvider)
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

   web::rest::json::CJson CEventLogger::getEvents(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CEventLoggerEntitySerializer hes;
         std::vector< boost::shared_ptr<database::entities::CEventLogger> > dvList = m_dataProvider->getEventLoggerRequester()->getEvents();
         return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CEventLogger>::SerializeCollection(dvList, hes, getRestKeyword()));
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in reading events");
      }
   }





   web::rest::json::CJson CEventLogger::getLastEvent(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CEventLoggerEntitySerializer hes;
         boost::shared_ptr<database::entities::CEventLogger> lastEvent = m_dataProvider->getEventLoggerRequester()->getLastEvent();
         return web::rest::json::CJsonResult::GenerateSuccess(hes.serialize(*lastEvent.get()));
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in reading the last events");
      }
   }

   web::rest::json::CJson CEventLogger::getEventsFrom(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         if(parameters.size()>2)
         {
            int eventIdFrom = boost::lexical_cast<int>(parameters[2]);
            web::rest::json::CEventLoggerEntitySerializer hes;
            std::vector< boost::shared_ptr<database::entities::CEventLogger> > eventList = m_dataProvider->getEventLoggerRequester()->getEventsFrom(eventIdFrom);
            return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CEventLogger>::SerializeCollection(eventList, hes, getRestKeyword()));
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive Id in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in reading events");
      }

   }

   web::rest::json::CJson CEventLogger::getEventsRange(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         if(parameters.size()>=3)
         {
            int offset = boost::lexical_cast<int>(parameters[2]);
            int count = boost::lexical_cast<int>(parameters[3]);
            web::rest::json::CEventLoggerEntitySerializer hes;
            std::vector< boost::shared_ptr<database::entities::CEventLogger> > eventList = m_dataProvider->getEventLoggerRequester()->getEventsRange(offset, count);
            return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CEventLogger>::SerializeCollection(eventList, hes, getRestKeyword()));
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive Id in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in reading events");
      }
   }






   web::rest::json::CJson CEventLogger::addEvent(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CEventLoggerEntitySerializer des;
         boost::shared_ptr<database::entities::CEventLogger> entityToAdd = des.deserialize(requestContent);
         m_dataProvider->getEventLoggerRequester()->addEvent(*entityToAdd.get());
         return web::rest::json::CJsonResult::GenerateSuccess();
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in creating a new event entry");
      }
   }


} //namespace service
} //namespace rest
} //namespace web 
