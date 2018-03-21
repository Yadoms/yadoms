#pragma once
#include "IRestService.h"
#include "dataAccessLayer/IConfigurationManager.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CConfiguration : public IRestService
         {
         public:
            explicit CConfiguration(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager);
            virtual ~CConfiguration();

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
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

            boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;
            static std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
