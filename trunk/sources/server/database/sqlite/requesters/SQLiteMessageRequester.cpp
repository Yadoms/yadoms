#include "stdafx.h"
#include "SQLiteMessageRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"

namespace database {  namespace sqlite {  namespace requesters { 

   CSQLiteMessageRequester::CSQLiteMessageRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CSQLiteMessageRequester::~CSQLiteMessageRequester()
   {
   }


   // IAcquisitionRequester implementation
   int CSQLiteMessageRequester::insertMessage(const database::entities::CMessage & newMessage, const std::vector<database::entities::CMessageContent> & messageContent)
   {
      CQuery qInsert;

      //create XplMessage entry
      qInsert.InsertInto(CMessageTable::getTableName(), CMessageTable::getDateColumnName(), CMessageTable::getDeviceIdColumnName()).
         Values(newMessage.getDate(), newMessage.getDeviceId());

      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("Fail to insert message");

      //retreive XplMessage entry to get the xplMessageId
      CQuery qSelect;
      qSelect. Select(CMessageTable::getIdColumnName()).
         From(CMessageTable::getTableName()).
         Where(CMessageTable::getDateColumnName(), CQUERY_OP_EQUAL, newMessage.getDate()).
         And(CMessageTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newMessage.getDeviceId()).
         OrderBy(CMessageTable::getIdColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
      {
         int insertedId = adapter.getResults()[0];

         BOOST_FOREACH(database::entities::CMessageContent bodyLine, messageContent)
         {
            qInsert.Clear().InsertInto(CMessageContentTable::getTableName(), CMessageContentTable::getIdMessageColumnName(), CMessageContentTable::getKeyColumnName(), CMessageContentTable::getValueColumnName()).
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

