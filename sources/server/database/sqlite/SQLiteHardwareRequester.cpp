#include "stdafx.h"
#include "SQLiteHardwareRequester.h"
#include "tools/Exceptions/NotImplementedException.hpp"
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/SQLiteDatabaseAdapters.h"
#include "tools/Exceptions/EmptyResultException.hpp"
#include "tools/Exceptions/InvalidParameterException.hpp"

#include "SQLiteDatabaseTables.h"
#include "Query.h"


CSQLiteHardwareRequester::CSQLiteHardwareRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
   :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
{
}

CSQLiteHardwareRequester::~CSQLiteHardwareRequester()
{
}

// IHardwareRequester implementation
int CSQLiteHardwareRequester::addHardware(boost::shared_ptr<CHardware> newHardware)
{
   CQuery qInsert;
   
   qInsert.insertInto(CHardwareTable::getTableName(), CHardwareTable::getNameColumnName(), CHardwareTable::getPluginNameColumnName(), CHardwareTable::getConfigurationColumnName(), CHardwareTable::getEnabledColumnName() ).
           values(newHardware->getName(), 
                  newHardware->getPluginName(),
                  newHardware->getConfiguration(), 
                  newHardware->getEnabled());

   if(m_databaseRequester->queryStatement(qInsert) <= 0)
      throw new CEmptyResultException("No lines affected");


   CQuery qSelect;
   qSelect. select(CHardwareTable::getIdColumnName()).
            from(CHardwareTable::getTableName()).
            where(CHardwareTable::getNameColumnName(), CQUERY_OP_EQUAL, newHardware->getName()).
            and(CHardwareTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, newHardware->getPluginName()).
            orderBy(CHardwareTable::getIdColumnName(), CQUERY_ORDER_DESC);

   CSingleValueAdapter<int> adapter;
   m_databaseRequester->queryEntities<int>(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
      throw new CEmptyResultException("Cannot retrieve inserted Hardware");
}

boost::shared_ptr<CHardware> CSQLiteHardwareRequester::getHardware(int hardwareId)
{
   CHardwareAdapter adapter;

   CQuery qSelect;

   qSelect. select().
            from(CHardwareTable::getTableName()).
            where(CHardwareTable::getIdColumnName(), CQUERY_OP_EQUAL, hardwareId).
            and(CHardwareTable::getDeletedColumnName(), CQUERY_OP_EQUAL, false);

   m_databaseRequester->queryEntities<boost::shared_ptr<CHardware> >(&adapter, qSelect);
   if (adapter.getResults().empty())
   {
      // Hardware not found
      throw CInvalidParameterException(CStringExtension::format("Hardware Id %d not found in database", hardwareId));
   }
   return adapter.getResults().at(0);
}

std::vector<boost::shared_ptr<CHardware> > CSQLiteHardwareRequester::getHardwares(bool evenDeleted)
{
   CHardwareAdapter adapter;

   CQuery qSelect;
   qSelect.select().from(CHardwareTable::getTableName());

   if (!evenDeleted)
      qSelect.where(CHardwareTable::getDeletedColumnName(), CQUERY_OP_EQUAL, false);

   m_databaseRequester->queryEntities<boost::shared_ptr<CHardware> >(&adapter, qSelect);
   return adapter.getResults();
}

//test
std::vector<std::string> CSQLiteHardwareRequester::getHardwareNameList()
{
   CSingleValueAdapter<std::string> adapter;

   CQuery qSelect;
   qSelect. select(CHardwareTable::getNameColumnName()).
            from(CHardwareTable::getTableName()).
            where(CHardwareTable::getDeletedColumnName(), CQUERY_OP_EQUAL, false).
            orderBy(CHardwareTable::getNameColumnName());

   m_databaseRequester->queryEntities<std::string>(&adapter, qSelect);
   return adapter.getResults();
}

void CSQLiteHardwareRequester::updateHardwareConfiguration(int hardwareId, const std::string& newConfiguration)
{
   CQuery qUpdate;
   qUpdate. update(CHardwareTable::getTableName()).
            set(CHardwareTable::getConfigurationColumnName(), newConfiguration).
            where(CHardwareTable::getIdColumnName(), CQUERY_OP_EQUAL, hardwareId);

   if(m_databaseRequester->queryStatement(qUpdate) <= 0)
      throw new CEmptyResultException("No lines affected");
}

void CSQLiteHardwareRequester::removeHardware(int hardwareId)
{
   CQuery qUpdate;
   qUpdate. update(CHardwareTable::getTableName()).
            set(CHardwareTable::getDeletedColumnName(), true).
            where(CHardwareTable::getIdColumnName(), CQUERY_OP_EQUAL, hardwareId);

   if(m_databaseRequester->queryStatement(qUpdate) <= 0)
      throw new CEmptyResultException("No lines affected");
}

void CSQLiteHardwareRequester::enableInstance(int hardwareId, bool enable)
{
   CQuery qUpdate;
   qUpdate. update(CHardwareTable::getTableName()).
            set(CHardwareTable::getEnabledColumnName(), enable).
            where(CHardwareTable::getIdColumnName(), CQUERY_OP_EQUAL, hardwareId);

   if(m_databaseRequester->queryStatement(qUpdate) <= 0)
      throw new CEmptyResultException("No lines affected");
}
// [END] IHardwareRequester implementation




