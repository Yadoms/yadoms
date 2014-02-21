#pragma once

#include "server/database/IHardwareRequester.h"


namespace server { 
   namespace database { 
      namespace sqlite { 

         class CSQLiteDataProvider;
         class CSQLiteRequester;

         namespace requesters { 

            //--------------------------------------------------------------
            /// \Brief		   Hardware requester for SQLite database
            //--------------------------------------------------------------
            class CSQLiteHardwareRequester: public IHardwareRequester
            {
            public:
               //--------------------------------------------------------------
               /// \Brief		   Constructor
               /// \param [in]	pDatabaseHandler: the database handler
               //--------------------------------------------------------------
               CSQLiteHardwareRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

               //--------------------------------------------------------------
               /// \Brief		   Destructor
               //--------------------------------------------------------------
               virtual ~CSQLiteHardwareRequester();

               // IHardwareRequester implementation
               virtual int addHardware(boost::shared_ptr<server::database::entities::CHardware> newHardware);
               virtual void removeHardware(int hardwareId);
               virtual boost::shared_ptr<server::database::entities::CHardware> getHardware(int hardwareId);
               virtual std::vector<boost::shared_ptr<server::database::entities::CHardware> > getHardwares(bool evenDeleted);
               virtual void updateHardwareConfiguration(int hardwareId, const std::string& newConfiguration);
               virtual void enableInstance(int hardwareId, bool enable);
               virtual void disableAllPluginInstance(const std::string& pluginName);
               // [END] IHardwareRequester implementation

               //test
               std::vector<std::string> getHardwareNameList();

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
