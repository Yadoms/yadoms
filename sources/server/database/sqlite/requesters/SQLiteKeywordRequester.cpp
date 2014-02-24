#include "stdafx.h"
#include "SQLiteKeywordRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"


   namespace database { 
      namespace sqlite { 
         namespace requesters { 

            CSQLiteKeywordRequester::CSQLiteKeywordRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
               :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
            {
            }

            CSQLiteKeywordRequester::~CSQLiteKeywordRequester()
            {
            }

            // IKeywordRequester implementation
            void CSQLiteKeywordRequester::addKeyword(boost::shared_ptr<database::entities::CKeyword> newKeyword)
            {
               CQuery qInsert;
               qInsert.InsertInto(CKeywordTable::getTableName(), CKeywordTable::getNameColumnName()).
                  Values(newKeyword->getName());

               if(m_databaseRequester->queryStatement(qInsert) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");
            }

            boost::shared_ptr<database::entities::CKeyword> CSQLiteKeywordRequester::getKeyword(const std::string & keyword)
            {
               database::sqlite::adapters::CKeywordAdapter adapter;

               CQuery qSelect;

               qSelect. Select().
                  From(CKeywordTable::getTableName()).
                  Where(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, keyword);

               m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CKeyword> >(&adapter, qSelect);
               if (adapter.getResults().empty())
               {
                  // Keyword not found
                  std::string sEx = (boost::format("Keyword name %1% not found in database") % keyword).str(); 
                  throw shared::exception::CException(sEx);
               }
               return adapter.getResults().at(0);
            }

            std::vector<boost::shared_ptr<database::entities::CKeyword> > CSQLiteKeywordRequester::getKeywords()
            {
               database::sqlite::adapters::CKeywordAdapter adapter;
               CQuery qSelect;
               qSelect. Select().
                  From(CKeywordTable::getTableName());
               m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CKeyword> >(&adapter, qSelect);
               return adapter.getResults();
            }

            void CSQLiteKeywordRequester::removeKeyword(const std::string & keyword)
            {
               CQuery qDelete;
               qDelete. DeleteFrom(CKeywordTable::getTableName()).
                  Where(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, keyword);
               if(m_databaseRequester->queryStatement(qDelete) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");
            }
            // [END] IKeywordRequester implementation

         } //namespace requesters
      } //namespace sqlite
   } //namespace database 

