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
   boost::shared_ptr<entities::CRecipient> CRecipient::addRecipient(const entities::CRecipient & recipient)
   {
      if (recipient.FirstName.isDefined() && recipient.LastName.isDefined())
      {
         //check recipient do not already exists
         if (exists(recipient.FirstName(), recipient.LastName()))
         {
            std::string exMessage = (boost::format("Fail to insert recipient. The recipient %1% %2% already exists") % recipient.FirstName() % recipient.LastName()).str();
            throw shared::exception::CInvalidParameter(exMessage);
         }


         //insert recipient
         CQuery qInsert;
         qInsert.InsertInto(CRecipientTable::getTableName(), CRecipientTable::getFirstNameColumnName(), CRecipientTable::getLastNameColumnName()).
            Values(recipient.FirstName(), recipient.LastName());
         if(m_databaseRequester->queryStatement(qInsert) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert recipient");

         //retreive inserted recipient
         CQuery qSelect;
         qSelect. Select(CRecipientTable::getIdColumnName()).
            From(CRecipientTable::getTableName()).
            Where(CRecipientTable::getFirstNameColumnName(), CQUERY_OP_EQUAL, recipient.FirstName()).
            And(CRecipientTable::getLastNameColumnName(), CQUERY_OP_EQUAL, recipient.LastName()).
            OrderBy(CRecipientTable::getIdColumnName(), CQUERY_ORDER_DESC);

         adapters::CSingleValueAdapter<int> adapter;
         m_databaseRequester->queryEntities<int>(&adapter, qSelect);
         if (adapter.getResults().size() >= 1)
         {
            int insertedRecipientId = adapter.getResults()[0];

            WriteRecipientFields(insertedRecipientId, recipient.Fields());
            return getRecipient(insertedRecipientId);
         }
         else
         {
            throw shared::exception::CEmptyResult("Cannot retrieve inserted Recipient");
         }
      }
      else
      {
         throw shared::exception::CEmptyResult("Cannot add recipient without first and last name");
      }
   }

   boost::shared_ptr<entities::CRecipient> CRecipient::updateRecipient(const entities::CRecipient & recipient)
   {
      if (recipient.Id.isDefined())
      {
         if(recipient.FirstName.isDefined() && recipient.LastName.isDefined())
         {
            CQuery qInsert;
            qInsert.InsertOrReplaceInto(CRecipientTable::getTableName(), CRecipientTable::getIdColumnName(), CRecipientTable::getFirstNameColumnName(), CRecipientTable::getLastNameColumnName()).
               Values(recipient.Id(), recipient.FirstName(), recipient.LastName());

            if (m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("Fail to update recipient");

            //find the db id from first and last name
            boost::shared_ptr<entities::CRecipient> dbRecipient = getRecipient(recipient.FirstName(), recipient.LastName());
            //update fields
            WriteRecipientFields(dbRecipient->Id(), recipient.Fields());

            //read fully the recipient
            return getRecipient(recipient.FirstName(), recipient.LastName());
         }
         else
         {
            throw shared::exception::CEmptyResult("The recipient first and last name msut be defined");
         }
      }
      else
      {
         throw shared::exception::CEmptyResult("The recipient id must be defined");
      }
   }

   std::vector<boost::shared_ptr<entities::CRecipient> > CRecipient::getRecipients()
   {
      CQuery qSelect;
      qSelect. Select().
         From(CRecipientTable::getTableName()).
         OrderBy( CRecipientTable::getFirstNameColumnName(), CQUERY_ORDER_ASC, 
                  CRecipientTable::getLastNameColumnName(), CQUERY_ORDER_ASC);

      adapters::CRecipientAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CRecipient> >(&adapter, qSelect);

      //read all recipients
      std::vector<boost::shared_ptr<database::entities::CRecipient> > recipients = adapter.getResults();

      //for each one, read its fields (not contained in recipient table)
      for (std::vector<boost::shared_ptr<database::entities::CRecipient> >::iterator i = recipients.begin(); i != recipients.end(); ++i)
         ReadRecipientFields(*i);

      return recipients;
   }
   

   boost::shared_ptr<entities::CRecipient> CRecipient::getRecipient(const int recipientId)
   {
      CQuery qSelect;
      qSelect.Select().
         From(CRecipientTable::getTableName()).
         Where(CRecipientTable::getIdColumnName(), CQUERY_OP_EQUAL, recipientId).
         OrderBy(CRecipientTable::getFirstNameColumnName(), CQUERY_ORDER_ASC,
         CRecipientTable::getLastNameColumnName(), CQUERY_ORDER_ASC);

      adapters::CRecipientAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CRecipient> >(&adapter, qSelect);

      //read all recipients
      std::vector<boost::shared_ptr<database::entities::CRecipient> > recipients = adapter.getResults();

      if (recipients.size() >= 1)
      {
         boost::shared_ptr<database::entities::CRecipient> result = recipients[0];
         ReadRecipientFields(result);
         return result;
      }

      std::string sEx = (boost::format("Cannot retrieve Recipient Id=%1% in database") % recipientId).str();
      throw shared::exception::CEmptyResult(sEx);
   }


   boost::shared_ptr<entities::CRecipient> CRecipient::getRecipient(const std::string & firstName, const std::string & lastName)
   {
      CQuery qSelect;
      qSelect.Select().
         From(CRecipientTable::getTableName()).
         Where(CRecipientTable::getFirstNameColumnName(), CQUERY_OP_EQUAL, firstName).
         And(CRecipientTable::getLastNameColumnName(), CQUERY_OP_EQUAL, lastName).
         OrderBy(CRecipientTable::getFirstNameColumnName(), CQUERY_ORDER_ASC,
         CRecipientTable::getLastNameColumnName(), CQUERY_ORDER_ASC);

      adapters::CRecipientAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CRecipient> >(&adapter, qSelect);

      //read all recipients
      std::vector<boost::shared_ptr<database::entities::CRecipient> > recipients = adapter.getResults();

      if (recipients.size() >= 1)
      {
         boost::shared_ptr<database::entities::CRecipient> result = recipients[0];
         ReadRecipientFields(result);
         return result;
      }

      std::string sEx = (boost::format("Cannot retrieve Recipient FirstName=%1% LastName=%2% in database") % firstName % lastName).str();
      throw shared::exception::CEmptyResult(sEx);
   }

   boost::shared_ptr<entities::CRecipient> CRecipient::findRecipient(const std::string& fieldName, const std::string& expectedFieldValue)
   {
      //TODO_HELP_JMD ! Je veux retrouver le recipient qui contient un field dont le nom est fieldName et que la valeur de ce field soit expectedFieldValue
      throw shared::exception::CNotImplemented("CRecipient::findRecipient");
   }


   bool CRecipient::exists(const std::string & firstName, const std::string & lastName)
   {
      CQuery qSelect;
      qSelect.SelectCount().
         From(CRecipientTable::getTableName()).
         Where(CRecipientTable::getFirstNameColumnName(), CQUERY_OP_EQUAL, firstName).
         And(CRecipientTable::getLastNameColumnName(), CQUERY_OP_EQUAL, lastName);

      int count = m_databaseRequester->queryCount(qSelect);
      return count != 0;
   }

   bool CRecipient::exists(const int id)
   {
      CQuery qSelect;
      qSelect.SelectCount().
         From(CRecipientTable::getTableName()).
         Where(CRecipientTable::getIdColumnName(), CQUERY_OP_EQUAL, id);

      int count = m_databaseRequester->queryCount(qSelect);
      return count != 0;
   }


   void CRecipient::removeRecipient(int recipientId)
   {
      //delete recipient
      CQuery qDelete;
      qDelete. DeleteFrom(CRecipientTable::getTableName()).
         Where(CRecipientTable::getIdColumnName(), CQUERY_OP_EQUAL, recipientId);
      if(m_databaseRequester->queryStatement(qDelete) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");

      //delete recipient fields
      qDelete.Clear().DeleteFrom(CRecipientFieldTable::getTableName())
         .Where(CRecipientFieldTable::getIdRecipientColumnName(), CQUERY_OP_EQUAL, recipientId);
      m_databaseRequester->queryStatement(qDelete);
   }

   void CRecipient::removeAllRecipients()
   {
      //delete recipient
      CQuery qDelete;
      qDelete. DeleteFrom(CRecipientTable::getTableName());
      m_databaseRequester->queryStatement(qDelete);

      //delete recipient fields
      qDelete.Clear().DeleteFrom(CRecipientFieldTable::getTableName());
      m_databaseRequester->queryStatement(qDelete);
   }
   // [END] IRecipientRequester implementation

   void CRecipient::ReadRecipientFields(boost::shared_ptr<database::entities::CRecipient> & recipientToComplete)
   {
      CQuery qSelect;
      qSelect.Select().
         From(CRecipientFieldTable::getTableName()).
         Where(CRecipientFieldTable::getIdRecipientColumnName(), CQUERY_OP_EQUAL, recipientToComplete->Id());

      adapters::CRecipientFieldAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CRecipientField> >(&adapter, qSelect);

      recipientToComplete->Fields().clear();

      std::vector< boost::shared_ptr<database::entities::CRecipientField> > fields = adapter.getResults();
      for (std::vector< boost::shared_ptr<database::entities::CRecipientField> >::iterator i = fields.begin(); i != fields.end(); ++i)
      {
         recipientToComplete->Fields().push_back(*i);
      }
   }

   void CRecipient::WriteRecipientFields(const int recipientId, const std::vector< boost::shared_ptr<database::entities::CRecipientField> > & fields)
   {
      //remove all existing fields
      CQuery removeFields;
      removeFields.DeleteFrom(CRecipientFieldTable::getTableName()).
         Where(CRecipientFieldTable::getIdRecipientColumnName(), CQUERY_OP_EQUAL, recipientId);
      m_databaseRequester->queryStatement(removeFields);

      //insert fields
      for (std::vector< boost::shared_ptr<database::entities::CRecipientField> >::const_iterator i = fields.begin(); i != fields.end(); ++i)
      {
         if ((*i)->Value.isDefined() && !(*i)->Value().empty())
         {
            //insert or update value in RecipientFieldsTable
            CQuery qInsert;
            qInsert.InsertOrReplaceInto(CRecipientFieldTable::getTableName(), CRecipientFieldTable::getIdRecipientColumnName(), CRecipientFieldTable::getPluginNameColumnName(), CRecipientFieldTable::getFieldNameColumnName(), CRecipientFieldTable::getValueColumnName()).
               Values(recipientId, (*i)->PluginName(), (*i)->FieldName(), (*i)->Value());

            if (m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("Fail to insert field : " + (*i)->PluginName() + "." + (*i)->FieldName() + " : " + (*i)->Value());
         }
         else
         {
            //value is not defined, do not insert it
         }
      }
   }
} //namespace requesters
} //namespace sqlite
} //namespace database 



