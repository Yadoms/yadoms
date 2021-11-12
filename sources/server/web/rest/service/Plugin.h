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
            boost::shared_ptr<shared::serialization::IDataSerializable> getOnePlugin(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllPluginsInstance(const std::vector<std::string>& parameters,
                                                                                              const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllPluginsInstanceWithState(const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllPluginsInstanceForManualDeviceCreation(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllAvailablePlugins(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllAvailablePluginsParameterized(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllAvailablePluginsWithPackage(const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> sendExtraQuery(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> sendDeviceExtraQuery(
               const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> createPlugin(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updatePlugin(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deletePlugin(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteAllPlugins(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getInstanceState(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getInstanceRunning(
               const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getPluginDevices(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> startInstance(const std::vector<std::string>& parameters,
                                                                                      const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> stopInstance(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getInstanceLog(const std::vector<std::string>& parameters,
                                                                                       const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> createDevice(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> getBinding(const std::vector<std::string>& parameters,
                                                                                   const std::string& requestContent) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(poco::CRestDispatcher::CRestMethodHandler realMethod,
                                                                                            const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const;

            boost::shared_ptr<IAnswer> getAvailablePlugins(boost::shared_ptr<IRequest> request) const;
            boost::shared_ptr<IAnswer> getPluginsInstances(boost::shared_ptr<IRequest> request) const;
            boost::shared_ptr<IAnswer> getInstanceDevices(boost::shared_ptr<IRequest> request) const;


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
