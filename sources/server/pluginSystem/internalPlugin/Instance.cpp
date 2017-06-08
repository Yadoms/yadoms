#include "stdafx.h"
#include "Instance.h"
#include <shared/Log.h>
#include <tools/OperatingSystem.h>

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
           m_eventHandler(boost::make_shared<shared::event::CEventHandler>()), //TODO utile ? Ne peut-on pas utiliser api->GetEventHandler() ?
           m_thread(&CInstance::doWorkThread, this, api, m_eventHandler, instanceStateHandler)
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

      void CInstance::postExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> extraQuery, const std::string& taskId)
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

      void CInstance::doWorkThread(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api,
                                   boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                                   boost::shared_ptr<CInstanceStateHandler> instanceStateHandler) const
      {
         instanceStateHandler->onStart();
         doWork(api, *eventHandler);
         instanceStateHandler->onFinish(0, std::string());
      }

      void CInstance::doWork(boost::shared_ptr<yApi::IYPluginApi> api,
                             shared::event::CEventHandler& eventHandler) const
      {
         try
         {
            YADOMS_LOG_CONFIGURE("InternalPlugin");

            // the main loop
            YADOMS_LOG(debug) << "InternalPlugin is running...";

            // Declare all device/keywords
            static const std::string& systemDevice("system");
            auto keywordShutdown(boost::make_shared<yApi::historization::CEvent>("shutdown"));
            auto keywordRestart(boost::make_shared<yApi::historization::CEvent>("restart"));
            std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> keywords({keywordShutdown, keywordRestart});

            // Device creation if needed
            if (!api->deviceExists(systemDevice))
               api->declareDevice(systemDevice,
                                  "system",
                                  "yadoms system",
                                  keywords);

            while (1)
            {
               switch (eventHandler.waitForEvents())
               {
               case yApi::IYPluginApi::kEventDeviceCommand:
                  {
                     auto command = eventHandler.getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();

                     if (boost::iequals(command->getKeyword(), keywordShutdown->getKeyword()))
                     {
                        YADOMS_LOG(information) << "Shutdown the system";
                        if (!tools::COperatingSystem::shutdown())
                        YADOMS_LOG(error) << "Fail to shutdown";
                     }
                     else if (boost::iequals(command->getKeyword(), keywordRestart->getKeyword()))
                     {
                        YADOMS_LOG(information) << "Reboot the system";
                        if (!tools::COperatingSystem::shutdown(true))
                        YADOMS_LOG(error) << "Fail to reboot";
                     }
                     else
                     {
                        YADOMS_LOG(warning) << "Received command for unknown keyword from Yadoms : " << yApi::IDeviceCommand::toString(command);
                     }
                     break;
                  }

               case yApi::IYPluginApi::kEventManuallyDeviceCreation:
                  {
                     auto request = eventHandler.getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
                     YADOMS_LOG(information) << "Manually device creation request received for device :" << request->getData().getDeviceName();
                     try
                     {
                        request->sendSuccess(createVirtualDevice(api,
                                                                 request->getData()));
                     }
                     catch (std::exception& e)
                     {
                        YADOMS_LOG(error) << "Unable to create virtual device " << request->getData().getDeviceName() << ", " << e.what();
                        request->sendError("Unable to create virtual device");
                     }

                     break;
                  }

               default:
                  break;
               }
            }
         }
         catch (boost::thread_interrupted&)
         {
            YADOMS_LOG(information) << "Thread is stopping...";
         }
         catch (shared::exception::CException& ex)
         {
            YADOMS_LOG(fatal) << "The Yadoms Sytem plugin fails. Unknown expcetion : " << ex.what();
         }
      }

      std::string CInstance::createVirtualDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                 const yApi::IManuallyDeviceCreationData& data) const
      {
         if (api->deviceExists(data.getDeviceName()))
            throw std::invalid_argument("device already exists");

         boost::shared_ptr<const yApi::historization::IHistorizable> keyword;
         if (data.getDeviceType() == "switch") //TODO ne pas laisser en dur
            keyword = boost::make_shared<yApi::historization::CSwitch>("state");

         if (!keyword)
            throw std::invalid_argument("Unsupported device type");

         api->declareDevice(data.getDeviceName(),
                            data.getDeviceType(),
                            data.getDeviceType(),
                            keyword);

         return data.getDeviceName();
      }
   }
} // namespace pluginSystem::internalPlugin


