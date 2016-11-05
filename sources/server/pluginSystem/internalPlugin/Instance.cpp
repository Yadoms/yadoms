#include "stdafx.h"
#include "Instance.h"

namespace pluginSystem
{
   namespace internalPlugin
   {
      CInstance::CInstance(boost::shared_ptr<const database::entities::CPlugin> instanceInformation,
                           boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                           boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api,
                           boost::shared_ptr<CInstanceStateHandler> instanceStateHandler)
         : m_instanceInformation(instanceInformation),
           m_pluginInformation(pluginInformation),
           m_eventHandler(boost::make_shared<shared::event::CEventHandler>()),
           m_thread(&CInstance::Thread, api, m_eventHandler, instanceStateHandler)
      {
      }

      CInstance::~CInstance()
      {
      }

      void CInstance::requestStop()
      {
         m_thread.interrupt();
      }

      void CInstance::kill()
      {
         m_thread.interrupt();
         m_thread.join();
      }

      void CInstance::Thread(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api,
                             boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                             boost::shared_ptr<CInstanceStateHandler> instanceStateHandler)
      {
         instanceStateHandler->onStart();

         CSystem system;
         system.doWork(api, *eventHandler);

         instanceStateHandler->onFinish(0, std::string());
      }

      void CInstance::postDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> deviceCommand)
      {
         m_eventHandler->postEvent(yApi::IYPluginApi::kEventDeviceCommand,
                                   deviceCommand);
      }

      void CInstance::postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request)
      {
         m_eventHandler->postEvent(yApi::IYPluginApi::kBindingQuery,
                                   request);
      }

      void CInstance::postDeviceConfigurationSchemaRequest(boost::shared_ptr<shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest> request)
      {
         m_eventHandler->postEvent(yApi::IYPluginApi::kGetDeviceConfigurationSchemaRequest,
                                   request);
      }

      void CInstance::postSetDeviceConfiguration(boost::shared_ptr<const shared::plugin::yPluginApi::ISetDeviceConfiguration> command)
      {
         m_eventHandler->postEvent(yApi::IYPluginApi::kSetDeviceConfiguration,
                                   command);
      }

      void CInstance::postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request)
      {
         m_eventHandler->postEvent(yApi::IYPluginApi::kEventManuallyDeviceCreation,
                                   request);
      }

      void CInstance::postExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> extraQuery)
      {
         m_eventHandler->postEvent(yApi::IYPluginApi::kEventExtraQuery,
                                   extraQuery);
      }

      void CInstance::updateConfiguration(const shared::CDataContainer& newConfiguration)
      {
         m_eventHandler->postEvent(yApi::IYPluginApi::kEventUpdateConfiguration,
                                   newConfiguration);
      }

      boost::shared_ptr<const database::entities::CPlugin> CInstance::about() const
      {
         return m_instanceInformation;
      }

      boost::shared_ptr<const shared::plugin::information::IInformation> CInstance::aboutPlugin() const
      {
         return m_pluginInformation;
      }

      void CInstance::postDeviceRemoved(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceRemoved> event)
      {
         m_eventHandler->postEvent(yApi::IYPluginApi::kEventDeviceRemoved,
                                   event);
      }
   }
} // namespace pluginSystem::internalPlugin


