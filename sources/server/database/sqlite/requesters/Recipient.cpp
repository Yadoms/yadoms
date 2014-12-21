#include "stdafx.h"
#include "Recipient.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/Query.h"

namespace database {  namespace sqlite { namespace requesters { 

   CRecipient::CRecipient(boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseRequester(databaseRequester)
   {
   }

   CRecipient::~CRecipient()
   {
   }


   // IRecipientRequester implementation
   int CRecipient::addRecipient(const database::entities::CRecipient & recipient)
   {

      //CQuery qInsert;
      //if(recipient.Id.isDefined())
      //{
      //   qInsert. InsertInto(CRecipientTable::getTableName(), CRecipientTable::getIdColumnName(), CRecipientTable::getNameColumnName(), CRecipientTable::getRecipientOrderColumnName()).
      //      Values(recipient.Id(), recipient.Name(), recipient.RecipientOrder());
      //}
      //else
      //{
      //   qInsert. InsertInto(CRecipientTable::getTableName(), CRecipientTable::getNameColumnName(), CRecipientTable::getRecipientOrderColumnName()).
      //      Values(recipient.Name(), recipient.RecipientOrder());
      //}
		//
      //if(m_databaseRequester->queryStatement(qInsert) <= 0)
      //   throw shared::exception::CEmptyResult("No lines affected");
		//
      //CQuery qSelect;
      //qSelect. Select(CRecipientTable::getIdColumnName()).
      //   From(CRecipientTable::getTableName()).
      //   Where(CRecipientTable::getNameColumnName(), CQUERY_OP_EQUAL, recipient.Name()).
      //   OrderBy(CRecipientTable::getIdColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CSingleValueAdapter<int> adapter;
      //m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
         throw shared::exception::CEmptyResult("Cannot retrieve inserted Recipient");      
   }

   std::vector<boost::shared_ptr<database::entities::CRecipient> > CRecipient::getRecipients()
   {
      //CQuery qSelect;
      //qSelect. Select().
      //   From(CRecipientTable::getTableName()).
      //   OrderBy(CRecipientTable::getRecipientOrderColumnName());

      database::sqlite::adapters::CRecipientAdapter adapter;
      //m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CRecipient> >(&adapter, qSelect);
      return adapter.getResults();
   }
   
   void CRecipient::removeRecipient(int recipientId)
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CRecipientTable::getTableName()).
         Where(CRecipientTable::getIdColumnName(), CQUERY_OP_EQUAL, recipientId);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CRecipient::removeAllRecipients()
   {
      CQuery qDelete;
      qDelete. DeleteFrom(CRecipientTable::getTableName());
      m_databaseRequester->queryStatement(qDelete);
   }
   // [END] IRecipientRequester implementation


} //namespace requesters
} //namespace sqlite
} //namespace database 



