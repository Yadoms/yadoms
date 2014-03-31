#include "stdafx.h"
#include "SQLiteDeviceRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
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

   boost::shared_ptr<entities::CDevice>  CSQLiteDeviceRequester::getDevice(const std::string & address, const std::string & protocol, const std::string & hardwareIdentifier)
   {
      //serach for sucgh a device
      CQuery qSelect;
      qSelect. Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getAddressColumnName(), CQUERY_OP_EQUAL, address).
         And(CDeviceTable::getProtocolColumnName(), CQUERY_OP_EQUAL, protocol).
         And(CDeviceTable::getHardwareIdentifierColumnName(), CQUERY_OP_EQUAL, hardwareIdentifier);

      database::sqlite::adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CDevice> >(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
      {
         //the device is found, return its entity
         return adapter.getResults()[0];
      }
      else
      {
         //device not found, create it
         CQuery qInsert;
         qInsert. InsertInto(CDeviceTable::getTableName(), CDeviceTable::getAddressColumnName(), CDeviceTable::getProtocolColumnName(), CDeviceTable::getNameColumnName(), CDeviceTable::getHardwareIdentifierColumnName()).
            Values(address, protocol, address, hardwareIdentifier);
         if(m_databaseRequester->queryStatement(qInsert) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert new device");

         //device is created, just find it in table and return entity
         m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CDevice> >(&adapter, qSelect);
         if(adapter.getResults().size() >= 1)
         {
            return adapter.getResults()[0];
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


   
   std::vector<boost::shared_ptr<database::entities::CDevice> > CSQLiteDeviceRequester::getDevicesMatchingKeyword(const std::string & keyword)
   {
      //sous requetes qui filtre les deviceID
      CQuery subquery;
      subquery.Select(CQUERY_DISTINCT(CKeywordTable::getDeviceIdColumnName())).
               From(CKeywordTable::getTableName()).
               Where(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, keyword);

      //requete qui lit les deviceId filtrés
      CQuery qSelect;
      qSelect. Select().
         From(CDeviceTable::getTableName()).
         Where(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, subquery) ;
      
      database::sqlite::adapters::CDeviceAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CDevice> >(&adapter, qSelect);
      return adapter.getResults();
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


