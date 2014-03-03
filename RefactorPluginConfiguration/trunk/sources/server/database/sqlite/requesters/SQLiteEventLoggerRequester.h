#pragma once

#include "server/database/IEventLoggerRequester.h"




   namespace database { 
      namespace sqlite { 

         class CSQLiteDataProvider;
         class CSQLiteRequester;

         namespace requesters { 

            //--------------------------------------------------------------
            /// \Brief		   EventLogger requester for SQLite database
            //--------------------------------------------------------------
            class CSQLiteEventLoggerRequester: public IEventLoggerRequester
            {
            public:
               //--------------------------------------------------------------
               /// \Brief		   Constructor
               /// \param [in]	pDatabaseHandler: the database handler
               //--------------------------------------------------------------
               CSQLiteEventLoggerRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

               //--------------------------------------------------------------
               /// \Brief		   Destructor
               //--------------------------------------------------------------
               virtual ~CSQLiteEventLoggerRequester();

               // IEventLoggerRequester implementation
               virtual int addEvent(const std::string & eventType, const std::string & optionalData);
               virtual int addEvent(const database::entities::CEventLogger & logEntry);
               virtual std::vector<boost::shared_ptr<database::entities::CEventLogger> > getEvents(); 
               // [END] IEventLoggerRequester implementation

            private:
               //--------------------------------------------------------------
               /// \Brief		   Reference to SQLiteDatabseHandler
               //--------------------------------------------------------------
               const CSQLiteDataProvider & m_databaseHandler;

               //--------------------------------------------------------------
               /// \Brief		   Reference to SQLiteRequester
               //--------------------------------------------------------------
               boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
            };

         } //namespace requesters
      } //namespace sqlite
   } //namespace database 

