#include "stdafx.h"
#include "SQLiteDeviceRequester.h"
#include "tools/Exceptions/NotImplementedException.hpp"
#include "tools/Exceptions/EmptyResultException.hpp"
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
   qInsert. insertInto(CDeviceTable::getTableName(), CDeviceTable::getDataSourceColumnName(), CDeviceTable::getNameColumnName(), CDeviceTable::getConfigurationColumnName()).
      values(newDevice->getDataSource(), newDevice->getName(), newDevice->getConfiguration());
   if(m_databaseRequester->queryStatement(qInsert) <= 0)
      throw new CEmptyResultException("No lines affected");
      
 CQuery qSelect;
   qSelect. select(CDeviceTable::getIdColumnName()).
            from(CDeviceTable::getTableName()).
            where(CDeviceTable::getDataSourceColumnName(), CQUERY_OP_EQUAL, newDevice->getDataSource()).
            and(CDeviceTable::getNameColumnName(), CQUERY_OP_EQUAL, newDevice->getName()).
            and(CDeviceTable::getConfigurationColumnName(), CQUERY_OP_EQUAL, newDevice->getConfiguration()).
            orderBy(CDeviceTable::getIdColumnName(), CQUERY_ORDER_DESC);

   CSingleValueAdapter<int> adapter;
   m_databaseRequester->queryEntities<int>(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
      throw new CEmptyResultException("Cannot retrieve inserted Hardware");      
}

boost::shared_ptr<CDevice> CSQLiteDeviceRequester::getDevice(int deviceId)
{
   CQuery qSelect;
   qSelect. select().
      from(CDeviceTable::getTableName()).
      where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

   CDeviceAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CDevice> >(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
      throw new CEmptyResultException("Cannot retrieve Device");
}

std::vector<boost::shared_ptr<CDevice> > CSQLiteDeviceRequester::getDevices()
{
   CQuery qSelect;
   qSelect. select().
      from(CDeviceTable::getTableName());

   CDeviceAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CDevice> >(&adapter, qSelect);
   return adapter.getResults();
}

void CSQLiteDeviceRequester::updateDeviceConfiguration(int deviceId, const std::string& newConfiguration)
{
   CQuery qUpdate;
   qUpdate. update(CHardwareTable::getTableName()).
            set(CDeviceTable::getConfigurationColumnName(), newConfiguration).
            where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

   if(m_databaseRequester->queryStatement(qUpdate) <= 0)
      throw new CEmptyResultException("No lines affected");
}

void CSQLiteDeviceRequester::removeDevice(int deviceId)
{
   CQuery qDelete;
   qDelete. deleteFrom(CDeviceTable::getTableName()).
            where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);
   if(m_databaseRequester->queryStatement(qDelete) <= 0)
      throw new CEmptyResultException("No lines affected");
}








// [END] IDeviceRequester implementation




