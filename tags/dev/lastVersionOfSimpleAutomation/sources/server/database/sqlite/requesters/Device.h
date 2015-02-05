#pragma once

#include "server/database/IDeviceRequester.h"


namespace database {  namespace sqlite { 

   class CSQLiteDataProvider;
   class CSQLiteRequester;

   namespace requesters { 

      //--------------------------------------------------------------
      /// \Brief		   Device requester for SQLite database
      //--------------------------------------------------------------
      class CDevice: public IDeviceRequester
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         /// \param [in]	pDatabaseHandler: the database handler
         //--------------------------------------------------------------
         CDevice(boost::shared_ptr<CSQLiteRequester> & databaseRequester);

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CDevice();

         // IDeviceRequester implementation
         virtual bool deviceExists(const int deviceId) const;
         virtual bool deviceExists(const int pluginId, const std::string & deviceName) const;
         virtual boost::shared_ptr<entities::CDevice> getDevice(int deviceId) const;
         virtual boost::shared_ptr<entities::CDevice> getDevice(const int pluginId, const std::string & name) const;
         virtual boost::shared_ptr<entities::CDevice> createDevice(int pluginId, const std::string & name, const std::string & friendlyName, const std::string & model, const shared::CDataContainer & details);
         virtual std::vector<boost::shared_ptr<entities::CDevice> > getDevices() const;
         virtual std::vector<boost::shared_ptr<entities::CDevice> > getDevices(int pluginId) const;
         virtual std::vector<boost::shared_ptr<entities::CDevice> > getDeviceWithCapacity(const std::string & capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode & accessMode) const;
         virtual std::vector<boost::shared_ptr<entities::CDevice> > getDeviceWithCapacityType(const shared::plugin::yPluginApi::EKeywordAccessMode & capacityAccessMode, const shared::plugin::yPluginApi::EKeywordDataType & capacityType) const;
         virtual void updateDeviceFriendlyName(int deviceId, const std::string & newFriendlyName);
         virtual void removeDevice(int deviceId);
         virtual void removeAllDeviceForPlugin(int pluginId);
         // [END] IDeviceRequester implementation

      private:
         //--------------------------------------------------------------
         /// \Brief		   Reference to SQLiteRequester
         //--------------------------------------------------------------
         boost::shared_ptr<CSQLiteRequester> m_databaseRequester;
      };

   } //namespace requesters
} //namespace sqlite
} //namespace database 

