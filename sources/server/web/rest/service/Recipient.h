#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "pluginSystem/Manager.h"
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
            explicit CRecipient(boost::shared_ptr<database::IDataProvider> dataProvider,
                                boost::shared_ptr<pluginSystem::CManager> pluginManager);
            ~CRecipient() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

         private:
            static const std::string& getRestKeyword();
            boost::shared_ptr<shared::serialization::IDataSerializable> getOneRecipientV1(const std::vector<std::string>& parameters,
                                                                                          const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllRecipientsV1(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> addRecipientV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> updateRecipientV1(const std::vector<std::string>& parameters,
                                                                                          const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> removeOneRecipientV1(
               const std::vector<std::string>& parameters, const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> removeAllRecipientsV1(
               const std::vector<std::string>& parameters, const std::string& requestContent);

            boost::shared_ptr<shared::serialization::IDataSerializable> getAllRecipientFieldsV1(
               const std::vector<std::string>& parameters, const std::string& requestContent);
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllRecipientsByFieldV1(
               const std::vector<std::string>& parameters, const std::string& requestContent);

            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethodV1(poco::CRestDispatcher::CRestMethodHandler realMethod,
               const std::vector<std::string>& parameters,
               const std::string& requestContent);

            boost::shared_ptr<IAnswer> getUsersV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> createUserV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> updateUserV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> deleteUserV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> getFieldsV2(const boost::shared_ptr<IRequest>& request) const;

            static std::string m_restKeyword;
            static std::string m_restFieldKeyword;
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            boost::shared_ptr<pluginSystem::CManager> m_pluginManager;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
