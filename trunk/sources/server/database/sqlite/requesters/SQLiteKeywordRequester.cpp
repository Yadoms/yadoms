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


namespace database { namespace sqlite { namespace requesters { 

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
      CQuery qSelect;

      qSelect. SelectCount().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword->getDeviceId()).
         And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword->getName());

      int count = m_databaseRequester->queryCount(qSelect);
      if(count == 0)
      {
         CQuery qInsert;
         qInsert.InsertInto(CKeywordTable::getTableName(), CKeywordTable::getDeviceIdColumnName(), CKeywordTable::getNameColumnName()).
            Values(newKeyword->getDeviceId(), newKeyword->getName());

         if(m_databaseRequester->queryStatement(qInsert) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert keyword into table");

         if(newKeyword->isUnitsFilled())
         {
            CQuery update;
            update.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getUnitsColumnName(), newKeyword->getUnits()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword->getDeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword->getName());

            if(m_databaseRequester->queryStatement(update) <= 0)
               throw shared::exception::CEmptyResult("Fail to update Unit field");
         }

         if(newKeyword->isTypeFilled())
         {
            CQuery update;
            update.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getTypeColumnName(), newKeyword->getType()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword->getDeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword->getName());

            if(m_databaseRequester->queryStatement(update) <= 0)
               throw shared::exception::CEmptyResult("Fail to update Type field");
         }

         if(newKeyword->isMinimumFilled())
         {
            CQuery update;
            update.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getMinimumColumnName(), newKeyword->getMinimum()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword->getDeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword->getName());

            if(m_databaseRequester->queryStatement(update) <= 0)
               throw shared::exception::CEmptyResult("Fail to update Minimum field");
         }

         if(newKeyword->isMaximumFilled())
         {
            CQuery update;
            update.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getMaximumColumnName(), newKeyword->getMaximum()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword->getDeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword->getName());

            if(m_databaseRequester->queryStatement(update) <= 0)
               throw shared::exception::CEmptyResult("Fail to update Maximum field");
         }    

         if(newKeyword->isParametersFilled())
         {
            CQuery update;
            update.Update(CKeywordTable::getTableName()).Set(CKeywordTable::getParametersColumnName(), newKeyword->getParameters()).
               Where(CKeywordTable::getDeviceIdColumnName(),  CQUERY_OP_EQUAL, newKeyword->getDeviceId()).
               And(CKeywordTable::getNameColumnName(),  CQUERY_OP_EQUAL, newKeyword->getName());

            if(m_databaseRequester->queryStatement(update) <= 0)
               throw shared::exception::CEmptyResult("Fail to update EnumValues field");
         }
      }
      else
      {
         //already exist, do nothing
      }
   }

   boost::shared_ptr<database::entities::CKeyword> CSQLiteKeywordRequester::getKeyword(const int deviceId, const std::string & keyword)
   {
      database::sqlite::adapters::CKeywordAdapter adapter;

      CQuery qSelect;

      qSelect. Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
         And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, keyword);

      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CKeyword> >(&adapter, qSelect);
      if (adapter.getResults().empty())
      {
         // Keyword not found
         std::string sEx = (boost::format("Keyword name %1% for device %2% not found in database") % keyword % deviceId).str(); 
         throw shared::exception::CException(sEx);
      }
      return adapter.getResults().at(0);
   }

   std::vector<boost::shared_ptr<database::entities::CKeyword> > CSQLiteKeywordRequester::getKeywords(const int deviceId)
   {
      database::sqlite::adapters::CKeywordAdapter adapter;
      CQuery qSelect;
      qSelect. Select().
         From(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId);
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CKeyword> >(&adapter, qSelect);
      return adapter.getResults();
   }

   void CSQLiteKeywordRequester::removeKeyword(const int deviceId, const std::string & keyword)
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CKeywordTable::getTableName()).
         Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
         And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, keyword);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }
   // [END] IKeywordRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 

