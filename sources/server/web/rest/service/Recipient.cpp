#include "stdafx.h"
#include "Recipient.h"
#include <shared/exception/NotImplemented.hpp>

#include "web/poco/RestResult.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CRecipient::m_restKeyword = std::string("recipient");
         std::string CRecipient::m_restFieldKeyword = std::string("field");

         CRecipient::CRecipient(boost::shared_ptr<database::IDataProvider> dataProvider)
            : m_dataProvider(dataProvider)
         {
         }


         CRecipient::~CRecipient()
         {
         }

         const std::string& CRecipient::getRestKeyword()
         {
            return m_restKeyword;
         }

         void CRecipient::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CRecipient::getAllRecipients);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CRecipient::getOneRecipient);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CRecipient::removeAllRecipients,
                                                        CRecipient::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CRecipient::removeOneRecipient,
                                                        CRecipient::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CRecipient::addRecipient,
                                                        CRecipient::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CRecipient::updateRecipient,
                                                        CRecipient::transactionalMethod);

            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restFieldKeyword), CRecipient::getAllRecipientFields);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restFieldKeyword)("*"), CRecipient::getAllRecipientsByField);
         }

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CRecipient::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            //TODO

            return m_endPoints;
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CRecipient::transactionalMethod(
            poco::CRestDispatcher::CRestMethodHandler realMethod,
            const std::vector<std::string>& parameters, const std::string& requestContent)
         {
            boost::shared_ptr<database::ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
            boost::shared_ptr<shared::serialization::IDataSerializable> result;
            try
            {
               if (pTransactionalEngine)
                  pTransactionalEngine->transactionBegin();
               result = realMethod(parameters, requestContent);
            }
            catch (std::exception& ex)
            {
               result = poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               result = poco::CRestResult::GenerateError("unknown exception recipient rest method");
            }

            if (pTransactionalEngine)
            {
               if (poco::CRestResult::isSuccess(*boost::dynamic_pointer_cast<shared::CDataContainer>(result)))
                  pTransactionalEngine->transactionCommit();
               else
                  pTransactionalEngine->transactionRollback();
            }
            return result;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CRecipient::getOneRecipient(
            const std::vector<std::string>& parameters, const std::string& requestContent)
         {
            if (parameters.size() <= 1)
               return poco::CRestResult::GenerateError("Invalid parameter count (need id of the recipient in url)");

            int recipientId = boost::lexical_cast<int>(parameters[1]);
            boost::shared_ptr<database::entities::CRecipient> recipient = m_dataProvider->getRecipientRequester()->getRecipient(recipientId);
            return poco::CRestResult::GenerateSuccess(recipient);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CRecipient::getAllRecipientsByField(
            const std::vector<std::string>& parameters, const std::string& requestContent)
         {
            if (parameters.size() != 3)
               return poco::CRestResult::GenerateError("Invalid parameter count (need name of the field in url)");

            std::string fieldName = parameters[2];
            std::vector<boost::shared_ptr<database::entities::CRecipientField>> fields = m_dataProvider->getRecipientRequester()->
               getFieldsByName(fieldName);
            shared::CDataContainer collection;
            collection.set(m_restFieldKeyword, fields);
            return poco::CRestResult::GenerateSuccess(collection);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CRecipient::getAllRecipients(
            const std::vector<std::string>& parameters, const std::string& requestContent)
         {
            std::vector<boost::shared_ptr<database::entities::CRecipient>> dvList = m_dataProvider->getRecipientRequester()->getRecipients();
            shared::CDataContainer collection;
            collection.set(getRestKeyword(), dvList);
            return poco::CRestResult::GenerateSuccess(collection);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CRecipient::getAllRecipientFields(
            const std::vector<std::string>& parameters, const std::string& requestContent)
         {
            std::vector<boost::shared_ptr<database::entities::CRecipientField>> dvList = m_dataProvider->getRecipientRequester()->getFields();
            shared::CDataContainer collection;
            collection.set(m_restFieldKeyword, dvList);
            return poco::CRestResult::GenerateSuccess(collection);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CRecipient::addRecipient(
            const std::vector<std::string>& parameters, const std::string& requestContent)
         {
            try
            {
               database::entities::CRecipient toAdd;
               toAdd.fillFromSerializedString(requestContent);
               boost::shared_ptr<database::entities::CRecipient> recipientFound = m_dataProvider->getRecipientRequester()->addRecipient(toAdd);
               return poco::CRestResult::GenerateSuccess(recipientFound);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new recipient");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CRecipient::updateRecipient(
            const std::vector<std::string>& parameters, const std::string& requestContent)
         {
            if (parameters.size() <= 1)
               return poco::CRestResult::GenerateError("Invalid parameter count (need id of the recipient to update in url)");

            int recipientId = boost::lexical_cast<int>(parameters[1]);
            try
            {
               database::entities::CRecipient toUpdate;
               toUpdate.fillFromSerializedString(requestContent);
               if (!toUpdate.Id.isDefined() || toUpdate.Id() != recipientId)
                  return poco::CRestResult::GenerateError("The recipient id in url does not match request content recipient id");

               boost::shared_ptr<database::entities::CRecipient> recipientFound = m_dataProvider->getRecipientRequester()->updateRecipient(toUpdate);
               return poco::CRestResult::GenerateSuccess(recipientFound);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in creating a new recipient");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CRecipient::removeOneRecipient(
            const std::vector<std::string>& parameters, const std::string& requestContent)
         {
            try
            {
               if (parameters.size() > 1)
               {
                  int recipientId = boost::lexical_cast<int>(parameters[1].c_str());
                  m_dataProvider->getRecipientRequester()->removeRecipient(recipientId);
                  return poco::CRestResult::GenerateSuccess();
               }
               return poco::CRestResult::GenerateError("The recipient to delete must have an id");
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in deleting a recipient");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CRecipient::removeAllRecipients(
            const std::vector<std::string>& parameters, const std::string& requestContent)
         {
            try
            {
               m_dataProvider->getRecipientRequester()->removeAllRecipients();
               return poco::CRestResult::GenerateSuccess();
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in deleting a recipient");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
