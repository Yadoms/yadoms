#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web { namespace rest { namespace service {

   class CEventLoggerRestService : public IRestService
   {
   public:
      CEventLoggerRestService(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CEventLoggerRestService();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();

   private:
      //-----------------------------------------
      ///\brief   get events
      //-----------------------------------------
      web::rest::json::CJson getEvents(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  

      //-----------------------------------------
      ///\brief   get the last event
      //-----------------------------------------
      web::rest::json::CJson getLastEvent(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  

      //-----------------------------------------
      ///\brief   get the events from
      //-----------------------------------------
      web::rest::json::CJson getEventsFrom(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  

      //-----------------------------------------
      ///\brief   get the events in a range
      //-----------------------------------------
      web::rest::json::CJson getEventsRange(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  

      //-----------------------------------------
      ///\brief   create an event entry
      //-----------------------------------------
      web::rest::json::CJson addEvent(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent);  

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 
