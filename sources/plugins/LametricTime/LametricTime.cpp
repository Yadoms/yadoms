#include "stdafx.h"
#include "LametricTime.h"
#include <plugin_cpp_api/ImplementationHelper.h>

#include <Poco/Net/HTTPSClientSession.h>
#include <shared/Log.h>
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
	  m_iconType(boost::make_shared<specificHistorizers::CCustomizeIconType>(IconTypeName))
{
}

void CLametricTime::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
	YADOMS_LOG(information) << "LametricTime is starting...";

	m_configuration.initializeWith(api->getConfiguration());

	init(api);

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
					onUpdateConfiguration(
						api, api->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>>());
					api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot,
					                                   boost::posix_time::seconds(30));

					m_configuration.getPairingMode() == EPairingMode::kAuto
						? m_devicesInformation.clear()
						: m_deviceInformation.reset();
					init(api);
				}
				catch (...)
				{
					YADOMS_LOG(information) << "Wrong configuration update";
				}
				break;
			}

		case yApi::IYPluginApi::kEventDeviceCommand:
			{
				if (m_configuration.getPairingMode() == EPairingMode::kAuto)
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
					api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
				YADOMS_LOG(information) << "Command received from Yadoms : " << yApi::IDeviceCommand::toString(command);

				if (m_configuration.getPairingMode() == EPairingMode::kAuto)
				{
					m_targetDevice = std::find_if(m_devicesInformation.begin(), m_devicesInformation.end(),
					                              boost::bind(&DeviceInformation::m_deviceName, boost::placeholders::_1) == command->getDevice());
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

				if (boost::iequals(command->getDevice(),
				                   m_configuration.getPairingMode() == EPairingMode::kAuto
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
				m_configuration.getPairingMode() == EPairingMode::kAuto
					? m_devicesInformation.clear()
					: m_deviceInformation.reset();

				init(api);

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

void CLametricTime::declareAllDevicesAndKeywords(boost::shared_ptr<yApi::IYPluginApi>& api,
                                                 CSsdpDiscoveredDevice& foundDevices,
                                                 std::vector<DeviceInformation>& devicesInformation) const
{
	for (size_t i = 0; i < foundDevices.getDevicesDescription().size(); i++)
	{
		YADOMS_LOG(information) << "Creating the device :" << devicesInformation[i].m_deviceName;
		if (!api->deviceExists(devicesInformation[i].m_deviceName))
			api->declareDevice(devicesInformation[i].m_deviceName, devicesInformation[i].m_deviceType,
			                   devicesInformation[i].m_deviceModel);
		if (!api->keywordExists(devicesInformation[i].m_deviceName, m_text))
			api->declareKeyword(devicesInformation[i].m_deviceName, m_text);
		if (!api->keywordExists(devicesInformation[i].m_deviceName, m_iconType))
			api->declareKeyword(devicesInformation[i].m_deviceName, m_iconType);
	}
}

void CLametricTime::declareKeyword(boost::shared_ptr<yApi::IYPluginApi>& api,
                                   boost::shared_ptr<DeviceInformation>& deviceInformation) const
{
	YADOMS_LOG(information) << "Declaring the keyword :" << m_text;

	if (!api->keywordExists(deviceInformation->m_deviceName, m_text))
		api->declareKeyword(deviceInformation->m_deviceName, m_text);
	if (!api->keywordExists(deviceInformation->m_deviceName, m_iconType))
		api->declareKeyword(deviceInformation->m_deviceName, m_iconType);
}

boost::shared_ptr<DeviceInformation> CLametricTime::fillDeviceInformationManually() const
{
	auto deviceInformation = boost::make_shared<DeviceInformation>();
	deviceInformation->m_deviceName = DeviceName;
	deviceInformation->m_deviceModel = m_deviceManager->getDeviceInformations()->get<std::string>("model");
	deviceInformation->m_deviceType = m_deviceManager->getDeviceInformations()->get<std::string>("name");
	return deviceInformation;
}


std::vector<DeviceInformation> CLametricTime::fillAllDevicesInformationAutomatically(
	CSsdpDiscoveredDevice& foundDevices)
{
	std::vector<DeviceInformation> devicesInformation;
	DeviceInformation deviceInformation;
	for (size_t i = 0; i < foundDevices.getDevicesDescription().size(); i++)
	{
		deviceInformation.m_deviceName = foundDevices.findTag("modelName", i) + " " + foundDevices.getIp(i);
		deviceInformation.m_deviceModel = foundDevices.findTag("friendlyName", i);
		deviceInformation.m_deviceType = foundDevices.findTag("modelName", i);
		deviceInformation.m_deviceIp = foundDevices.getIp(i);
		devicesInformation.push_back(deviceInformation);
	}

	return devicesInformation;
}


void CLametricTime::init(boost::shared_ptr<yApi::IYPluginApi>& api)
{
	if (m_configuration.getPairingMode() == EPairingMode::kAuto)
	{
		m_devicesInformation = initAutomatically(api);
	}
	else
	{
		m_deviceInformation = initManually(api);
	}
}

std::vector<DeviceInformation> CLametricTime::initAutomatically(boost::shared_ptr<yApi::IYPluginApi>& api) const
{
	try
	{
		CSsdpDiscoveredDevice foundDevices;
		if (!CSsdpDiscoverService::discover(
				[&foundDevices](const CSsdpDiscoveredDevice& device)
				{
					if (!device.getDevicesDescription().empty())
					{
						foundDevices = device;
						return true;
					}
					return false;
				},
				std::chrono::seconds(10),
				"urn:schemas-upnp-org:device:LaMetric:1")
		)
		{
			api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
			throw std::runtime_error("No response from the device: wrong ip or no device listening on this network");
		}

		auto devicesInformation = fillAllDevicesInformationAutomatically(foundDevices);

		declareAllDevicesAndKeywords(api, foundDevices, devicesInformation);

		api->setPluginState(yApi::historization::EPluginState::kRunning);

		return devicesInformation;
	}
	catch (const std::exception& exception)
	{
		YADOMS_LOG(error) << exception.what();
		api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
		throw;
	}
}

boost::shared_ptr<DeviceInformation> CLametricTime::initManually(boost::shared_ptr<yApi::IYPluginApi>& api)
{
	YADOMS_LOG(information) << "Init the connection ...";

	m_deviceManager = CFactory::createDeviceState(m_configuration);
	m_senderManager = CFactory::createNotificationSender(m_configuration);
	try
	{
		m_deviceManager->getDeviceState();

		auto deviceInformation = fillDeviceInformationManually();

		declareDevice(api, deviceInformation);

		declareKeyword(api, deviceInformation);

		api->setPluginState(yApi::historization::EPluginState::kRunning);

		return deviceInformation;
	}
	catch (std::exception&)
	{
		api->getEventHandler().createTimer(kConnectionRetryTimer,
		                                   shared::event::CEventTimer::kOneShot,
		                                   boost::posix_time::seconds(30));
		api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
		throw;
	}
}

void CLametricTime::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi>& api,
                                          const boost::shared_ptr<shared::CDataContainer>& newConfigurationData)
{
	const auto newConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>>();
	YADOMS_LOG(information) << "Update configuration...";

	m_configuration.initializeWith(newConfiguration);
	m_configuration.trace();
}
