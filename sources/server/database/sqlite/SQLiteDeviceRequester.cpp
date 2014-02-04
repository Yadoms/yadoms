#include "stdafx.h"
#include "SQLiteDeviceRequester.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include <shared/Exceptions/EmptyResultException.hpp>
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/SQLiteDatabaseAdapters.h"
#include "SQLiteDatabaseTables.h"
#include "Query.h"

CSQLiteDeviceRequester::CSQLiteDeviceRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
   :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
{
}

CSQLiteDeviceRequester::~CSQLiteDeviceRequester()
{
}


// IDeviceRequester implementation
int CSQLiteDeviceRequester::addDevice(boost::shared_ptr<CDevice> newDevice)
{
   CQuery qInsert;
   qInsert. InsertInto(CDeviceTable::getTableName(), CDeviceTable::getDataSourceColumnName(), CDeviceTable::getNameColumnName(), CDeviceTable::getConfigurationColumnName()).
            Values(newDevice->getDataSource(), newDevice->getName(), newDevice->getConfiguration());
   if(m_databaseRequester->queryStatement(qInsert) <= 0)
      throw CEmptyResultException("No lines affected");
      
 CQuery qSelect;
   qSelect. Select(CDeviceTable::getIdColumnName()).
            From(CDeviceTable::getTableName()).
            Where(CDeviceTable::getDataSourceColumnName(), CQUERY_OP_EQUAL, newDevice->getDataSource()).
            And(CDeviceTable::getNameColumnName(), CQUERY_OP_EQUAL, newDevice->getName()).
            And(CDeviceTable::getConfigurationColumnName(), CQUERY_OP_EQUAL, newDevice->getConfiguration()).
            OrderBy(CDeviceTable::getIdColumnName(), CQUERY_ORDER_DESC);

   CSingleValueAdapter<int> adapter;
   m_databaseRequester->queryEntities<int>(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
      throw CEmptyResultException("Cannot retrieve inserted Hardware");      
}

boost::shared_ptr<CDevice> CSQLiteDeviceRequester::getDevice(int deviceId)
{
   CQuery qSelect;
   qSelect. Select().
            From(CDeviceTable::getTableName()).
            Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

   CDeviceAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CDevice> >(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
   {
      std::string sEx = (boost::format("Cannot retrieve Device Id=%1% in database") % deviceId).str(); 
      throw CEmptyResultException(sEx);
   }
}

std::vector<boost::shared_ptr<CDevice> > CSQLiteDeviceRequester::getDevices()
{
   CQuery qSelect;
   qSelect. Select().
            From(CDeviceTable::getTableName());

   CDeviceAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CDevice> >(&adapter, qSelect);
   return adapter.getResults();
}

void CSQLiteDeviceRequester::updateDeviceConfiguration(int deviceId, const std::string& newConfiguration)
{
   CQuery qUpdate;
   qUpdate. Update(CHardwareTable::getTableName()).
            Set(CDeviceTable::getConfigurationColumnName(), newConfiguration).
            Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

   if(m_databaseRequester->queryStatement(qUpdate) <= 0)
      throw CEmptyResultException("No lines affected");
}

void CSQLiteDeviceRequester::removeDevice(int deviceId)
{
   CQuery qDelete;
   qDelete. DeleteFrom(CDeviceTable::getTableName()).
            Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);
   if(m_databaseRequester->queryStatement(qDelete) <= 0)
      throw  CEmptyResultException("No lines affected");
}








// [END] IDeviceRequester implementation




