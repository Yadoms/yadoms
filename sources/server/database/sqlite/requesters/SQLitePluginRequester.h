#pragma once

#include "../../IPluginRequester.h"



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
         virtual int addInstance(const database::entities::CPlugin& newPlugin);
         virtual void removeInstance(int pluginId);
         virtual boost::shared_ptr<database::entities::CPlugin> getInstance(int pluginId);
         virtual std::vector<boost::shared_ptr<database::entities::CPlugin> > getInstances();
         virtual void updateInstance(const database::entities::CPlugin& newPlugin);
         virtual void disableAllPluginInstances(const std::string& pluginName);
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

