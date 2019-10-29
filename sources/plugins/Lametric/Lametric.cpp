#include "stdafx.h"
#include "Lametric.h"
#include <plugin_cpp_api/ImplementationHelper.h>

#include <Poco/Net/HTTPSClientSession.h>
#include <shared/plugin/yPluginApi/historization/MessageFormatter.h>
#include <shared/Log.h>

IMPLEMENT_PLUGIN(CLametric)

const std::string CLametric::DeviceName("Lametric display");
const std::string CLametric::TextKeywordName("Message");

// Event IDs
enum
{
   kCustomEvent = yApi::IYPluginApi::kPluginFirstEventId,
   kConnectionRetryTimer,
   kAnswerTimeout
};

CLametric::CLametric()
   : m_text(boost::make_shared<yApi::historization::CText>(TextKeywordName,
                                                           yApi::EKeywordAccessMode::kGetSet))
{
}

void CLametric::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "CLametric is starting...";
   DeviceInformation deviceInformation;
   try
   {
      m_configuration.initializeWith(api->getConfiguration());
      m_lametricManager = boost::make_shared<CUrlManager>(m_configuration);

      // First connexion
      api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot,
                                         boost::posix_time::seconds(00));
      m_refreshTimer = api->getEventHandler().createTimer(kAnswerTimeout, shared::event::CEventTimer::kPeriodic,
                                                          boost::posix_time::seconds(10));
      m_refreshTimer->stop();

      api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");
   }
   catch (...)
   {
      api->setPluginState(yApi::historization::EPluginState::kCustom, "initializationError");
      YADOMS_LOG(error) << "Lametric plugin initialization error...";
   }

   // the main loop
   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested";
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }

      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            try
            {
               api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
               api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot,
                                                  boost::posix_time::seconds(30));
            }
            catch (...)
            {
               YADOMS_LOG(information) << "Wrong configuration update";
            }
            break;
         }

      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            const auto command =
               api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            YADOMS_LOG(information) << "Command received from Yadoms : " << yApi::IDeviceCommand::toString(command);

            if (boost::iequals(command->getDevice(), deviceInformation.deviceName))
            {
               m_lametricManager->displayText(command->getBody());
            }

            break;
         }

      case kConnectionRetryTimer:
         {
            initLametric(api, deviceInformation);
            break;
         }
      case kAnswerTimeout:
         {
            api->setPluginState(yApi::historization::EPluginState::kCustom, "failedToConnect");
            m_refreshTimer->stop();
            YADOMS_LOG(error) << "No answer received, try to reconnect in a while...";
            api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot,
                                               boost::posix_time::seconds(30));
            break;
         }
      default:
         {
            YADOMS_LOG(error) << "Unknown or unsupported message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

void CLametric::declareDevice(boost::shared_ptr<yApi::IYPluginApi>& api, DeviceInformation& deviceInformation)
{
   if (!api->deviceExists(deviceInformation.deviceName))
      api->declareDevice(deviceInformation.deviceName, deviceInformation.deviceType, deviceInformation.deviceModel);
}

void CLametric::declareKeyword(boost::shared_ptr<yApi::IYPluginApi>& api, DeviceInformation& deviceInformation) const
{
   // TODO : Declare icon & priority keywords
   if (!api->keywordExists(deviceInformation.deviceName, m_text))
      api->declareKeyword(deviceInformation.deviceName, m_text);
}

void CLametric::fillDeviceInformation(DeviceInformation& deviceInformation) const
{
   deviceInformation.deviceName = "Lametric";
   deviceInformation.deviceModel = m_lametricManager->getDeviceState().get<std::string>("model");
   deviceInformation.deviceType = m_lametricManager->getDeviceState().get<std::string>("name");
}


void CLametric::initLametric(boost::shared_ptr<yApi::IYPluginApi>& api, DeviceInformation& deviceInformation) const
{
   YADOMS_LOG(information) << "Init the connexion ...";

   try
   {
      const auto response = m_lametricManager->getWifiState();
      m_refreshTimer->start();
      fillDeviceInformation(deviceInformation);

      declareDevice(api, deviceInformation);

      declareKeyword(api, deviceInformation);
   }
   catch (std::exception& e)
   {
      api->getEventHandler().createTimer(kAnswerTimeout, shared::event::CEventTimer::kOneShot,
                                         boost::posix_time::seconds(0));
      const auto message = (boost::format("Failed to send GET http request \"%1%\"") % e.
         what()).str();
      YADOMS_LOG(error) << message;
   }
}

void CLametric::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                      const shared::CDataContainer& newConfigurationData)
{
   const auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
   YADOMS_LOG(information) << "Update configuration...";

   m_configuration.initializeWith(newConfiguration);
   m_lametricManager = boost::make_shared<CUrlManager>(m_configuration);

   m_configuration.trace();
}
