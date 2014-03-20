#include "stdafx.h"
#include "SQLiteXplMessageRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"

namespace database {  namespace sqlite {  namespace requesters { 

   CSQLiteXplMessageRequester::CSQLiteXplMessageRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CSQLiteXplMessageRequester::~CSQLiteXplMessageRequester()
   {
   }


   // IAcquisitionRequester implementation
   int CSQLiteXplMessageRequester::addXplMessage(const database::entities::CXplMessage & newXplMessage, const std::vector<database::entities::CXplMessageContent> & messageContent)
   {
      CQuery qInsert;

      //create XplMessage entry
      qInsert.InsertInto(CXplMessageTable::getTableName(), CXplMessageTable::getTypeColumnName(), CXplMessageTable::getHopColumnName(), CXplMessageTable::getSourceColumnName(), CXplMessageTable::getTargetColumnName(), CXplMessageTable::getMessageSchemaIdColumnName(), CXplMessageTable::getDateColumnName()).
         Values(newXplMessage.getType(), newXplMessage.getHop(), newXplMessage.getSource(), newXplMessage.getTarget(), newXplMessage.getMessageSchemaId(), newXplMessage.getDate());

      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("Fail to insert xpl message");

      //retreive XplMessage entry to get the xplMessageId
      CQuery qSelect;
      qSelect. Select(CXplMessageTable::getIdColumnName()).
         From(CXplMessageTable::getTableName()).
         Where(CXplMessageTable::getTypeColumnName(), CQUERY_OP_EQUAL, newXplMessage.getType()).
         And(CXplMessageTable::getHopColumnName(), CQUERY_OP_EQUAL, newXplMessage.getHop()).
         And(CXplMessageTable::getSourceColumnName(), CQUERY_OP_EQUAL, newXplMessage.getSource()).
         And(CXplMessageTable::getTargetColumnName(), CQUERY_OP_EQUAL, newXplMessage.getTarget()).
         And(CXplMessageTable::getMessageSchemaIdColumnName(), CQUERY_OP_EQUAL, newXplMessage.getMessageSchemaId()).
         And(CXplMessageTable::getDateColumnName(), CQUERY_OP_EQUAL, newXplMessage.getDate()).
         OrderBy(CXplMessageTable::getIdColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
      {
         int insertedId = adapter.getResults()[0];

         BOOST_FOREACH(database::entities::CXplMessageContent bodyLine, messageContent)
         {
            qInsert.Clear().InsertInto(CXplMessageContentTable::getTableName(), CXplMessageContentTable::getIdMessageColumnName(), CXplMessageContentTable::getKeyColumnName(), CXplMessageContentTable::getValueColumnName()).
               Values(insertedId, bodyLine.getKey(), bodyLine.getValue());
            if(m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("Fail to insert xpl message content");
         }

         return insertedId;
      }
      else
         throw shared::exception::CEmptyResult("Cannot retrieve inserted XplMessage");      
   }

} //namespace requesters
} //namespace sqlite
} //namespace database 

