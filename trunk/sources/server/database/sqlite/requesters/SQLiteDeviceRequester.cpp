#include "stdafx.h"
#include "SQLiteDeviceRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/adapters/MultipleValueAdapter.hpp"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


namespace database {  namespace sqlite { namespace requesters { 

   CSQLiteDeviceRequester::CSQLiteDeviceRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CSQLiteDeviceRequester::~CSQLiteDeviceRequester()
   {
   }



   boost::shared_ptr<database::entities::CDevice> CSQLiteDeviceRequester::getDevice(int deviceId)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

      database::sqlite::adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CDevice> >(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
      {
         std::string sEx = (boost::format("Cannot retrieve Device Id=%1% in database") % deviceId).str(); 
         throw shared::exception::CEmptyResult(sEx);
      }
   }

   boost::shared_ptr<entities::CDevice> CSQLiteDeviceRequester::getDevice(const int pluginId, const std::string & name)
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
      return boost::shared_ptr<entities::CDevice>();
   }

   boost::shared_ptr<entities::CDevice> CSQLiteDeviceRequester::createDevice(int pluginId, const std::string & name, const std::string & friendlyName)
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
         boost::shared_ptr<entities::CDevice> deviceCreated = getDevice(pluginId, name);
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


   std::vector<boost::shared_ptr<database::entities::CDevice> > CSQLiteDeviceRequester::getDevices()
   {
      CQuery qSelect;
      qSelect. Select().
         From(CDeviceTable::getTableName());

      database::sqlite::adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CDevice> >(&adapter, qSelect);
      return adapter.getResults();
   }



   std::vector< boost::tuple<boost::posix_time::ptime, std::string, std::string>  > CSQLiteDeviceRequester::getDeviceLastData(int deviceId)
   {
      CQuery qSelect;
      qSelect. Select(CMessageTable::getDateColumnName(), CMessageContentTable::getKeyColumnName(), CMessageContentTable::getValueColumnName()).
         From(CMessageTable::getTableName(), CMessageContentTable::getTableName()).
         Where(CMessageTable::getTableName() + "." + CMessageTable::getIdColumnName(), CQUERY_OP_EQUAL, CQueryValue(CMessageContentTable::getTableName() + "." + CMessageContentTable::getIdMessageColumnName(), false)).
         And(CMessageTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
         GroupBy(CMessageContentTable::getKeyColumnName());

      database::sqlite::adapters::CMultipleValueAdapter<boost::posix_time::ptime, std::string, std::string> mva;

      m_databaseRequester->queryEntities(&mva, qSelect);

      return mva.getResults();
   }


   std::vector< boost::tuple<boost::posix_time::ptime, std::string>  >  CSQLiteDeviceRequester::getDeviceData(int deviceId, const std::string & keyword,  boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo )
   {
      CQuery qSelect;
      qSelect. Select(CMessageTable::getDateColumnName(),CMessageContentTable::getValueColumnName()).
         From(CMessageTable::getTableName(), CMessageContentTable::getTableName()).
         Where(CMessageTable::getTableName() + "." + CMessageTable::getIdColumnName(), CQUERY_OP_EQUAL, CQueryValue(CMessageContentTable::getTableName() + "." + CMessageContentTable::getIdMessageColumnName(), false)).
         And(CMessageContentTable::getTableName() + "." + CMessageContentTable::getKeyColumnName(), CQUERY_OP_EQUAL, keyword).
         And(CMessageTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId);

      if(!timeFrom.is_not_a_date_time())
      {
         qSelect.And(CMessageTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom);
         if(!timeTo.is_not_a_date_time())
         {
            qSelect.And(CMessageTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, timeTo);
         }
      }

      qSelect.OrderBy(CMessageTable::getDateColumnName());

      database::sqlite::adapters::CMultipleValueAdapter<boost::posix_time::ptime, std::string> mva;
      m_databaseRequester->queryEntities(&mva, qSelect);
      return mva.getResults();
   }


   void CSQLiteDeviceRequester::removeDevice(int deviceId)
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CDeviceTable::getTableName()).
         Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw  shared::exception::CEmptyResult("No lines affected");
   }
   // [END] IDeviceRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 


