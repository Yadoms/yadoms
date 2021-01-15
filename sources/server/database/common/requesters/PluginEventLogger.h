#pragma once

#include "database/IPluginEventLoggerRequester.h"
#include "database/IDatabaseRequester.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   PluginEventLogger requester for SQLite database
         //--------------------------------------------------------------
         class CPluginEventLogger : public IPluginEventLoggerRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	databaseRequester: the database requester
            //--------------------------------------------------------------
            explicit CPluginEventLogger(boost::shared_ptr<IDatabaseRequester> databaseRequester);

            virtual ~CPluginEventLogger() = default;

            // IPluginEventLoggerRequester implementation
            int addEvent(const std::string& pluginName,
                         const std::string& pluginVersion,
                         const entities::EEventType& eventType,
                         const std::string& message = std::string()) override;
            int addEvent(const entities::CPluginEventLogger& pluginLogEntry) override;
            std::vector<boost::shared_ptr<entities::CPluginEventLogger>> getPluginEvents(const std::string& pluginName,
                                                                                         const boost::posix_time::ptime& fromDate) override;
            // [END] IPluginEventLoggerRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 
