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
	kEvtKeyStateReceived,
};

void CStreamDeck::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
	api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

	YADOMS_LOG(information) << "StreamDeck is starting...";

	m_configuration.initializeWith(api->getConfiguration());
	m_deviceManager = CFactory::createDeviceManager(m_configuration, api->getEventHandler(), kEvtKeyStateReceived);

	auto deviceInformation = initDevice(api);
	
	try
	{
		m_deviceManager->open();
		
		m_deviceManager->reset();

		m_deviceManager->setBrightness(30);

		m_deviceManager->runKeyStateThread();
	}
	catch (const std::exception & exception)
	{
		YADOMS_LOG(error) << exception.what();
		api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
		throw;
	}

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
				m_deviceManager->close();
				return;
			}

		case yApi::IYPluginApi::kEventUpdateConfiguration:
			{
				// Configuration was updated
				api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
				const auto newConfiguration = api->getEventHandler().getEventData<shared::CDataContainer>();
				YADOMS_LOG(information) << "Update configuration...";

				m_deviceManager = CFactory::createDeviceManager(m_configuration, api->getEventHandler(),
				                                                kEvtKeyStateReceived);

				deviceInformation.reset();
				deviceInformation = initDevice(api);

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
		case kEvtKeyStateReceived:
			{
				auto keyIndex = api->getEventHandler().getEventData<int>();
				api->historizeData(m_usbDeviceInformation->deviceName, m_keywords[keyIndex]);
				break;
			}
		case yApi::IYPluginApi::kBindingQuery:
			{
				// Yadoms ask for a binding query 
				auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IBindingQueryRequest>>();
				if (request->getData().getQuery() == "keyCreation")
				{
					auto keys = CDeviceManagerHelper::buildKeys(deviceInformation->keyCols, deviceInformation->keyRows);
					auto coordinates = CDeviceManagerHelper::buildCoordinates(
						deviceInformation->keyCols, deviceInformation->keyRows);

					shared::CDataContainer ev;
					for (size_t i = 0; i < keys.size(); i++)
					{
						ev.set(keys[i], coordinates[i]);
					}

					shared::CDataContainer en;
					en.set("type", "enum");
					en.set("values", ev);
					en.set("defaultValue", "KEY0");

					shared::CDataContainer result;
					result.set("interval", en);

					request->sendSuccess(result);
				}
				else
				{
					const auto errorMessage = (boost::format("unknown query : %1%") % request->getData().getQuery()).str();
					request->sendError(errorMessage);
					YADOMS_LOG(error) << errorMessage;
				}
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
						auto interval = extraQuery->getData()->data().get<std::string>(
							"dynamicSection.content.interval");

						auto keyIndex = CDeviceManagerHelper::getKeyIndex(interval);

						auto customText = extraQuery->getData()->data().get<std::string>(
							"customText");

						YADOMS_LOG(information) << "Command with plugin binded data received : value=" << interval;

						auto fileFromClient = extraQuery->getData()->data().get<yApi::configuration::CFile>("fileContent");
						auto firmwareContent = fileFromClient.getContent();

						YADOMS_LOG(information) << "File received from extra command";
						YADOMS_LOG(information) << "    File name = " << fileFromClient.getFileName();
						YADOMS_LOG(information) << "    File size = " << fileFromClient.getSize();
						YADOMS_LOG(information) << "    File type = " << fileFromClient.getMimeType();
						YADOMS_LOG(information) << "    File date = " << fileFromClient
						                                                 .getLastModificationDate().getBoostDateTime();
						YADOMS_LOG(information) << "    content = " << fileFromClient.getContent();

						auto img = fileFromClient.getContent();
						m_deviceManager->setKeyImage(img, keyIndex, customText);

						for (auto i = 0; i < 100; ++i)
						{
							if (i < 25)
								extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step1");
							else if (i < 50)
								extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step2");
							else if (i < 75)
								extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step3");
							else
								extraQuery->reportProgress(i * 1.0f, "customLabels.createKey.step4");
							boost::this_thread::sleep(boost::posix_time::milliseconds(35));
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

void CStreamDeck::declareDeviceAndKeywords(boost::shared_ptr<yApi::IYPluginApi>& api,
                                           boost::shared_ptr<UsbDeviceInformation>& deviceInformation)

{
	if (!api->deviceExists(deviceInformation->deviceName))
	{
		for (auto i = 0; i < deviceInformation->keyCount; ++i)
			m_keywords[i] = boost::make_shared<yApi::historization::CEvent>("Key #" + std::to_string(i), shared::plugin::yPluginApi::EKeywordAccessMode::kGet);

		const auto keywordsAsVecor = CDeviceManagerHelper::mapToHistorizableVector(m_keywords);

		api->declareDevice(deviceInformation->deviceName, deviceInformation->serialNumber,
		                   deviceInformation->deviceModel, keywordsAsVecor);
	}
}


boost::shared_ptr<UsbDeviceInformation> CStreamDeck::initDevice(boost::shared_ptr<yApi::IYPluginApi>& api)
{
	try
	{
		m_usbDeviceInformation = CDeviceManagerHelper::getDeviceInformation(m_configuration);

		declareDeviceAndKeywords(api, m_usbDeviceInformation);

		api->setPluginState(yApi::historization::EPluginState::kRunning);

		return m_usbDeviceInformation;
	}
	catch (std::exception&)
	{
		api->setPluginState(yApi::historization::EPluginState::kError, "initializationError");
		throw;
	}
}
