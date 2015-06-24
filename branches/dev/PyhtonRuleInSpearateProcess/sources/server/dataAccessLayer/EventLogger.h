#pragma once
#include "IEventLogger.h"
#include "database/IEventLoggerRequester.h"

namespace dataAccessLayer {

   class CEventLogger : public IEventLogger
   {
   public:
      //--------------------------------------------------------------
      /// \brief                          Constructor
      /// \param [in] requester           The event logger requester
      //--------------------------------------------------------------
      CEventLogger(boost::shared_ptr<database::IEventLoggerRequester> requester);
   
      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~CEventLogger();

      // IEventLogger Implementation
      virtual int addEvent(const database::entities::ESystemEventCode& eventType, const std::string& who, const std::string& what);
      virtual int addEvent(const database::entities::CEventLogger& logEntry);
      virtual std::vector<boost::shared_ptr<database::entities::CEventLogger> > getEvents();
      virtual boost::shared_ptr<database::entities::CEventLogger> getEvent(const int eventId);
      virtual boost::shared_ptr<database::entities::CEventLogger> getLastEvent();
      virtual std::vector<boost::shared_ptr<database::entities::CEventLogger> > getEventsFrom(const int eventId);
      virtual std::vector<boost::shared_ptr<database::entities::CEventLogger> > getEventsRange(const int offset, const int count);
      // [END] IEventLogger Implementation
   
   protected:
      //--------------------------------------------------------------
      /// \brief           The requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IEventLoggerRequester> m_requester;
   };
 
} //namespace dataAccessLayer 
