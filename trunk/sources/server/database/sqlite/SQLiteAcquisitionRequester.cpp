#include "stdafx.h"
#include "SQLiteAcquisitionRequester.h"
#include <shared/Exceptions/NotImplementedException.hpp>
#include <shared/Exceptions/EmptyResultException.hpp>
#include "SQLiteDataProvider.h"
#include "adapters/SingleValueAdapter.hpp"
#include "adapters/SQLiteDatabaseAdapters.h"
#include "SQLiteDatabaseTables.h"
#include "Query.h"

CSQLiteAcquisitionRequester::CSQLiteAcquisitionRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
   :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
{
}

CSQLiteAcquisitionRequester::~CSQLiteAcquisitionRequester()
{
}


// IAcquisitionRequester implementation
void CSQLiteAcquisitionRequester::addAcquisition(const CAcquisition & newAcquisition)
{
   CQuery qInsert;
   qInsert. InsertInto(CAcquisitionTable::getTableName(), CAcquisitionTable::getSourceColumnName(), CAcquisitionTable::getKeywordColumnName(), CAcquisitionTable::getValueColumnName(), CAcquisitionTable::getDateColumnName()).
            Values(newAcquisition.getSource(), newAcquisition.getKeyword(), newAcquisition.getValue(), newAcquisition.getDate());
   if(m_databaseRequester->queryStatement(qInsert) <= 0)
      throw CEmptyResultException("No lines affected");
}

boost::shared_ptr<CAcquisition> CSQLiteAcquisitionRequester::getAcquisition(int acquisitionId)
{
   CQuery qSelect;
   qSelect. Select().
            From(CAcquisitionTable::getTableName()).
            Where(CAcquisitionTable::getIdColumnName(), CQUERY_OP_EQUAL, acquisitionId);

   CAcquisitionAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CAcquisition> >(&adapter, qSelect);
   if(adapter.getResults().size() >= 1)
      return adapter.getResults()[0];
   else
   {
      std::string sEx = (boost::format("Cannot retrieve Acquisition Id=%1% in database") % acquisitionId).str(); 
      throw CEmptyResultException(sEx);
   }

}

std::vector<boost::shared_ptr<CAcquisition> > CSQLiteAcquisitionRequester::getAcquisitions(const std::string & source, const std::string & keyword)
{
   CQuery qSelect;
   qSelect. Select().
            From(CAcquisitionTable::getTableName()).
            Where(CAcquisitionTable::getSourceColumnName(), CQUERY_OP_EQUAL, source).
            And(CAcquisitionTable::getKeywordColumnName(), CQUERY_OP_EQUAL, keyword);

   CAcquisitionAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CAcquisition> >(&adapter, qSelect);
   return adapter.getResults();
}


std::vector<boost::shared_ptr<CAcquisition> > CSQLiteAcquisitionRequester::getLastAcquisitions(const std::string & source)
{
   CQuery qSelect;
   qSelect. Select().
            From(CAcquisitionTable::getTableName()).
            Where(CAcquisitionTable::getSourceColumnName(), CQUERY_OP_EQUAL, source).
            GroupBy(CAcquisitionTable::getKeywordColumnName()).
            OrderBy(CAcquisitionTable::getDateColumnName(), CQUERY_ORDER_DESC);

   CAcquisitionAdapter adapter;
   m_databaseRequester->queryEntities<boost::shared_ptr<CAcquisition> >(&adapter, qSelect);
   return adapter.getResults();
}

void CSQLiteAcquisitionRequester::removeAcquisition(int acquisitionId)
{
   CQuery qDelete;
   qDelete. DeleteFrom(CAcquisitionTable::getTableName()).
            Where(CAcquisitionTable::getIdColumnName(), CQUERY_OP_EQUAL, acquisitionId);
   if(m_databaseRequester->queryStatement(qDelete) <= 0)
      throw CEmptyResultException("No lines affected");
}




// [END] IAcquisitionRequester implementation




