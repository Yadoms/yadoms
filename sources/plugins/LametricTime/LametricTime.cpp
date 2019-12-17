#include "stdafx.h"
#include "LametricTime.h"
#include <plugin_cpp_api/ImplementationHelper.h>

#include <Poco/Net/HTTPSClientSession.h>
#include <shared/Log.h>
#include "DeviceState.h"
#include "NotificationSender.h"
#include "CFactory.h"

IMPLEMENT_PLUGIN(CLametricTime)

const std::string CLametricTime::DeviceName("Lametric Time");
const std::string CLametricTime::TextKeywordName("Message");

// Event IDs
enum
{
	kCustomEvent = yApi::IYPluginApi::kPluginFirstEventId,
	kConnectionRetryTimer,
	kAnswerTimeout
};

CLametricTime::CLametricTime()
	: m_text(boost::make_shared<yApi::historization::CText>(TextKeywordName,
	                                                        yApi::EKeywordAccessMode::kGetSet))
{
}

void CLametricTime::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
	YADOMS_LOG(information) << "LametricTime is starting...";

	m_configuration.initializeWith(api->getConfiguration());

	auto deviceInformation = initLametricTime(api);

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
					deviceInformation = initLametricTime(api);
				}
				catch (...)
				{
					YADOMS_LOG(information) << "Wrong configuration update";
				}
				break;
			}

		case yApi::IYPluginApi::kEventDeviceCommand:
			{
				if (!deviceInformation)
					break;

				const auto command =
					api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
				YADOMS_LOG(information) << "Command received from Yadoms : " << yApi::IDeviceCommand::toString(command);

				if (boost::iequals(command->getDevice(), deviceInformation->m_deviceName))
				{
					m_senderManager->displayText(command->getBody());
				}

				break;
			}

		case kConnectionRetryTimer:
			{
				deviceInformation.reset();
				deviceInformation = initLametricTime(api);

				break;
			}
		case kAnswerTimeout:
			{
				api->setPluginState(yApi::historization::EPluginState::kCustom, "failedToConnect");
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

void CLametricTime::declareDevice(boost::shared_ptr<yApi::IYPluginApi>& api,
                                  boost::shared_ptr<DeviceInformation>& deviceInformation)
{
	YADOMS_LOG(information) << "Creating the device :" << deviceInformation->m_deviceName;
	if (!api->deviceExists(deviceInformation->m_deviceName))
		api->declareDevice(deviceInformation->m_deviceName, deviceInformation->m_deviceType,
		                   deviceInformation->m_deviceModel);
}

void CLametricTime::declareKeyword(boost::shared_ptr<yApi::IYPluginApi>& api,
                                   boost::shared_ptr<DeviceInformation>& deviceInformation) const
{
	YADOMS_LOG(information) << "Declaring the keyword :" << m_text;
	
	// TODO : Declare icon & priority keywords
	if (!api->keywordExists(deviceInformation->m_deviceName, m_text))
		api->declareKeyword(deviceInformation->m_deviceName, m_text);
}

void CLametricTime::fillDeviceInformationManually(boost::shared_ptr<DeviceInformation>& deviceInformation) const
{
	deviceInformation->m_deviceName = "Lametric Time";
	deviceInformation->m_deviceModel = m_deviceManager->getDeviceState().get<std::string>("model");
	deviceInformation->m_deviceType = m_deviceManager->getDeviceState().get<std::string>("name");
}


void CLametricTime::fillDeviceInformationAutomatically(CSsdpDiscoveredDevice& foundDevice,
                                                       boost::shared_ptr<DeviceInformation>& deviceInformation)
{
	deviceInformation->m_deviceName = foundDevice.findTag("friendlyName");
	deviceInformation->m_deviceModel = foundDevice.findTag("modelName");
	deviceInformation->m_deviceType = foundDevice.findTag("modelNumber");
}

boost::shared_ptr<DeviceInformation> CLametricTime::initLametricTime(boost::shared_ptr<yApi::IYPluginApi>& api)
{
	YADOMS_LOG(information) << "Init the connection ...";

	if (m_configuration.getPairingMode() != EPairingMode::kAuto)
	{
		m_deviceManager = CFactory::createDeviceState(m_configuration);
		m_senderManager = CFactory::createNotificationSender(m_configuration);
		auto deviceInformation = manualInit(api);

		return deviceInformation;
	}

	auto deviceInformation = automaticInit(api);
	m_deviceManager = CFactory::createDeviceState(m_configuration);
	m_senderManager = CFactory::createNotificationSender(m_configuration);
	return deviceInformation;
}

boost::shared_ptr<DeviceInformation> CLametricTime::automaticInit(boost::shared_ptr<yApi::IYPluginApi>& api)
{
	try
	{
		CSsdpDiscoveredDevice foundDevice;
		if (!CSsdpDiscoverService::discover("urn:schemas-upnp-org:device:LaMetric:1",
		                                    [&foundDevice](const CSsdpDiscoveredDevice& device)
		                                    {
			                                    if (device.findTag("modelName") != "LaMetric Time")
				                                    return false;
			                                    foundDevice = device;
			                                    return true;
		                                    }))
		{
			api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
			throw;
		}
		// Device Found
		auto ip = foundDevice.getIp();

		m_configuration.setIPAddress(ip);

		auto deviceInformation = boost::make_shared<DeviceInformation>();

		fillDeviceInformationAutomatically(foundDevice, deviceInformation);

		declareDevice(api, deviceInformation);

		declareKeyword(api, deviceInformation);

		api->setPluginState(yApi::historization::EPluginState::kRunning);

		return deviceInformation;
	}
	catch (const std::exception& exception)
	{
		//Erreur
		YADOMS_LOG(error) << exception.what();
		api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
		throw;
	}
}

boost::shared_ptr<DeviceInformation> CLametricTime::manualInit(boost::shared_ptr<yApi::IYPluginApi>& api) const
{
	try
	{
		m_deviceManager->getWifiState();

		auto deviceInformation = boost::make_shared<DeviceInformation>();

		fillDeviceInformationManually(deviceInformation);

		declareDevice(api, deviceInformation);

		declareKeyword(api, deviceInformation);

		api->setPluginState(yApi::historization::EPluginState::kRunning);

		return deviceInformation;
	}
	catch (std::exception&)
	{
		api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
		api->getEventHandler().createTimer(kConnectionRetryTimer,
		                                   shared::event::CEventTimer::kOneShot,
		                                   boost::posix_time::seconds(30));
		throw;
	}
}

void CLametricTime::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi>& api,
                                          const shared::CDataContainer& newConfigurationData)
{
	const auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
	YADOMS_LOG(information) << "Update configuration...";

	m_configuration.initializeWith(newConfiguration);
	m_configuration.trace();
}
