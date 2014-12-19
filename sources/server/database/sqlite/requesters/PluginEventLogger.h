#pragma once

#include "server/database/IPluginEventLoggerRequester.h"


namespace database { namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   PluginEventLogger requester for SQLite database
      //--------------------------------------------------------------
      class CPluginEventLogger : public IPluginEventLoggerRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         CPluginEventLogger(boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CPluginEventLogger();

         // IPluginEventLoggerRequester implementation
         virtual int addEvent(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType & pluginReleaseType, const database::entities::EEventType & eventType, const std::string & message = shared::CStringExtension::EmptyString);
         virtual int addEvent(const database::entities::CPluginEventLogger & pluginLogEntry);
         virtual std::vector<boost::shared_ptr<database::entities::CPluginEventLogger> > getPluginEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType & pluginReleaseType);
         virtual std::vector<boost::shared_ptr<database::entities::CPluginEventLogger> > getPluginEvents(const std::string & pluginName, const std::string & pluginVersion, const shared::plugin::information::EReleaseType & pluginReleaseType, const boost::posix_time::ptime & fromDate);
         // [END] IPluginEventLoggerRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Reference to SQLiteRequester
         //--------------------------------------------------------------
         boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
      };

   } //namespace requesters
} //namespace sqlite
} //namespace database 

