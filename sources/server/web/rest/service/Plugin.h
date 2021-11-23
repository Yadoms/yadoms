#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "pluginSystem/Manager.h"
#include "communication/ISendMessageAsync.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CPlugin final : public IRestService
         {
         public:
            CPlugin(boost::shared_ptr<database::IDataProvider> dataProvider,
                    boost::shared_ptr<pluginSystem::CManager> pluginManager,
                    boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
                    communication::ISendMessageAsync& messageSender,
                    bool developerMode);
            ~CPlugin() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

         private:
            const std::string& getRestKeyword() const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getOnePluginV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllPluginsInstanceV1(const std::vector<std::string>& parameters,
                                                                                                const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllPluginsInstanceWithStateV1(const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllPluginsInstanceForManualDeviceCreationV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllAvailablePluginsV1(const std::vector<std::string>& parameters,
                                                                                                 const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllAvailablePluginsParameterizedV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllAvailablePluginsWithPackageV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> sendExtraQuery(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> sendDeviceExtraQueryV1(
               const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> createPluginV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updatePluginV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deletePluginV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteAllPluginsV1(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getInstanceStateV1(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getInstanceRunningV1(
               const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getPluginDevicesV1(const std::vector<std::string>& parameters,
                                                                                           const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> startInstanceV1(const std::vector<std::string>& parameters,
                                                                                        const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> stopInstanceV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getInstanceLogV1(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> createDeviceV1(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> getBindingV1(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethodV1(poco::CRestDispatcher::CRestMethodHandler realMethod,
                                                                                              const std::vector<std::string>& parameters,
                                                                                              const std::string& requestContent) const;

            boost::shared_ptr<IAnswer> getAvailablePluginsV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> getPluginsInstancesV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> createPluginsInstanceV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> updatePluginsInstanceV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> deletePluginsInstanceV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> getPluginsInstancesLogV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> getPluginsInstancesBindingV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> startPluginsInstanceV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> sendExtraQueryToPluginInstanceV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> stopPluginsInstanceV2(const boost::shared_ptr<IRequest>& request) const;


            std::string generateUniqueDeviceName(const int pluginId) const;

            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            boost::shared_ptr<pluginSystem::CManager> m_pluginManager;

            boost::shared_ptr<dataAccessLayer::IDeviceManager> m_deviceManager;

            std::string m_restKeyword;

            communication::ISendMessageAsync& m_messageSender;

            bool m_developerMode;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
