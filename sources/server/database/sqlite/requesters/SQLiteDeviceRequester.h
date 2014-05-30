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
               virtual boost::shared_ptr<entities::CDevice> getDevice(const int pluginId, const std::string & name) ;
               virtual boost::shared_ptr<entities::CDevice> createDevice(int pluginId, const std::string & name, const std::string & friendlyName);
               virtual std::vector<boost::shared_ptr<database::entities::CDevice> > getDevices();
               virtual std::vector< boost::tuple<boost::posix_time::ptime, std::string, std::string>  > getDeviceLastData(int deviceId);
               virtual std::vector< boost::tuple<boost::posix_time::ptime, std::string>  > getDeviceData(int deviceId, const std::string & keyword,  boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo );
               virtual void removeDevice(int deviceId);
               virtual std::vector< entities::CDeviceCapacity > GetDeviceCapacities(entities::CDevice & device);
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

