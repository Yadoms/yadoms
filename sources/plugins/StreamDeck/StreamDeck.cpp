#include "CFactory.h"
#include "stdafx.h"
#include "StreamDeck.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include "DeviceManager.h"
#include <shared/plugin/yPluginApi/configuration/File.h>

IMPLEMENT_PLUGIN(CStreamDeck)

CStreamDeck::CStreamDeck()
{
}

CStreamDeck::~CStreamDeck()
{
}

enum
{
	kCustomEvent = yApi::IYPluginApi::kPluginFirstEventId,
};

void CStreamDeck::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
	api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

	YADOMS_LOG(information) << "StreamDeck is starting...";

	m_configuration.initializeWith(api->getConfiguration());
	m_deviceManager = CFactory::createDeviceManager(m_configuration);

	auto deviceInformation = initDevice(api);

	// the main loop
	while (true)
	{
		// Wait for an event
		switch (api->getEventHandler().waitForEvents())
		{
		case yApi::IYPluginApi::kEventStopRequested:
			{
				// Yadoms request the plugin to stop. Note that plugin must be stopped in 10 seconds max, otherwise it will be killed.
				YADOMS_LOG(information) << "Stop requested";
				api->setPluginState(yApi::historization::EPluginState::kStopped);
				return;
			}

		case yApi::IYPluginApi::kEventUpdateConfiguration:
			{
				// Configuration was updated
				api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
				const auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
				YADOMS_LOG(information) << "Update configuration...";

				m_configuration.initializeWith(newConfiguration);

				// Trace the configuration
				m_configuration.trace();

				api->setPluginState(yApi::historization::EPluginState::kRunning);

				break;
			}

		case yApi::IYPluginApi::kEventDeviceCommand:
			{
				// A command was received from Yadoms
				const auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>
				>();
				YADOMS_LOG(information) << "Command received from Yadoms : " << yApi::IDeviceCommand::toString(command);

				/*
	
				Process the command here (to drive a keyword for example)
	
				*/

				break;
			}

		case yApi::IYPluginApi::kEventExtraQuery:
		{
			// Extra-command was received from Yadoms
			auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery>>();

			if (extraQuery)
			{
				YADOMS_LOG(information) << "Extra command received : " << extraQuery->getData()->query();

				if (extraQuery->getData()->query() == "createKey")
				{
					auto fileFromClient = extraQuery->getData()->data().get<yApi::configuration::CFile>("fileContent");
					auto firmwareContent = fileFromClient.getContent();

					YADOMS_LOG(information) << "File received from extra command";
					YADOMS_LOG(information) << "    File name = " << fileFromClient.getFileName();
					YADOMS_LOG(information) << "    File size = " << fileFromClient.getSize();
					YADOMS_LOG(information) << "    File type = " << fileFromClient.getMimeType();
					YADOMS_LOG(information) << "    File date = " << fileFromClient.getLastModificationDate().getBoostDateTime();
					YADOMS_LOG(information) << "    content = " << fileFromClient.getContent();

					for (int i = 0; i < 100; ++i)
					{
						if (i < 25)
							extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step1");
						else if (i < 50)
							extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step2");
						else if (i < 75)
							extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step3");
						else
							extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step4");
						boost::this_thread::sleep(boost::posix_time::milliseconds(200));
					}
				}

				extraQuery->sendSuccess(shared::CDataContainer());
			}
			else
			{
				extraQuery->sendError("error content");
			}
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
void CStreamDeck::declareDevice(boost::shared_ptr<yApi::IYPluginApi>& api,
										boost::shared_ptr<UsbDeviceInformation>& deviceInformation)

{
	if (!api->deviceExists(deviceInformation->deviceName))
		api->declareDevice(deviceInformation->deviceName, deviceInformation->serialNumber, deviceInformation->deviceModel);
}

boost::shared_ptr<UsbDeviceInformation> CStreamDeck::initDevice(boost::shared_ptr<yApi::IYPluginApi>& api) const
{
	try
	{
		auto usbDeviceInformation = m_deviceManager->getDeviceInformation();

		declareDevice(api, usbDeviceInformation);

		api->setPluginState(yApi::historization::EPluginState::kRunning);
		
		return usbDeviceInformation;
	}
	catch (std::exception&)
	{
		api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
		throw;
	}
}
