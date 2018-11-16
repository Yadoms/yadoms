#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"


namespace web { namespace rest { namespace service {

   class CRecipient : public IRestService
   {
   public:
      explicit CRecipient(boost::shared_ptr<database::IDataProvider> dataProvider);
      virtual ~CRecipient();

   public:
      // IRestService implementation
      void configureDispatcher(CRestDispatcher & dispatcher) override;
      // [END] IRestService implementation

      static const std::string & getRestKeyword();
      boost::shared_ptr<shared::serialization::IDataSerializable> getOneRecipient(const std::vector<std::string> & parameters, const std::string & requestContent);
      boost::shared_ptr<shared::serialization::IDataSerializable> getAllRecipients(const std::vector<std::string> & parameters, const std::string & requestContent);
      boost::shared_ptr<shared::serialization::IDataSerializable> addRecipient(const std::vector<std::string> & parameters, const std::string & requestContent);
      boost::shared_ptr<shared::serialization::IDataSerializable> updateRecipient(const std::vector<std::string> & parameters, const std::string & requestContent);
      boost::shared_ptr<shared::serialization::IDataSerializable> removeOneRecipient(const std::vector<std::string> & parameters, const std::string & requestContent);
      boost::shared_ptr<shared::serialization::IDataSerializable> removeAllRecipients(const std::vector<std::string> & parameters, const std::string & requestContent);

      boost::shared_ptr<shared::serialization::IDataSerializable> getAllRecipientFields(const std::vector<std::string> & parameters, const std::string & requestContent);
      boost::shared_ptr<shared::serialization::IDataSerializable> getAllRecipientsByField(const std::vector<std::string> & parameters, const std::string & requestContent);

   private:
      static std::string m_restKeyword;
      static std::string m_restFieldKeyword;
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const std::string & requestContent);
   };


} //namespace service
} //namespace rest
} //namespace web 
