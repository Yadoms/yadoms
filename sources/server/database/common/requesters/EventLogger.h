#pragma once

#include "database/IEventLoggerRequester.h"
#include "database/IDatabaseRequester.h"

namespace database { namespace common { namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   EventLogger requester for database
      //--------------------------------------------------------------
      class CEventLogger : public IEventLoggerRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         explicit CEventLogger(boost::shared_ptr<IDatabaseRequester> databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CEventLogger();

         // IEventLoggerRequester implementation
         virtual int addEvent(const entities::ESystemEventCode & eventType, const std::string & who, const std::string & what);
         virtual std::vector<boost::shared_ptr<entities::CEventLogger> > getEvents();
         virtual boost::shared_ptr<entities::CEventLogger> getEvent(const int eventId);
         virtual boost::shared_ptr<entities::CEventLogger> getLastEvent();
         virtual std::vector<boost::shared_ptr<entities::CEventLogger> > getEventsFrom(const int eventId);
         virtual std::vector<boost::shared_ptr<entities::CEventLogger> > getEventsRange(const int offset, const int count);
         // [END] IEventLoggerRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Reference to IDatabaseRequester
         //--------------------------------------------------------------
         boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
      };

} //namespace requesters
} //namespace common
} //namespace database 

