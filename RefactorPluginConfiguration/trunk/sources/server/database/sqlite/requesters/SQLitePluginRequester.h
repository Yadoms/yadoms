#pragma once

#include "server/database/IPluginRequester.h"



namespace database { namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   Plugin requester for SQLite database
      //--------------------------------------------------------------
      class CSQLitePluginRequester: public IPluginRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         CSQLitePluginRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CSQLitePluginRequester();

         // IPluginRequester implementation
         virtual int addPlugin(boost::shared_ptr<database::entities::CPlugin> newPlugin);
         virtual void removePlugin(int pluginId);
         virtual boost::shared_ptr<database::entities::CPlugin> getPlugin(int pluginId);
         virtual std::vector<boost::shared_ptr<database::entities::CPlugin> > getPlugins(bool evenDeleted);
         virtual void updatePluginConfiguration(int pluginId, const std::string& newConfiguration);
         virtual void updatePlugin(const database::entities::CPlugin & newPlugin);
         virtual void enableInstance(int pluginId, bool enable);
         virtual void disableAllPluginInstance(const std::string& pluginName);
         // [END] IPluginRequester implementation

         //test
         std::vector<std::string> getPluginNameList();

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

