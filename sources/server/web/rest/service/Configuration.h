#pragma once
#include "IRestService.h"
#include "dataAccessLayer/IConfigurationManager.h"
#include "web/rest/ResultV2.h"

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
            void configurePocoDispatcher(CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

         private:
            boost::shared_ptr<shared::serialization::IDataSerializable> resetServerConfiguration(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getServerConfiguration(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> saveServerConfiguration(const std::vector<std::string>& parameters,
                                                                                                const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getDatabaseVersion(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> getExternalConfiguration(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> saveExternalConfiguration(const std::vector<std::string>& parameters,
                                                                                                  const std::string& requestContent) const;

            // REST Api v2
            boost::shared_ptr<IRestAnswer> getServerConfigurationV2(boost::shared_ptr<IRestRequest> request) const;
            boost::shared_ptr<IRestAnswer> saveServerConfigurationV2(const boost::shared_ptr<IRestRequest>& request) const;
            boost::shared_ptr<IRestAnswer> resetServerConfigurationV2(boost::shared_ptr<IRestRequest> request) const;
            boost::shared_ptr<IRestAnswer> getDatabaseVersionV2(boost::shared_ptr<IRestRequest> request) const;
            boost::shared_ptr<IRestAnswer> getExternalConfigurationV2(const boost::shared_ptr<IRestRequest>& request) const;
            boost::shared_ptr<IRestAnswer> saveExternalConfigurationV2(const boost::shared_ptr<IRestRequest>& request) const;

            boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;
            static std::string m_restKeyword;

            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
