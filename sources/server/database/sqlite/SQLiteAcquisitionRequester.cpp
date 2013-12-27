#include "stdafx.h"
#include "SQLiteAcquisitionRequester.h"
#include "tools/Exceptions/NotImplementedException.hpp"
#include "tools/Exceptions/EmptyResultException.hpp"
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
void CSQLiteAcquisitionRequester::addAcquisition(boost::shared_ptr<CAcquisition> newAcquisition)
{
   CQuery qInsert;
   qInsert. InsertInto(CAcquisitionTable::getTableName(), CAcquisitionTable::getSourceColumnName(), CAcquisitionTable::getKeywordColumnName(), CAcquisitionTable::getValueColumnName(), CAcquisitionTable::getDateColumnName()).
            Values(newAcquisition->getSource(), newAcquisition->getKeyword(), newAcquisition->getValue(), newAcquisition->getDate());
   if(m_databaseRequester->queryStatement(qInsert) <= 0)
      throw new CEmptyResultException("No lines affected");
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
      throw new CEmptyResultException("Cannot retrieve inserted Hardware");
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

void CSQLiteAcquisitionRequester::removeAcquisition(int acquisitionId)
{
   CQuery qDelete;
   qDelete. DeleteFrom(CAcquisitionTable::getTableName()).
            Where(CAcquisitionTable::getIdColumnName(), CQUERY_OP_EQUAL, acquisitionId);
   if(m_databaseRequester->queryStatement(qDelete) <= 0)
      throw new CEmptyResultException("No lines affected");
}




// [END] IAcquisitionRequester implementation




