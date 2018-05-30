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
         class CPlugin : public IRestService
         {
         public:
            explicit CPlugin(boost::shared_ptr<database::IDataProvider> dataProvider,
                             boost::shared_ptr<pluginSystem::CManager> pluginManager,
                             boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
                             communication::ISendMessageAsync& messageSender);
            virtual ~CPlugin();

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            const std::string& getRestKeyword() const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getOnePlugin(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllPluginsInstance(const std::vector<std::string>& parameters,
                                                                                              const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllPluginsInstanceForManualDeviceCreation(const std::vector<std::string>& parameters,
                                                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllAvailablePlugins(const std::vector<std::string>& parameters,
                                                                                               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllAvailablePluginsWithPackage(const std::vector<std::string>& parameters,
                                                                                                          const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> sendExtraQuery(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> createPlugin(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> updatePlugin(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deletePlugin(const std::vector<std::string>& parameters,
                                                                                     const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteAllPlugins(const std::vector<std::string>& parameters,
                                                                                         const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getInstanceState(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getInstanceRunning(const std::vector<std::string>& parameters, const std::string& requestContent) const;
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

            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod,
                                                                                            const std::vector<std::string>& parameters,
                                                                                            const std::string& requestContent) const;

         protected:
            std::string generateUniqueDeviceName(const int pluginId) const;

         private:
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            boost::shared_ptr<pluginSystem::CManager> m_pluginManager;

            //-----------------------------------------
            ///\brief   Device manager
            //-----------------------------------------
            boost::shared_ptr<dataAccessLayer::IDeviceManager> m_deviceManager;

            std::string m_restKeyword;

            //-----------------------------------------
            ///\brief   The send message interface
            //-----------------------------------------
            communication::ISendMessageAsync& m_messageSender;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


