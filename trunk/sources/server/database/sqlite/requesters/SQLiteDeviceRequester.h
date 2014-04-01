#pragma once

#include "server/database/IDeviceRequester.h"
class CSQLiteDataProvider;
class CSQLiteRequester;


   namespace database { 
      namespace sqlite { 

         class CSQLiteDataProvider;
         class CSQLiteRequester;

         namespace requesters { 

            //--------------------------------------------------------------
            /// \Brief		   Device requester for SQLite database
            //--------------------------------------------------------------
            class CSQLiteDeviceRequester: public IDeviceRequester
            {
            public:
               //--------------------------------------------------------------
               /// \Brief		   Constructor
               /// \param [in]	pDatabaseHandler: the database handler
               //--------------------------------------------------------------
               CSQLiteDeviceRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester);

               //--------------------------------------------------------------
               /// \Brief		   Destructor
               //--------------------------------------------------------------
               virtual ~CSQLiteDeviceRequester();

               // IDeviceRequester implementation
               virtual boost::shared_ptr<database::entities::CDevice> getDevice(int deviceId);
               virtual boost::shared_ptr<entities::CDevice> getDevice(const std::string & address, const std::string & protocol, const std::string & hardwareIdentifier);
               virtual std::vector<boost::shared_ptr<database::entities::CDevice> > getDevices();
               virtual std::vector<boost::shared_ptr<database::entities::CDevice> > getDevicesMatchingKeyword(const std::string & keyword);
               virtual std::vector< boost::tuple<boost::posix_time::ptime, std::string, std::string>  > getDeviceLastData(int deviceId);
               virtual void removeDevice(int deviceId);
               // [END] IDeviceRequester implementation

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

