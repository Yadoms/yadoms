#include "stdafx.h"
#include "Instance.h"
#include "CustomEnumHistorizer.h"
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

            while (true)
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
                        YADOMS_LOG(information) << "Received command for virtual device from Yadoms : " << yApi::IDeviceCommand::toString(command);
                     }

                     break;
                  }

               case yApi::IYPluginApi::kEventManuallyDeviceCreation:
                  {
                     auto request = eventHandler.getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
                     YADOMS_LOG(information) << "Manually device creation request received for device : " << request->getData().getDeviceName();
                     try
                     {
                        createVirtualDevice(api,
                                            request->getData());
                        request->sendSuccess();
                     }
                     catch (std::exception& e)
                     {
                        YADOMS_LOG(error) << "Unable to create virtual device " << request->getData().getDeviceName() << ", " << e.what();
                        request->sendError("Virtual device creation failed");
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

      void CInstance::createVirtualDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                          const yApi::IManuallyDeviceCreationData& data) const
      {
         if (data.getDeviceType() != "virtualDeviceType")
            throw std::invalid_argument("Wrong device type");

         std::string standardCapacity;
         bool isStandardCapacity;
         try
         {
            isStandardCapacity = data.getConfiguration().get<std::string>("capacity.activeSection") == "standardCapacity";
            boost::shared_ptr<const yApi::historization::IHistorizable> keyword;
            if (isStandardCapacity)
               createStandardCapacityDevice(api,
                                            data.getDeviceName(),
                                            data.getConfiguration().get<std::string>("capacity.content.standardCapacity.content.capacity"));
            else
               createCustomEnumCapacityDevice(api,
                                              data.getDeviceName(),
                                              data.getConfiguration().get<std::string>("capacity.content.customEnumCapacity.content.valueList"));
         }
         catch (std::exception& e)
         {
            throw std::invalid_argument(std::string("Unable to create virtual device : ") + e.what());
         }
      }

      void CInstance::createStandardCapacityDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                   const std::string& deviceName,
                                                   const std::string& standardCapacity) const
      {
         boost::shared_ptr<const yApi::historization::IHistorizable> keyword;

         if (standardCapacity == yApi::CStandardCapacities::ApparentPower().getName())
            keyword = boost::make_shared<yApi::historization::CApparentPower>("ApparentPower", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::ArmingAlarm().getName())
            keyword = boost::make_shared<yApi::historization::CArmingAlarm>("armAlarm", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::BatteryLevel().getName())
            keyword = boost::make_shared<yApi::historization::CBatteryLevel>("battery", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::CameraMove().getName())
            keyword = boost::make_shared<yApi::historization::CCameraMove>("camera", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Counter().getName())
            keyword = boost::make_shared<yApi::historization::CCounter>("counter", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Current().getName())
            keyword = boost::make_shared<yApi::historization::CCurrent>("current", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Curtain().getName())
            keyword = boost::make_shared<yApi::historization::CCurtain>("state");
         else if (standardCapacity == yApi::CStandardCapacities::Dimmable().getName())
            keyword = boost::make_shared<yApi::historization::CDimmable>("state", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Direction().getName())
            keyword = boost::make_shared<yApi::historization::CDirection>("uv", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Distance().getName())
            keyword = boost::make_shared<yApi::historization::CDistance>("distance", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Duration().getName())
            keyword = boost::make_shared<yApi::historization::CDuration>("duration", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Energy().getName())
            keyword = boost::make_shared<yApi::historization::CEnergy>("energy", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Event().getName())
            keyword = boost::make_shared<yApi::historization::CEvent>("event", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Frequency().getName())
            keyword = boost::make_shared<yApi::historization::CFrequency>("frequency", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Humidity().getName())
            keyword = boost::make_shared<yApi::historization::CHumidity>("humidity", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Illumination().getName())
            keyword = boost::make_shared<yApi::historization::CIllumination>("illumination", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Load().getName())
            keyword = boost::make_shared<yApi::historization::CLoad>("load", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Power().getName())
            keyword = boost::make_shared<yApi::historization::CPower>("power", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::PowerFactor().getName())
            keyword = boost::make_shared<yApi::historization::CPowerFactor>("powerFactor", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Pressure().getName())
            keyword = boost::make_shared<yApi::historization::CPressure>("pressure", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Rain().getName())
            keyword = boost::make_shared<yApi::historization::CRain>("rain", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::RainRate().getName())
            keyword = boost::make_shared<yApi::historization::CRainRate>("rainRate", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Rssi().getName())
            keyword = boost::make_shared<yApi::historization::CRssi>("rssi", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Speed().getName())
            keyword = boost::make_shared<yApi::historization::CSpeed>("speed", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Switch().getName())
            keyword = boost::make_shared<yApi::historization::CSwitch>("state", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Temperature().getName())
            keyword = boost::make_shared<yApi::historization::CTemperature>("temperature", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Text().getName())
            keyword = boost::make_shared<yApi::historization::CText>("text", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::UpDownStop().getName())
            keyword = boost::make_shared<yApi::historization::CUpDownStop>("UpDownStop", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Uv().getName())
            keyword = boost::make_shared<yApi::historization::CUv>("uv", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Voltage().getName())
            keyword = boost::make_shared<yApi::historization::CVoltage>("voltage", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::WeatherCondition().getName())
            keyword = boost::make_shared<yApi::historization::CWeatherCondition>("weatherCondition", yApi::EKeywordAccessMode::kGetSet);
         else if (standardCapacity == yApi::CStandardCapacities::Weight().getName())
            keyword = boost::make_shared<yApi::historization::CWeight>("weight", yApi::EKeywordAccessMode::kGetSet);

         if (!keyword)
            throw std::invalid_argument("Unsupported device type");

         api->declareKeyword(deviceName,
                             keyword);
      }

      void CInstance::createCustomEnumCapacityDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                     const std::string& deviceName,
                                                     const std::string& commaSeparatedValues) const
      {
         std::vector<std::string> enumValues;
         boost::split(enumValues, commaSeparatedValues, boost::is_any_of(","));
         for (auto& enumValue : enumValues)
            boost::trim(enumValue);

         auto keyword = boost::make_shared<CCustomEnumHistorizer>("state",
                                                                  yApi::EKeywordAccessMode::kGetSet,
                                                                  enumValues);

         api->declareKeyword(deviceName,
                             keyword);
      }
   }
} // namespace pluginSystem::internalPlugin


