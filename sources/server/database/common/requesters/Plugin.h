#pragma once

#include "database/IPluginRequester.h"
#include "database/IDatabaseRequester.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   Plugin requester for SQLite database
         //--------------------------------------------------------------
         class CPlugin : public IPluginRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	pDatabaseHandler: the database handler
            //--------------------------------------------------------------
            explicit CPlugin(boost::shared_ptr<IDatabaseRequester> databaseRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CPlugin();

            // IPluginRequester implementation
            int addInstance(const entities::CPlugin& newPlugin) override;
            void removeInstance(int pluginId) override;
            boost::shared_ptr<entities::CPlugin> getInstance(int pluginId) override;
            boost::shared_ptr<entities::CPlugin> getSystemInstance() override;
            std::vector<boost::shared_ptr<entities::CPlugin>> getInstances() override;
            void updateInstance(const entities::CPlugin& newPlugin) override;
            void disableAutoStartForAllPluginInstances(const std::string& pluginName) override;
            // [END] IPluginRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 


