#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "web/poco/RestDispatcher.h"


namespace web
{
   namespace rest
   {
      namespace service
      {
         class CRecipient final : public IRestService
         {
         public:
            explicit CRecipient(boost::shared_ptr<database::IDataProvider> dataProvider);
            ~CRecipient() override;

         public:
            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();
            boost::shared_ptr<shared::serialization::IDataSerializable> getOneRecipient(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllRecipients(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> addRecipient(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> updateRecipient(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> removeOneRecipient(
               const std::vector<std::string>& parameters, const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> removeAllRecipients(
               const std::vector<std::string>& parameters, const std::string& requestContent);

            boost::shared_ptr<shared::serialization::IDataSerializable> getAllRecipientFields(
               const std::vector<std::string>& parameters, const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllRecipientsByField(
               const std::vector<std::string>& parameters, const std::string& requestContent);

         private:
            static std::string m_restKeyword;
            static std::string m_restFieldKeyword;
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(poco::CRestDispatcher::CRestMethodHandler realMethod,
                                                                                            const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent);
         };
      } //namespace service
   } //namespace rest
} //namespace web 
