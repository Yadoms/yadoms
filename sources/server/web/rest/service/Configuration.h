#pragma once
#include "IRestService.h"
#include "dataAccessLayer/IConfigurationManager.h"
#include "web/rest/IAnswer.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CConfiguration final : public IRestService
         {
         public:
            explicit CConfiguration(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager);
            ~CConfiguration() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

         private:
            boost::shared_ptr<shared::serialization::IDataSerializable> resetServerConfigurationV1(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getServerConfigurationV1(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> saveServerConfigurationV1(const std::vector<std::string>& parameters,
                                                                                                const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getDatabaseVersionV1(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> getExternalConfigurationV1(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> saveExternalConfigurationV1(const std::vector<std::string>& parameters,
                                                                                                  const std::string& requestContent) const;

            // REST Api v2
            boost::shared_ptr<IAnswer> getServerConfigurationV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> saveServerConfigurationV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> resetServerConfigurationV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> getExternalConfigurationV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> saveExternalConfigurationV2(const boost::shared_ptr<IRequest>& request) const;

            boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;
            static std::string m_restKeyword;

            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
