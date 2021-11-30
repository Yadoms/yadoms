#include "stdafx.h"
#include "Recipient.h"
#include <shared/exception/EmptyResult.hpp>
#include <utility>
#include "database/common/DatabaseTables.h"
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/Query.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         CRecipient::CRecipient(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(std::move(databaseRequester))
         {
         }

         // IRecipientRequester implementation
         boost::shared_ptr<entities::CRecipient> CRecipient::addRecipient(const entities::CRecipient& recipient)
         {
            if (recipient.FirstName.isDefined() && recipient.LastName.isDefined())
            {
               //check recipient do not already exists
               if (exists(recipient.FirstName(), recipient.LastName()))
               {
                  const auto exMessage = (boost::format("Fail to insert recipient. The recipient %1% %2% already exists") % recipient.FirstName() %
                     recipient.LastName()).str();
                  throw shared::exception::CInvalidParameter(exMessage);
               }


               //insert recipient
               const auto qInsert = m_databaseRequester->newQuery();
               qInsert->InsertInto(CRecipientTable::getTableName(), CRecipientTable::getFirstNameColumnName(),
                                   CRecipientTable::getLastNameColumnName()).
                        Values(recipient.FirstName(), recipient.LastName());
               if (m_databaseRequester->queryStatement(*qInsert) <= 0)
                  throw shared::exception::CEmptyResult("Fail to insert recipient");

               //retrieve inserted recipient
               const auto qSelect = m_databaseRequester->newQuery();
               qSelect->Select(CRecipientTable::getIdColumnName()).
                        From(CRecipientTable::getTableName()).
                        Where(CRecipientTable::getFirstNameColumnName(), CQUERY_OP_EQUAL, recipient.FirstName()).
                        And(CRecipientTable::getLastNameColumnName(), CQUERY_OP_EQUAL, recipient.LastName()).
                        OrderBy(CRecipientTable::getIdColumnName(), CQuery::kDesc);

               adapters::CSingleValueAdapter<int> adapter;
               m_databaseRequester->queryEntities(&adapter, *qSelect);

               if (adapter.getResults().empty())
                  throw shared::exception::CEmptyResult("Cannot retrieve inserted Recipient");

               const int insertedRecipientId = adapter.getResults()[0];

               writeRecipientFields(insertedRecipientId, recipient.Fields());
               return getRecipient(insertedRecipientId);
            }
            throw shared::exception::CEmptyResult("Cannot add recipient without first and last name");
         }

         int CRecipient::createUser(const entities::CRecipient& user)
         {
            if (!user.FirstName.isDefined() || !user.LastName.isDefined())
               throw shared::exception::CEmptyResult("Cannot add user without first and last name");

            if (exists(user.FirstName(), user.LastName()))
            {
               const auto exMessage = (boost::format("Fail to insert user. The user %1% %2% already exists") % user.FirstName() %
                  user.LastName()).str();
               throw shared::exception::CInvalidParameter(exMessage);
            }

            // Create user
            const auto qInsert = m_databaseRequester->newQuery();
            qInsert->InsertInto(CRecipientTable::getTableName(),
                                CRecipientTable::getFirstNameColumnName(), CRecipientTable::getLastNameColumnName()).
                     Values(user.FirstName(), user.LastName());
            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("Fail to create user");

            // Retrieve created user
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CRecipientTable::getIdColumnName()).
                     From(CRecipientTable::getTableName()).
                     Where(CRecipientTable::getFirstNameColumnName(), CQUERY_OP_EQUAL, user.FirstName()).
                     And(CRecipientTable::getLastNameColumnName(), CQUERY_OP_EQUAL, user.LastName());

            adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            if (adapter.getResults().size() != 1)
               throw shared::exception::CEmptyResult("Cannot retrieve inserted user");

            return adapter.getResults()[0];
         }

         boost::shared_ptr<entities::CRecipient> CRecipient::updateRecipient(const entities::CRecipient& recipient)
         {
            if (recipient.Id.isDefined())
            {
               if (recipient.FirstName.isDefined() && recipient.LastName.isDefined())
               {
                  const auto qUpdate = m_databaseRequester->newQuery();
                  qUpdate->Update(CRecipientTable::getTableName())
                         .Set(CRecipientTable::getFirstNameColumnName(), recipient.FirstName(), CRecipientTable::getLastNameColumnName(),
                              recipient.LastName())
                         .Where(CRecipientTable::getIdColumnName(), CQUERY_OP_EQUAL, recipient.Id());

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw shared::exception::CEmptyResult("Fail to update recipient");

                  //find the db id from first and last name
                  const auto dbRecipient = getRecipient(recipient.FirstName(), recipient.LastName());
                  //update fields
                  writeRecipientFields(dbRecipient->Id(), recipient.Fields());

                  //read fully the recipient
                  return getRecipient(recipient.FirstName(), recipient.LastName());
               }
               throw shared::exception::CEmptyResult("The recipient first and last name msut be defined");
            }
            throw shared::exception::CEmptyResult("The recipient id must be defined");
         }

         void CRecipient::updateUser(const entities::CRecipient& user)
         {
            if (!user.Id.isDefined())
               throw CDatabaseException("Need an id to update");

            const auto query = m_databaseRequester->newQuery();
            query->Update(CRecipientTable::getTableName());

            if (user.FirstName.isDefined())
               query->MultiSet(CRecipientTable::getFirstNameColumnName(), user.FirstName());
            if (user.LastName.isDefined())
               query->MultiSet(CRecipientTable::getLastNameColumnName(), user.LastName());

            query->Where(CRecipientTable::getIdColumnName(), CQUERY_OP_EQUAL, user.Id());

            if (m_databaseRequester->queryStatement(*query) <= 0)
               throw shared::exception::CEmptyResult("Fail to update user");
         }

         std::vector<boost::shared_ptr<entities::CRecipient>> CRecipient::getRecipients()
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CRecipientTable::getTableName()).
                     OrderBy(CRecipientTable::getFirstNameColumnName(), CQuery::kAsc,
                             CRecipientTable::getLastNameColumnName(), CQuery::kAsc);

            adapters::CRecipientAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            //read all recipients
            auto recipients = adapter.getResults();

            //for each one, read its fields (not contained in recipient table)
            for (auto& recipient : recipients)
               readRecipientFields(recipient);

            return recipients;
         }

         std::vector<boost::shared_ptr<entities::CRecipient>> CRecipient::getUsers(const boost::optional<int>& userId,
                                                                                   const boost::optional<std::string>& firstName,
                                                                                   const boost::optional<std::string>& lastName)
         {
            const auto query = m_databaseRequester->newQuery();

            query->Select().
                   From(CRecipientTable::getTableName()).
                   WhereTrue();

            if (userId)
               query->And(CRecipientTable::getIdColumnName(), CQUERY_OP_EQUAL, *userId);
            if (firstName)
               query->And(CRecipientTable::getFirstNameColumnName(), CQUERY_OP_EQUAL, *firstName);
            if (lastName)
               query->And(CRecipientTable::getLastNameColumnName(), CQUERY_OP_EQUAL, *lastName);

            adapters::CRecipientAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *query);

            return adapter.getResults();
         }

         boost::shared_ptr<entities::CRecipient> CRecipient::getRecipient(int recipientId)
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CRecipientTable::getTableName()).
                     Where(CRecipientTable::getIdColumnName(), CQUERY_OP_EQUAL, recipientId).
                     OrderBy(CRecipientTable::getFirstNameColumnName(), CQuery::kAsc,
                             CRecipientTable::getLastNameColumnName(), CQuery::kAsc);

            adapters::CRecipientAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            //read all recipients
            const auto recipients = adapter.getResults();

            if (recipients.empty())
               throw shared::exception::CEmptyResult((boost::format("Cannot retrieve Recipient Id=%1% in database") % recipientId).str());

            auto result = recipients[0];
            readRecipientFields(result);
            return result;
         }


         boost::shared_ptr<entities::CRecipient> CRecipient::getRecipient(const std::string& firstName,
                                                                          const std::string& lastName)
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CRecipientTable::getTableName()).
                     Where(CRecipientTable::getFirstNameColumnName(), CQUERY_OP_EQUAL, firstName).
                     And(CRecipientTable::getLastNameColumnName(), CQUERY_OP_EQUAL, lastName).
                     OrderBy(CRecipientTable::getFirstNameColumnName(), CQuery::kAsc,
                             CRecipientTable::getLastNameColumnName(), CQuery::kAsc);

            adapters::CRecipientAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            //read all recipients
            const auto recipients = adapter.getResults();

            if (recipients.empty())
               throw shared::exception::CEmptyResult(
                  (boost::format("Cannot retrieve Recipient FirstName=%1% LastName=%2% in database") % firstName % lastName).str());

            auto result = recipients[0];
            readRecipientFields(result);
            return result;
         }

         std::vector<boost::shared_ptr<entities::CRecipient>> CRecipient::findRecipientsFromField(const std::string& fieldName,
                                                                                                  const std::string& expectedFieldValue)
         {
            const auto qSelect = m_databaseRequester->newQuery();
            const auto qubQuery = m_databaseRequester->newQuery();
            qubQuery->Select(CRecipientFieldTable::getIdRecipientColumnName()).
                      From(CRecipientFieldTable::getTableName()).
                      Where(CRecipientFieldTable::getFieldNameColumnName(), CQUERY_OP_EQUAL, fieldName).
                      And(CRecipientFieldTable::getValueColumnName(), CQUERY_OP_EQUAL, expectedFieldValue);

            qSelect->Select().
                     From(CRecipientTable::getTableName()).
                     Where(CRecipientTable::getIdColumnName(), CQUERY_OP_IN, *qubQuery);

            adapters::CRecipientAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            //read all recipients
            auto recipients = adapter.getResults();

            //for each one, read its fields (not contained in recipient table)
            for (auto& recipient : recipients)
               readRecipientFields(recipient);

            return recipients;
         }

         std::vector<boost::shared_ptr<entities::CRecipientField>> CRecipient::getFields()
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CRecipientFieldTable::getTableName());

            adapters::CRecipientFieldAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CRecipientField>> CRecipient::getFieldsByName(const std::string& fieldName)
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CRecipientFieldTable::getTableName()).
                     Where(CRecipientFieldTable::getFieldNameColumnName(), CQUERY_OP_EQUAL, fieldName);

            adapters::CRecipientFieldAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            return adapter.getResults();
         }

         bool CRecipient::exists(const std::string& firstName,
                                 const std::string& lastName)
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->SelectCount().
                     From(CRecipientTable::getTableName()).
                     Where(CRecipientTable::getFirstNameColumnName(), CQUERY_OP_EQUAL, firstName).
                     And(CRecipientTable::getLastNameColumnName(), CQUERY_OP_EQUAL, lastName);

            const int count = m_databaseRequester->queryCount(*qSelect);
            return count != 0;
         }

         bool CRecipient::exists(const int id)
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->SelectCount().
                     From(CRecipientTable::getTableName()).
                     Where(CRecipientTable::getIdColumnName(), CQUERY_OP_EQUAL, id);

            const int count = m_databaseRequester->queryCount(*qSelect);
            return count != 0;
         }


         void CRecipient::removeRecipient(int recipientId)
         {
            //delete recipient
            const auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CRecipientTable::getTableName()).
                     Where(CRecipientTable::getIdColumnName(), CQUERY_OP_EQUAL, recipientId);
            if (m_databaseRequester->queryStatement(*qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            //delete recipient fields
            qDelete->Clear().DeleteFrom(CRecipientFieldTable::getTableName())
                   .Where(CRecipientFieldTable::getIdRecipientColumnName(), CQUERY_OP_EQUAL, recipientId);
            m_databaseRequester->queryStatement(*qDelete);
         }

         void CRecipient::removeUser(int userId)
         {
            // Delete user
            const auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CRecipientTable::getTableName()).
                     Where(CRecipientTable::getIdColumnName(), CQUERY_OP_EQUAL, userId);
            if (m_databaseRequester->queryStatement(*qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            // Delete recipient fields
            qDelete->Clear().DeleteFrom(CRecipientFieldTable::getTableName())
                   .Where(CRecipientFieldTable::getIdRecipientColumnName(), CQUERY_OP_EQUAL, userId);
            m_databaseRequester->queryStatement(*qDelete);
         }

         void CRecipient::removeAllRecipients()
         {
            //delete recipient
            const auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CRecipientTable::getTableName());
            m_databaseRequester->queryStatement(*qDelete);

            //delete recipient fields
            qDelete->Clear().DeleteFrom(CRecipientFieldTable::getTableName());
            m_databaseRequester->queryStatement(*qDelete);
         }

         // [END] IRecipientRequester implementation

         void CRecipient::readRecipientFields(const boost::shared_ptr<entities::CRecipient>& recipientToComplete) const
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CRecipientFieldTable::getTableName()).
                     Where(CRecipientFieldTable::getIdRecipientColumnName(), CQUERY_OP_EQUAL, recipientToComplete->Id());

            adapters::CRecipientFieldAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            recipientToComplete->Fields().clear();

            const auto fields = adapter.getResults();
            for (auto& field : fields)
               recipientToComplete->Fields().push_back(field);
         }

         void CRecipient::writeRecipientFields(int recipientId,
                                               const std::vector<boost::shared_ptr<entities::CRecipientField>>& fields) const
         {
            //remove all existing fields
            const auto removeFields = m_databaseRequester->newQuery();
            removeFields->DeleteFrom(CRecipientFieldTable::getTableName()).
                          Where(CRecipientFieldTable::getIdRecipientColumnName(), CQUERY_OP_EQUAL, recipientId);
            m_databaseRequester->queryStatement(*removeFields);

            //insert fields
            for (const auto& field : fields)
            {
               if (!field->Value.isDefined() || field->Value().empty())
                  continue; //value is not defined, do not insert it

               //insert or update value in RecipientFieldsTable
               auto qInsert = m_databaseRequester->newQuery();
               qInsert->InsertInto(CRecipientFieldTable::getTableName(), CRecipientFieldTable::getIdRecipientColumnName(),
                                   CRecipientFieldTable::getPluginTypeColumnName(), CRecipientFieldTable::getFieldNameColumnName(),
                                   CRecipientFieldTable::getValueColumnName()).
                        Values(recipientId, field->PluginType(), field->FieldName(), field->Value());

               if (m_databaseRequester->queryStatement(*qInsert) <= 0)
                  throw shared::exception::CEmptyResult(
                     "Fail to insert field : " + field->PluginType() + "." + field->FieldName() + " : " + field->Value());
            }
         }

         bool CRecipient::fieldExists(const std::string& fieldName, const std::string& pluginName) const
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->SelectCount().
                     From(CRecipientFieldTable::getTableName()).
                     Where(CRecipientFieldTable::getFieldNameColumnName(), CQUERY_OP_EQUAL, fieldName).
                     And(CRecipientFieldTable::getPluginTypeColumnName(), CQUERY_OP_EQUAL, pluginName);

            const int count = m_databaseRequester->queryCount(*qSelect);
            return count != 0;
         }

         boost::shared_ptr<entities::CRecipientField> CRecipient::createField(const entities::CRecipientField& field)
         {
            if (!field.FieldName.isDefined() || !field.PluginType.isDefined())
               throw shared::exception::CEmptyResult("Cannot add recipient field without name and plugin name");

            //check field do not already exists
            if (fieldExists(field.FieldName(), field.PluginType()))
               throw shared::exception::CInvalidParameter(
                  (boost::format("Fail to insert recipient field %1% (already exists)") % field.FieldName()).str());

            //insert field
            const auto qInsert = m_databaseRequester->newQuery();
            qInsert->InsertInto(CRecipientFieldTable::getTableName(), CRecipientFieldTable::getFieldNameColumnName(),
                                CRecipientFieldTable::getPluginTypeColumnName()).
                     Values(field.FieldName(), field.PluginType());
            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("Fail to insert recipient field");

            //retrieve inserted field
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CRecipientFieldTable::getTableName()).
                     Where(CRecipientFieldTable::getFieldNameColumnName(), CQUERY_OP_EQUAL, field.FieldName()).
                     And(CRecipientFieldTable::getPluginTypeColumnName(), CQUERY_OP_EQUAL, field.PluginType());

            adapters::CRecipientFieldAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() != 1)
               throw shared::exception::CEmptyResult("Fail to insert recipient field : cannot retrieve inserted field");

            return adapter.getResults()[0];
         }
      } //namespace requesters
   } //namespace common
} //namespace database 
