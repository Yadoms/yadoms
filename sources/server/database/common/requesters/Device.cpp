#include "stdafx.h"
#include "Device.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"
#include <shared/Log.h> //TODO virer


namespace database {  namespace common { namespace requesters { 

   CDevice::CDevice(boost::shared_ptr<IDatabaseRequester> databaseRequester)
      :m_databaseRequester(databaseRequester)
   {
   }

   CDevice::~CDevice()
   {
   }

   bool CDevice::deviceExists(const int deviceId) const
   {
      try
      {
         getDevice(deviceId);
      }
      catch (shared::exception::CEmptyResult&)
      {
         return false;
      }
      return true;
   }

   bool CDevice::deviceExists(const int pluginId, const std::string & deviceName) const
   {
      try
      {
         YADOMS_LOG(debug) << "CDevice::deviceExists : " << pluginId << ", " << deviceName;//TODO virer
         getDevice(pluginId, deviceName);
      }
      catch (shared::exception::CEmptyResult&)
      {
         YADOMS_LOG(debug) << "CDevice::deviceExists : EXCEPTION, not exist";//TODO virer
         return false;
      }
      return true;
   }

   boost::shared_ptr<entities::CDevice> CDevice::getDevice(int deviceId) const
   {
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

      adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      if(adapter.getResults().empty())
         throw shared::exception::CEmptyResult((boost::format("Cannot retrieve Device Id=%1% in database") % deviceId).str());

      return adapter.getResults()[0];
   }

   boost::shared_ptr<entities::CDevice> CDevice::getDevice(const int pluginId, const std::string & name) const
   {
      //search for such a device
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId).
         And(CDeviceTable::getNameColumnName(), CQUERY_OP_EQUAL, name);

      adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      if(adapter.getResults().empty())
         throw shared::exception::CEmptyResult((boost::format("Cannot retrieve Device Id=%1% in database") % name).str());

      return adapter.getResults()[0];
   }

   std::vector<boost::shared_ptr<entities::CDevice> > CDevice::getDevicesIdFromFriendlyName(const std::string & friendlyName) const
   {
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect.Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getFriendlyNameColumnName(), CQUERY_OP_EQUAL, friendlyName);

      adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();
   }

   std::vector<boost::shared_ptr<entities::CDevice> > CDevice::getDeviceWithCapacity(const std::string & capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode & accessMode) const
   {
      CQuery subQuery = m_databaseRequester->newQuery();
      subQuery.Select(CKeywordTable::getDeviceIdColumnName()).
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, capacityName);

      if (accessMode() == shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
      {
         //we add a constraint on accessmode
         subQuery.And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL, accessMode);
      }

      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, subQuery);

      adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();
   }

   std::vector<boost::shared_ptr<entities::CDevice> > CDevice::getDeviceWithCapacityType(const shared::plugin::yPluginApi::EKeywordAccessMode & capacityAccessMode, const shared::plugin::yPluginApi::EKeywordDataType & capacityType) const
   {
		CQuery subQuery = m_databaseRequester->newQuery();
		subQuery.Select(CKeywordTable::getDeviceIdColumnName()).
			From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getTypeColumnName(), CQUERY_OP_EQUAL, capacityType);

      if (capacityAccessMode() == shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
      {
         //we add a constraint on accessmode
         subQuery.And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL, capacityAccessMode);
      }

      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, subQuery);

      adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();
   }


   boost::shared_ptr<entities::CDevice> CDevice::createDevice(int pluginId, const std::string & name, const std::string & friendlyName, const std::string & model, const shared::CDataContainer & details)
   {
      if(deviceExists(pluginId, name))
         throw shared::exception::CEmptyResult("The device already exists, cannot create it a new time");

      //device not found, creation is enabled

      //get a good name
      std::string realFriendlyName = friendlyName;
      if(realFriendlyName == shared::CStringExtension::EmptyString)
         realFriendlyName = name;

      //insert in db
      CQuery qInsert = m_databaseRequester->newQuery();
	  qInsert.InsertInto(CDeviceTable::getTableName(), CDeviceTable::getPluginIdColumnName(), CDeviceTable::getNameColumnName(), CDeviceTable::getFriendlyNameColumnName(), CDeviceTable::getModelColumnName(), CDeviceTable::getDetailsColumnName()).
		  Values(pluginId, name, realFriendlyName, model, details.serialize());
      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("Fail to insert new device");

      //device is created, just find it in table and return entity
      boost::shared_ptr<entities::CDevice> deviceCreated = getDevice(pluginId, name);
      if(!deviceCreated)
         throw shared::exception::CEmptyResult("Fail to retrieve created device");

      return deviceCreated;
   }

   void CDevice::updateDeviceFriendlyName(int deviceId, const std::string & newFriendlyName)
   {
      if(!deviceExists(deviceId))
         throw shared::exception::CEmptyResult("The device do not exists");

      //device not found, creation is enabled

      //get a good name
      if(newFriendlyName != shared::CStringExtension::EmptyString)
      {
         //insert in db
         CQuery qUpdate = m_databaseRequester->newQuery();
         qUpdate. Update(CDeviceTable::getTableName()).
            Set(CDeviceTable::getFriendlyNameColumnName(), newFriendlyName).
            Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw shared::exception::CEmptyResult("Fail to update device friendlyName");
      }
   }

   std::vector<boost::shared_ptr<entities::CDevice> > CDevice::getDevices() const
   {
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select().
         From(CDeviceTable::getTableName());

      adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();
   }

   std::vector<boost::shared_ptr<entities::CDevice> >CDevice::getDevices(int pluginId) const
   {
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect.Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId);

      adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      return adapter.getResults();

   }

   void CDevice::removeDevice(int deviceId)
   {
      CQuery qDelete = m_databaseRequester->newQuery();
      qDelete.DeleteFrom(CDeviceTable::getTableName()).
         Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw  shared::exception::CEmptyResult("No lines affected");

      qDelete.Clear().DeleteFrom(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId);
      m_databaseRequester->queryStatement(qDelete);
   }

   void CDevice::removeAllDeviceForPlugin(int pluginId)
   {
      CQuery qSelect = m_databaseRequester->newQuery();
         qSelect.Select().
         From(CDeviceTable::getTableName()).Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId);

      adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);
      std::vector<boost::shared_ptr<entities::CDevice> > devicestoDelete = adapter.getResults();
      std::vector<boost::shared_ptr<entities::CDevice> >::iterator i;

      for (i = devicestoDelete.begin(); i != devicestoDelete.end(); ++i)
      {
         removeDevice( (*i)->Id());
      }
   }

   // [END] IDeviceRequester implementation

} //namespace requesters
} //namespace common
} //namespace database 


