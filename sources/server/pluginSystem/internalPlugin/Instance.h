#pragma once
#include <shared/plugin/information/IInformation.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IDeviceCommand.h>
#include <shared/plugin/yPluginApi/IExtraQuery.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/event/EventHandler.hpp>
#include "../InstanceStateHandler.h"
#include "../IInstance.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace pluginSystem
{
   namespace internalPlugin
   {
      //--------------------------------------------------------------
      /// \brief	this class is used to manage a plugin instance. 
      //--------------------------------------------------------------
      class CInstance : public IInstance
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                           Constructor
         /// \param[in] instanceInformation     Instance information
         /// \param[in] pluginInformation       Plugin information
         /// \param[in] api              yPluginApi context instance
         /// \param[in] instanceStateHandler    Instance state handler
         //--------------------------------------------------------------
         CInstance(boost::shared_ptr<const database::entities::CPlugin> instanceInformation,
                   boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                   boost::shared_ptr<yApi::IYPluginApi> api,
                   boost::shared_ptr<CInstanceStateHandler> instanceStateHandler);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CInstance() = default;

         // IInstance Implementation
         void updateConfiguration(const shared::CDataContainer& newConfiguration) override;
         void requestStop() override;
         void kill() override;
         boost::shared_ptr<const database::entities::CPlugin> about() const override;
         boost::shared_ptr<const shared::plugin::information::IInformation> aboutPlugin() const override;
         void postDeviceCommand(boost::shared_ptr<const yApi::IDeviceCommand> deviceCommand) override;
         void postExtraQuery(boost::shared_ptr<yApi::IExtraQuery> extraQuery, const std::string& taskId) override;
         void postBindingQueryRequest(boost::shared_ptr<yApi::IBindingQueryRequest> request) override;
         void postDeviceConfigurationSchemaRequest(boost::shared_ptr<yApi::IDeviceConfigurationSchemaRequest> request) override;
         void postManuallyDeviceCreationRequest(boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> request) override;
         void postSetDeviceConfiguration(boost::shared_ptr<const yApi::ISetDeviceConfiguration> command) override;
         void postDeviceRemoved(boost::shared_ptr<const yApi::IDeviceRemoved> event) override;
         // [END] IInstance Implementation

      protected:
         void doWorkThread(boost::shared_ptr<yApi::IYPluginApi> api,
                           boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                           boost::shared_ptr<CInstanceStateHandler> instanceStateHandler) const;

         void doWork(boost::shared_ptr<yApi::IYPluginApi> api,
                     shared::event::CEventHandler& eventHandler) const;

         void createVirtualDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const yApi::IManuallyDeviceCreationData& data) const;
         void createStandardCapacityDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                           const std::string& deviceName,
                                           const shared::CDataContainer& standardCapacity) const;
         void createCustomEnumCapacityDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                             const std::string& deviceName,
                                             const std::string& commaSeparatedValues) const;

      private:
         boost::shared_ptr<const database::entities::CPlugin> m_instanceInformation;
         boost::shared_ptr<const shared::plugin::information::IInformation> m_pluginInformation;
         boost::shared_ptr<shared::event::CEventHandler> m_eventHandler;
         boost::thread m_thread;
      };
   }
} // namespace pluginSystem::internalPlugin


