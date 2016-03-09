#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "dataAccessLayer/IEventLogger.h"

namespace web { namespace rest { namespace service {

   class CEventLogger : public IRestService
   {
   public:
      explicit CEventLogger(boost::shared_ptr<dataAccessLayer::IEventLogger> dataProvider);
      virtual ~CEventLogger();

   public:
      // IRestService implementation
      virtual void configureDispatcher(CRestDispatcher & dispatcher);
      // [END] IRestService implementation

      const std::string & getRestKeyword();

   private:
      //-----------------------------------------
      ///\brief   get events
      //-----------------------------------------
      shared::CDataContainer getEvents(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  

      //-----------------------------------------
      ///\brief   get the last event
      //-----------------------------------------
      shared::CDataContainer getLastEvent(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  

      //-----------------------------------------
      ///\brief   get the events from
      //-----------------------------------------
      shared::CDataContainer getEventsFrom(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  

      //-----------------------------------------
      ///\brief   get the events in a range
      //-----------------------------------------
      shared::CDataContainer getEventsRange(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  

      //-----------------------------------------
      ///\brief   create an event entry
      //-----------------------------------------
      shared::CDataContainer addEvent(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent);  

   private:
      boost::shared_ptr<dataAccessLayer::IEventLogger> m_dataProvider;
      std::string m_restKeyword;
   };


} //namespace service
} //namespace rest
} //namespace web 
