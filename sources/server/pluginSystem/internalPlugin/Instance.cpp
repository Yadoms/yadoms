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
            static const std::string& SystemDevice("system");
            const auto keywordShutdown(boost::make_shared<yApi::historization::CEvent>("shutdown"));
            const auto keywordRestart(boost::make_shared<yApi::historization::CEvent>("restart"));
            const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> keywords({keywordShutdown, keywordRestart});

            // Device creation if needed
            if (!api->deviceExists(SystemDevice))
               api->declareDevice(SystemDevice,
                                  "system",
                                  "yadoms system",
                                  keywords);

            while (true)
            {
               switch (eventHandler.waitForEvents())
               {
               case yApi::IYPluginApi::kEventDeviceCommand:
                  {
                     const auto command = eventHandler.getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();

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
            YADOMS_LOG(fatal) << "The Yadoms Sytem plugin fails. Unknown exception : " << ex.what();
         }
      }

      void CInstance::createVirtualDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                          const yApi::IManuallyDeviceCreationData& data) const
      {
         if (data.getDeviceType() != "virtualDeviceType")
            throw std::invalid_argument("Wrong device type");

         try
         {
            const auto isStandardCapacity = data.getConfiguration().get<std::string>("capacity.activeSection") == "standardCapacity";
            boost::shared_ptr<const yApi::historization::IHistorizable> keyword;
            if (isStandardCapacity)
               createStandardCapacityDevice(api,
                                            data.getDeviceName(),
                                            data.getConfiguration().get<shared::CDataContainer>("capacity.content.standardCapacity.content.selectCapacity"));
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
                                                   const shared::CDataContainer& standardCapacity) const
      {
         boost::shared_ptr<const yApi::historization::IHistorizable> keyword;

         const auto selectedCapacity = standardCapacity.get<std::string>("activeSection");

         if (selectedCapacity == yApi::CStandardCapacities::ApparentPower().getName())
            keyword = boost::make_shared<yApi::historization::CApparentPower>("ApparentPower", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::ArmingAlarm().getName())
            keyword = boost::make_shared<yApi::historization::CArmingAlarm>("armAlarm", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::BatteryLevel().getName())
            keyword = boost::make_shared<yApi::historization::CBatteryLevel>("battery", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::CameraMove().getName())
            keyword = boost::make_shared<yApi::historization::CCameraMove>("camera", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::ColorRGB().getName())
            keyword = boost::make_shared<yApi::historization::CColorRGB>("ColorRGB", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::ColorRGBW().getName())
            keyword = boost::make_shared<yApi::historization::CColorRGBW>("ColorRGBW", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Counter().getName())
            keyword = boost::make_shared<yApi::historization::CCounter>("counter",
               yApi::EKeywordAccessMode::kGetSet,
               yApi::historization::EMeasureType(standardCapacity.get<std::string>("content." + yApi::CStandardCapacities::Counter().getName() + ".content.measureType")));
         else if (selectedCapacity == yApi::CStandardCapacities::Current().getName())
            keyword = boost::make_shared<yApi::historization::CCurrent>("current", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Curtain().getName())
            keyword = boost::make_shared<yApi::historization::CCurtain>("state");
         else if (selectedCapacity == yApi::CStandardCapacities::Dimmable().getName())
            keyword = boost::make_shared<yApi::historization::CDimmable>("state", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Direction().getName())
            keyword = boost::make_shared<yApi::historization::CDirection>("direction", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Distance().getName())
            keyword = boost::make_shared<yApi::historization::CDistance>("distance", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Duration().getName())
            keyword = boost::make_shared<yApi::historization::CDuration>("duration", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Energy().getName())
            keyword = boost::make_shared<yApi::historization::CEnergy>("energy",
               yApi::EKeywordAccessMode::kGetSet,
               yApi::historization::EMeasureType(standardCapacity.get<std::string>("content." + yApi::CStandardCapacities::Energy().getName() + ".content.measureType")));
         else if (selectedCapacity == yApi::CStandardCapacities::Event().getName())
            keyword = boost::make_shared<yApi::historization::CEvent>("event", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Frequency().getName())
            keyword = boost::make_shared<yApi::historization::CFrequency>("frequency", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Humidity().getName())
            keyword = boost::make_shared<yApi::historization::CHumidity>("humidity", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Illumination().getName())
            keyword = boost::make_shared<yApi::historization::CIllumination>("illumination", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Load().getName())
            keyword = boost::make_shared<yApi::historization::CLoad>("load", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Power().getName())
            keyword = boost::make_shared<yApi::historization::CPower>("power", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::PowerFactor().getName())
            keyword = boost::make_shared<yApi::historization::CPowerFactor>("powerFactor", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Pressure().getName())
            keyword = boost::make_shared<yApi::historization::CPressure>("pressure", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Rain().getName())
            keyword = boost::make_shared<yApi::historization::CRain>("rain",
               yApi::EKeywordAccessMode::kGetSet,
               yApi::historization::EMeasureType(standardCapacity.get<std::string>("content." + yApi::CStandardCapacities::Rain().getName() + ".content.measureType")));
         else if (selectedCapacity == yApi::CStandardCapacities::RainRate().getName())
            keyword = boost::make_shared<yApi::historization::CRainRate>("rainRate", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Rssi().getName())
            keyword = boost::make_shared<yApi::historization::CRssi>("rssi", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Speed().getName())
            keyword = boost::make_shared<yApi::historization::CSpeed>("speed", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Switch().getName())
            keyword = boost::make_shared<yApi::historization::CSwitch>("state", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Temperature().getName())
            keyword = boost::make_shared<yApi::historization::CTemperature>("temperature", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Text().getName())
            keyword = boost::make_shared<yApi::historization::CText>("text", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::UpDownStop().getName())
            keyword = boost::make_shared<yApi::historization::CUpDownStop>("UpDownStop", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Uv().getName())
            keyword = boost::make_shared<yApi::historization::CUv>("uv", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Voltage().getName())
            keyword = boost::make_shared<yApi::historization::CVoltage>("voltage", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Volume().getName())
            keyword = boost::make_shared<yApi::historization::CVolume>("volume", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::WeatherCondition().getName())
            keyword = boost::make_shared<yApi::historization::CWeatherCondition>("weatherCondition", yApi::EKeywordAccessMode::kGetSet);
         else if (selectedCapacity == yApi::CStandardCapacities::Weight().getName())
            keyword = boost::make_shared<yApi::historization::CWeight>("weight", yApi::EKeywordAccessMode::kGetSet);

         if (!keyword)
            throw std::invalid_argument("Unsupported device type");

         api->declareKeyword(deviceName,
                             keyword);

         if (selectedCapacity != yApi::CStandardCapacities::Event().getName())
         {
            // Historize the default value (already set in historizer), except for event (event has no value)
            api->historizeData(deviceName,
                               keyword);
         }
      }

      void CInstance::createCustomEnumCapacityDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                     const std::string& deviceName,
                                                     const std::string& commaSeparatedValues) const
      {
         std::vector<std::string> enumValues;
         boost::split(enumValues, commaSeparatedValues, boost::is_any_of(","));
         for (auto& enumValue : enumValues)
            boost::trim(enumValue);

         const auto keyword = boost::make_shared<CCustomEnumHistorizer>("state",
                                                                  yApi::EKeywordAccessMode::kGetSet,
                                                                  enumValues);

         api->declareKeyword(deviceName,
                             keyword);

         // Historize the default value (already set in historizer)
         api->historizeData(deviceName,
                            keyword);
      }
   }
} // namespace pluginSystem::internalPlugin


