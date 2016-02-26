#include "stdafx.h"
#include "Recipient.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/Result.h"
#include <Poco/URI.h>

namespace web { namespace rest { namespace service {

   std::string CRecipient::m_restKeyword = std::string("recipient");
   std::string CRecipient::m_restFieldKeyword = std::string("field");

   CRecipient::CRecipient(boost::shared_ptr<database::IDataProvider> dataProvider)
      :m_dataProvider(dataProvider)
   {
   }


   CRecipient::~CRecipient()
   {
   }

   const std::string & CRecipient::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CRecipient::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CRecipient::getAllRecipients);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CRecipient::getOneRecipient);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword), CRecipient::removeAllRecipients, CRecipient::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*"), CRecipient::removeOneRecipient, CRecipient::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword), CRecipient::addRecipient, CRecipient::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*"), CRecipient::updateRecipient, CRecipient::transactionalMethod);

      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restFieldKeyword), CRecipient::getAllRecipientFields);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)(m_restFieldKeyword)("*"), CRecipient::getAllRecipientsByField);
   }



   shared::CDataContainer CRecipient::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      boost::shared_ptr<database::ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
      shared::CDataContainer result;
      try
      {
         if (pTransactionalEngine)
            pTransactionalEngine->transactionBegin();
         result = realMethod(parameters, requestContent);
      }
      catch (std::exception &ex)
      {
         result = CResult::GenerateError(ex);
      }
      catch (...)
      {
         result = CResult::GenerateError("unknown exception recipient rest method");
      }

      if (pTransactionalEngine)
      {
         if (CResult::isSuccess(result))
            pTransactionalEngine->transactionCommit();
         else
            pTransactionalEngine->transactionRollback();
      }
      return result;
   }

   shared::CDataContainer CRecipient::getOneRecipient(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      if(parameters.size() <= 1)
         return CResult::GenerateError("Invalid parameter count (need id of the recipient in url)");

      int recipientId = boost::lexical_cast<int>(parameters[1]);
      boost::shared_ptr< database::entities::CRecipient > recipient = m_dataProvider->getRecipientRequester()->getRecipient(recipientId);
      return CResult::GenerateSuccess(recipient);
   }

   shared::CDataContainer CRecipient::getAllRecipientsByField(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      if(parameters.size() != 3)
         return CResult::GenerateError("Invalid parameter count (need name of the field in url)");

      std::string fieldName = parameters[2];
      std::vector< boost::shared_ptr<database::entities::CRecipientField> > fields = m_dataProvider->getRecipientRequester()->getFieldsByName(fieldName);
      shared::CDataContainer collection;
      collection.set(m_restFieldKeyword, fields);
      return CResult::GenerateSuccess(collection);
   }

   shared::CDataContainer CRecipient::getAllRecipients(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CRecipient> > dvList = m_dataProvider->getRecipientRequester()->getRecipients();
      shared::CDataContainer collection;
      collection.set(getRestKeyword(), dvList);
      return CResult::GenerateSuccess(collection);
   }

   shared::CDataContainer CRecipient::getAllRecipientFields(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CRecipientField> > dvList = m_dataProvider->getRecipientRequester()->getFields();
      shared::CDataContainer collection;
      collection.set(m_restFieldKeyword, dvList);
      return CResult::GenerateSuccess(collection);
   }

   shared::CDataContainer CRecipient::addRecipient(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         database::entities::CRecipient toAdd;
         toAdd.fillFromContent(requestContent);
         boost::shared_ptr<database::entities::CRecipient> recipientFound = m_dataProvider->getRecipientRequester()->addRecipient(toAdd);
         return CResult::GenerateSuccess(recipientFound);
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in creating a new recipient");
      }
   }  
   
   shared::CDataContainer CRecipient::updateRecipient(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      if (parameters.size() <= 1)
         return CResult::GenerateError("Invalid parameter count (need id of the recipient to update in url)");

      int recipientId = boost::lexical_cast<int>(parameters[1]);
      try
      {
         database::entities::CRecipient toUpdate;

         requestContent.printToLog();

         toUpdate.fillFromContent(requestContent);
         if (!toUpdate.Id.isDefined() || toUpdate.Id() != recipientId)
            return CResult::GenerateError("The recipient id in url do not match request content recipient id");

         boost::shared_ptr<database::entities::CRecipient> recipientFound = m_dataProvider->getRecipientRequester()->updateRecipient(toUpdate);
         return CResult::GenerateSuccess(recipientFound);
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in creating a new recipient");
      }
   }

   shared::CDataContainer CRecipient::removeOneRecipient(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size()>1)
         {
            int recipientId = boost::lexical_cast<int>(parameters[1].c_str());
            m_dataProvider->getRecipientRequester()->removeRecipient(recipientId);
            return CResult::GenerateSuccess();
         }
         return CResult::GenerateError("The recipient to delete must have an id");
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in deleting a recipient");
      }
   }

   shared::CDataContainer CRecipient::removeAllRecipients(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         m_dataProvider->getRecipientRequester()->removeAllRecipients();
         return CResult::GenerateSuccess();
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in deleting a recipient");
      }
   }




} //namespace service
} //namespace rest
} //namespace web 
