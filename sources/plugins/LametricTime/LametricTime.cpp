#include "stdafx.h"
#include "LametricTime.h"
#include <plugin_cpp_api/ImplementationHelper.h>

#include <Poco/Net/HTTPSClientSession.h>
#include <shared/Log.h>
#include <shared/http/Codes.h>
#include <shared/exception/HttpException.hpp>
#include "DeviceState.h"
#include "NotificationSender.h"
#include "CFactory.h"

IMPLEMENT_PLUGIN(CLametricTime)

const std::string CLametricTime::DeviceName("LaMetric Time");
const std::string CLametricTime::TextKeywordName("Message");
const std::string CLametricTime::IconTypeName("iconType");

// Event IDs
enum
{
   kCustomEvent = yApi::IYPluginApi::kPluginFirstEventId,
   kConnectionRetryTimer,
   kAnswerTimeout
};

CLametricTime::CLametricTime()
   : m_text(boost::make_shared<yApi::historization::CText>(TextKeywordName,
                                                           yApi::EKeywordAccessMode::kGetSet)),
     m_iconType(boost::make_shared<specificHistorizers::CCustomizeIconType>(IconTypeName)),
     m_deviceInformation(boost::make_shared<DeviceInformation>())
{
}

void CLametricTime::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   m_api = api;
   YADOMS_LOG(information) << "LametricTime is starting...";

   m_configuration.initializeWith(api->getConfiguration());

   init();

   // the main loop
   while (true)
   {
      // Wait for an event
      switch (m_api->getEventHandler().waitForEvents())
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
               m_api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               onUpdateConfiguration(
                  m_api->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>>());

               retryConnection(30);

               m_configuration.getPairingMode() == kAuto
                  ? m_devicesInformation.clear()
                  : m_deviceInformation.reset();
               init();
            }
            catch (...)
            {
               YADOMS_LOG(information) << "Wrong configuration update";
            }
            break;
         }

      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            if (m_configuration.getPairingMode() == kAuto)
            {
               if (m_devicesInformation.empty())
                  break;
            }
            else
            {
               if (!m_deviceInformation)
                  break;
            }

            const auto command =
               m_api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            YADOMS_LOG(information) << "Command received from Yadoms : " << yApi::IDeviceCommand::toString(command);

            if (m_configuration.getPairingMode() == kAuto && command->getKeyword() != TextKeywordName)
            {
               m_targetDevice = std::find_if(m_devicesInformation.begin(), m_devicesInformation.end(),
                                             [&command](const auto& deviceInformation)
                                             {
                                                return deviceInformation.m_deviceName == command->getDevice();
                                             });
               if (m_targetDevice != std::end(m_devicesInformation))
               {
                  YADOMS_LOG(information) << "Target device found";
                  m_configuration.setIPAddress(m_targetDevice->m_deviceIp);
                  m_deviceManager = CFactory::createDeviceState(m_configuration);
                  m_senderManager = CFactory::createNotificationSender(m_configuration);
               }
               else
               {
                  YADOMS_LOG(error) << "Target Device not found";
               }
            }

            auto commandBody = command->getBody();
            if (command->getKeyword() == IconTypeName)
            {
               m_senderManager->setCustomizeIcon(commandBody);
            }
            if (boost::iequals(command->getDevice(), m_configuration.getPairingMode() == kAuto
                                                        ? m_targetDevice->m_deviceName
                                                        : m_deviceInformation->m_deviceName) && command->getKeyword() !=
               IconTypeName)
            {
               m_senderManager->displayText(commandBody);
            }

            break;
         }

      case kConnectionRetryTimer:
         {
            m_configuration.getPairingMode() == kAuto
               ? m_devicesInformation.clear()
               : m_deviceInformation.reset();

            //init(); //TODO remove ?
            retryInitManually();

            break;
         }
      case kAnswerTimeout:
         {
            m_api->setPluginState(yApi::historization::EPluginState::kCustom, "failedToConnect");
            YADOMS_LOG(error) << "No answer received, try to reconnect in a while...";
            retryConnection(30);
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

void CLametricTime::declareDevice() const
{
   YADOMS_LOG(information) << "Creating the device :" << m_deviceInformation->m_deviceName;
   if (!m_api->deviceExists(m_deviceInformation->m_deviceName))
      m_api->declareDevice(m_deviceInformation->m_deviceName, m_deviceInformation->m_deviceType,
                           m_deviceInformation->m_deviceModel);
}

void CLametricTime::declareAllDevicesAndKeywords(std::vector<DeviceInformation>& devicesInformation) const
{
   for (const auto& deviceInformation : devicesInformation)
   {
      YADOMS_LOG(information) << "Creating the device :" << deviceInformation.m_deviceName;
      if (!m_api->deviceExists(deviceInformation.m_deviceName))
         m_api->declareDevice(deviceInformation.m_deviceName, deviceInformation.m_deviceType,
                              deviceInformation.m_deviceModel);
      if (!m_api->keywordExists(deviceInformation.m_deviceName, m_text))
         m_api->declareKeyword(deviceInformation.m_deviceName, m_text);
      if (!m_api->keywordExists(deviceInformation.m_deviceName, m_iconType))
         m_api->declareKeyword(deviceInformation.m_deviceName, m_iconType);
   }
}

void CLametricTime::declareKeyword() const
{
   YADOMS_LOG(information) << "Declaring the keyword :" << m_text;

   if (!m_api->keywordExists(m_deviceInformation->m_deviceName, m_text))
      m_api->declareKeyword(m_deviceInformation->m_deviceName, m_text);
   if (!m_api->keywordExists(m_deviceInformation->m_deviceName, m_iconType))
      m_api->declareKeyword(m_deviceInformation->m_deviceName, m_iconType);
}

void CLametricTime::fillDeviceInformationManually() const
{
   if (!m_deviceInformation)
      m_deviceInformation = boost::make_shared<DeviceInformation>();

   m_deviceInformation->m_deviceName = DeviceName;
   m_deviceInformation->m_deviceModel = m_deviceManager->getDeviceInformations()->get<std::string>("model");
   m_deviceInformation->m_deviceType = m_deviceManager->getDeviceInformations()->get<std::string>("name");
}


std::vector<DeviceInformation> CLametricTime::fillAllDevicesInformationAutomatically(
   const std::vector<boost::shared_ptr<shared::http::ssdp::IDiscoveredDevice>>& foundDevices)
{
   std::vector<DeviceInformation> devicesInformation;
   DeviceInformation deviceInformation;
   for (const auto& foundDevice : foundDevices)
   {
      deviceInformation.m_deviceIp = getIpAddress(foundDevice->xmlContent()->get<std::string>("root.URLBase"));
      deviceInformation.m_deviceName = foundDevice->xmlContent()->get<std::string>("root.device.modelName") + " " +
         deviceInformation.m_deviceIp;
      deviceInformation.m_deviceModel = foundDevice->xmlContent()->get<std::string>("root.device.friendlyName");
      deviceInformation.m_deviceType = foundDevice->xmlContent()->get<std::string>("root.device.modelName");
      devicesInformation.push_back(deviceInformation);
   }

   return devicesInformation;
}


void CLametricTime::init()
{
   if (m_configuration.getPairingMode() == kAuto)
   {
      m_devicesInformation = initAutomatically();
   }
   else
   {
      initManually();
   }
}

std::vector<DeviceInformation> CLametricTime::initAutomatically() const
{
   try
   {
      const auto foundDevices = shared::http::ssdp::CDiscoverService::discover("urn:schemas-upnp-org:device:LaMetric:1",
                                                                               std::chrono::seconds(15));
      if (foundDevices.empty())
      {
         m_api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
         throw std::runtime_error("No response from the device: wrong ip or no device listening on this network");
      }

      auto devicesInformation = fillAllDevicesInformationAutomatically(foundDevices);

      declareAllDevicesAndKeywords(devicesInformation);

      m_api->setPluginState(yApi::historization::EPluginState::kRunning);

      return devicesInformation;
   }
   catch (const std::exception& exception)
   {
      YADOMS_LOG(error) << exception.what();
      m_api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
      throw;
   }
}

void CLametricTime::initManually()
{
   YADOMS_LOG(information) << "Init the connection ...";

   m_deviceManager = CFactory::createDeviceState(m_configuration);
   m_senderManager = CFactory::createNotificationSender(m_configuration);

   try
   {
      createDevice();
   }
   catch (shared::exception::CHttpException& e)
   {
      if (e.code() == shared::http::ECodes::kUnauthorized)
      {
         YADOMS_LOG(error) << e.what();
         m_api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
         throw;
      }

      YADOMS_LOG(error) << e.what();
      retryConnection(10);
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << e.what();
      retryConnection(10);
   }
}

void CLametricTime::onUpdateConfiguration(
   const boost::shared_ptr<shared::CDataContainer>& newConfigurationData)
{
   const auto newConfiguration = m_api->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>>();
   YADOMS_LOG(information) << "Update configuration...";

   m_configuration.initializeWith(newConfiguration);
   m_configuration.trace();
}

void CLametricTime::retryInitManually()
{
   YADOMS_LOG(information) << "Init the connection ...";

   m_deviceManager = CFactory::createDeviceState(m_configuration);
   m_senderManager = CFactory::createNotificationSender(m_configuration);

   try
   {
      createDevice();
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << e.what();

      m_api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
      throw;
   }
}

void CLametricTime::createDevice() const
{
   m_deviceManager->getDeviceState();

   fillDeviceInformationManually();

   declareDevice();

   declareKeyword();

   m_api->setPluginState(yApi::historization::EPluginState::kRunning);
}

void CLametricTime::retryConnection(int withinDelaySeconds) const
{
   m_api->getEventHandler().createTimer(kConnectionRetryTimer,
                                        shared::event::CEventTimer::kOneShot,
                                        boost::posix_time::seconds(withinDelaySeconds));
}

std::string CLametricTime::getIpAddress(const std::string& urlBase)
{
   const boost::regex reg(R"((\d{1,3}(\.\d{1,3}){3}))");
   boost::smatch match;
   if (!regex_search(urlBase, match, reg) || match.length() < 2)
      throw std::runtime_error("Invalid IP found");

   return match[1].str();
}
