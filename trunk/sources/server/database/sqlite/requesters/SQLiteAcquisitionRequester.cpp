#include "stdafx.h"
#include "SQLiteAcquisitionRequester.h"
#include <shared/exceptions/NotImplemented.hpp>
#include <shared/exceptions/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


   namespace database { 
      namespace sqlite { 
         namespace requesters { 

            CSQLiteAcquisitionRequester::CSQLiteAcquisitionRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
               :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
            {
            }

            CSQLiteAcquisitionRequester::~CSQLiteAcquisitionRequester()
            {
            }


            // IAcquisitionRequester implementation
            void CSQLiteAcquisitionRequester::addAcquisition(const database::entities::CAcquisition & newAcquisition)
            {
               CQuery qInsert;
               qInsert. InsertInto(CAcquisitionTable::getTableName(), CAcquisitionTable::getSourceColumnName(), CAcquisitionTable::getKeywordColumnName(), CAcquisitionTable::getValueColumnName(), CAcquisitionTable::getDateColumnName()).
                  Values(newAcquisition.getSource(), newAcquisition.getKeyword(), newAcquisition.getValue(), newAcquisition.getDate());
               if(m_databaseRequester->queryStatement(qInsert) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");
            }

            boost::shared_ptr<database::entities::CAcquisition> CSQLiteAcquisitionRequester::getAcquisition(int acquisitionId)
            {
               CQuery qSelect;
               qSelect. Select().
                  From(CAcquisitionTable::getTableName()).
                  Where(CAcquisitionTable::getIdColumnName(), CQUERY_OP_EQUAL, acquisitionId);

               database::sqlite::adapters::CAcquisitionAdapter adapter;
               m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CAcquisition> >(&adapter, qSelect);
               if(adapter.getResults().size() >= 1)
                  return adapter.getResults()[0];
               else
               {
                  std::string sEx = (boost::format("Cannot retrieve Acquisition Id=%1% in database") % acquisitionId).str(); 
                  throw shared::exception::CEmptyResult(sEx);
               }

            }

            std::vector<boost::shared_ptr<database::entities::CAcquisition> > CSQLiteAcquisitionRequester::getAcquisitions(const std::string & source, const std::string & keyword)
            {
               CQuery qSelect;
               qSelect. Select().
                  From(CAcquisitionTable::getTableName()).
                  Where(CAcquisitionTable::getSourceColumnName(), CQUERY_OP_EQUAL, source).
                  And(CAcquisitionTable::getKeywordColumnName(), CQUERY_OP_EQUAL, keyword);

               database::sqlite::adapters::CAcquisitionAdapter adapter;
               m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CAcquisition> >(&adapter, qSelect);
               return adapter.getResults();
            }


            std::vector<boost::shared_ptr<database::entities::CAcquisition> > CSQLiteAcquisitionRequester::getLastAcquisitions(const std::string & source)
            {
               CQuery qSelect;
               qSelect. Select().
                  From(CAcquisitionTable::getTableName()).
                  Where(CAcquisitionTable::getSourceColumnName(), CQUERY_OP_EQUAL, source).
                  GroupBy(CAcquisitionTable::getKeywordColumnName()).
                  OrderBy(CAcquisitionTable::getDateColumnName(), CQUERY_ORDER_DESC);

               database::sqlite::adapters::CAcquisitionAdapter adapter;
               m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CAcquisition> >(&adapter, qSelect);
               return adapter.getResults();
            }

            void CSQLiteAcquisitionRequester::removeAcquisition(int acquisitionId)
            {
               CQuery qDelete;
               qDelete. DeleteFrom(CAcquisitionTable::getTableName()).
                  Where(CAcquisitionTable::getIdColumnName(), CQUERY_OP_EQUAL, acquisitionId);
               if(m_databaseRequester->queryStatement(qDelete) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");
            }

         } //namespace requesters
      } //namespace sqlite
   } //namespace database 

