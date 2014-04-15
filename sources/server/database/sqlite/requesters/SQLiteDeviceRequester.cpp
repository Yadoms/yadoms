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

   boost::shared_ptr<entities::CDevice> CSQLiteDeviceRequester::getDevice(const std::string & address, const std::string & protocol, const std::string & hardwareIdentifier)
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
      return boost::shared_ptr<entities::CDevice>();
   }

   boost::shared_ptr<entities::CDevice> CSQLiteDeviceRequester::createDevice(const std::string & address, const std::string & protocol, const std::string & hardwareIdentifier, const std::string & name /*= shared::CStringExtension::EmptyString*/)
   {
      if(getDevice(address, protocol, hardwareIdentifier))
      {
         throw shared::exception::CEmptyResult("The device already exists, cannot create it a new time");
      }
      else
      {
         //device not found, creation is enabled

         //get a good name
         std::string realName = name;
         if(realName == shared::CStringExtension::EmptyString)
            realName = address;

         //insert in db
         CQuery qInsert;
         qInsert. InsertInto(CDeviceTable::getTableName(), CDeviceTable::getAddressColumnName(), CDeviceTable::getProtocolColumnName(), CDeviceTable::getNameColumnName(), CDeviceTable::getHardwareIdentifierColumnName()).
            Values(address, protocol, realName, hardwareIdentifier);
         if(m_databaseRequester->queryStatement(qInsert) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert new device");

         //device is created, just find it in table and return entity
         boost::shared_ptr<entities::CDevice> deviceCreated = getDevice(address, protocol, hardwareIdentifier);
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

   /*
   SELECT date, key, value
   from Message, MessageContent
   where MessageContent.idMessage = Message.id
   and deviceId = 2
   group by key
   */

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


   std::vector< std::string > CSQLiteDeviceRequester::getDeviceHardwares()
   {
      //sous requetes qui filtre les deviceID
      CQuery qSelect;
      qSelect.Select(CQUERY_DISTINCT(CDeviceTable::getHardwareIdentifierColumnName())).
         From(CDeviceTable::getTableName()).
         OrderBy(CDeviceTable::getHardwareIdentifierColumnName());

      database::sqlite::adapters::CSingleValueAdapter<std::string> sva;
      m_databaseRequester->queryEntities(&sva, qSelect);
      return sva.getResults();
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


