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

         public:
            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation


            static const std::string& getRestKeyword();

         public:
            boost::shared_ptr<shared::serialization::IDataSerializable> getConfiguration(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getSectionConfigurations(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllConfigurations(const std::vector<std::string>& parameters,
                                                                                             const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> createOneConfiguration(const std::vector<std::string>& parameters
                                                                                             , const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updateOneConfiguration(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updateAllConfigurations(const std::vector<std::string>& parameters,
                                                                                                const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteOneConfiguration(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const;

         private:
            boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;
            static std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


