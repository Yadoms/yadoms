#pragma once

#include "server/database/IHardwareEventLoggerRequester.h"

namespace server { 
   namespace database { 
      namespace sqlite { 

         class CSQLiteDataProvider;
         class CSQLiteRequester;

         namespace requesters { 

            //--------------------------------------------------------------
            /// \Brief		   HardwareEventLogger requester for SQLite database
            //--------------------------------------------------------------
            class CSQLiteHardwareEventLoggerRequester: public IHardwareEventLoggerRequester
            {
            public:
               //--------------------------------------------------------------
               /// \Brief		   Constructor
               /// \param [in]	pDatabaseHandler: the database handler
               //--------------------------------------------------------------
               CSQLiteHardwareEventLoggerRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

               //--------------------------------------------------------------
               /// \Brief		   Destructor
               //--------------------------------------------------------------
               virtual ~CSQLiteHardwareEventLoggerRequester();

               // IHardwareEventLoggerRequester implementation
               virtual int addEvent(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType pluginReleaseType, const server::database::entities::CHardwareEventLogger::EEventType eventType, const std::string & message = CStringExtension::EmptyString);
               virtual int addEvent(const server::database::entities::CHardwareEventLogger & hardwareLogEntry);
               virtual std::vector<boost::shared_ptr<server::database::entities::CHardwareEventLogger> > getHardwareEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType pluginReleaseType);
               virtual std::vector<boost::shared_ptr<server::database::entities::CHardwareEventLogger> > getHardwareEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType pluginReleaseType, const boost::posix_time::ptime & fromDate);
               // [END] IHardwareEventLoggerRequester implementation

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
} //namespace server
