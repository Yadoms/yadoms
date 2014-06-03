#include "stdafx.h"
#include "Device.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/adapters/MultipleValueAdapter.hpp"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


namespace database {  namespace sqlite { namespace requesters { 

   CDevice::CDevice(CSQLiteDataProvider * databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CDevice::~CDevice()
   {
   }

   boost::shared_ptr<database::entities::CDevice> CDevice::getDevice(int deviceId)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

      database::sqlite::adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CDevice> >(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
      {
         return adapter.getResults()[0];
      }
      else
      {
         std::string sEx = (boost::format("Cannot retrieve Device Id=%1% in database") % deviceId).str(); 
         throw shared::exception::CEmptyResult(sEx);
      }
   }

   boost::shared_ptr<database::entities::CDevice> CDevice::getDevice(const int pluginId, const std::string & name)
   {
      //serach for sucgh a device
      CQuery qSelect;
      qSelect. Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId).
         And(CDeviceTable::getNameColumnName(), CQUERY_OP_EQUAL, name);

      database::sqlite::adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CDevice> >(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
      {
         //the device is found, return its entity
         return adapter.getResults()[0];
      }
      return boost::shared_ptr<database::entities::CDevice>();
   }

   std::vector<boost::shared_ptr<entities::CDevice> > CDevice::getDeviceWithCapacity(const std::string & capacityName, const database::entities::ECapacityAccessMode capacityAccessMode)
   {
      CQuery subQuery;
      subQuery. Select(CKeywordTable::getDeviceIdColumnName()).
                From(CKeywordTable::getTableName()).
                Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, capacityName).
                And(CKeywordTable::getCapacityAccessModeColumnName(), CQUERY_OP_EQUAL, (int)capacityAccessMode);

      CQuery qSelect;
      qSelect. Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, subQuery);

      database::sqlite::adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CDevice> >(&adapter, qSelect);
      return adapter.getResults();
   }


   boost::shared_ptr<database::entities::CDevice> CDevice::createDevice(int pluginId, const std::string & name, const std::string & friendlyName)
   {
      if(getDevice(pluginId, name))
      {
         throw shared::exception::CEmptyResult("The device already exists, cannot create it a new time");
      }
      else
      {
         //device not found, creation is enabled

         //get a good name
         std::string realFriendlyName = friendlyName;
         if(realFriendlyName == shared::CStringExtension::EmptyString)
            realFriendlyName = name;

         //insert in db
         CQuery qInsert;
         qInsert. InsertInto(CDeviceTable::getTableName(), CDeviceTable::getPluginIdColumnName(), CDeviceTable::getNameColumnName(), CDeviceTable::getFriendlyNameColumnName()).
            Values(pluginId, name, realFriendlyName);
         if(m_databaseRequester->queryStatement(qInsert) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert new device");

         //device is created, just find it in table and return entity
         boost::shared_ptr<database::entities::CDevice> deviceCreated = getDevice(pluginId, name);
         if(deviceCreated)
         {
            return deviceCreated;
         }
         else
         {
            throw shared::exception::CEmptyResult("Fail to retreive created device");
         }
      }
   }

   void CDevice::updateDeviceFriendlyName(int deviceId, const std::string & newFriendlyName)
   {
      if(!getDevice(deviceId))
      {
         throw shared::exception::CEmptyResult("The device do not exists");
      }
      else
      {
         //device not found, creation is enabled

         //get a good name
         if(newFriendlyName != shared::CStringExtension::EmptyString)
         {
         //insert in db
         CQuery qUpdate;
         qUpdate. Update(CDeviceTable::getTableName()).
            Set(CDeviceTable::getFriendlyNameColumnName(), newFriendlyName).
            Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw shared::exception::CEmptyResult("Fail to update device friendlyName");

         }

      }   
   }


   std::vector<boost::shared_ptr<database::entities::CDevice> > CDevice::getDevices()
   {
      CQuery qSelect;
      qSelect. Select().
         From(CDeviceTable::getTableName());

      database::sqlite::adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CDevice> >(&adapter, qSelect);
      return adapter.getResults();
   }




   void CDevice::removeDevice(int deviceId)
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CDeviceTable::getTableName()).
         Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw  shared::exception::CEmptyResult("No lines affected");

      qDelete.Clear(). DeleteFrom(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId);
      m_databaseRequester->queryStatement(qDelete);
   }
   // [END] IDeviceRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 


