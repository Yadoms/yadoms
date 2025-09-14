#include "stdafx.h"
#include "EnOcean.h"
#include <utility>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/communication/PortException.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "DeviceConfigurationHelper.h"
#include "Factory.h"
#include "manufacturers.h"
#include "ProfileHelper.h"
#include "ProtocolException.hpp"
#include "message/RequestDongleVersionCommand.h"
#include "message/response/ReceivedMessage.h"
#include "message/smart_ack/EnablePostMasterCommand.h"
#include "message/smart_ack/ReadClientMailboxStatusCommand.h"
#include "message/smart_ack/ReadLearnedClientsCommand.h"
#include "message/smart_ack/ReadLearnModeCommand.h"
#include "message/radioErp1/4BSTeachinResponse.h"
#include "message/radioErp1/ReceivedMessage.h"
#include "message/radioErp1/UTEGigaConceptReversedAnswer.h"
#include "message/radioErp1/UTEGigaConceptReversedRequest.h"
#include "message/MessageHelpers.h"
#include "profiles/bitsetHelpers.hpp"
#include "profiles/eep.h"


IMPLEMENT_PLUGIN(CEnOcean)


// Event IDs
enum
{
	kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId,
	kEvtPortDataReceived,
	kProtocolErrorRetryTimer,
	kProgressPairingTimer,
	kAnswerTimeout,
};

static constexpr unsigned int PairingTimeoutSeconds = 60;
static constexpr unsigned int PairingPeriodTimeSeconds = 5;
static constexpr unsigned int PairingProgressNbMaxPeriods = PairingTimeoutSeconds / PairingPeriodTimeSeconds;


CEnOcean::CEnOcean()
	: m_signalPowerKeyword(boost::make_shared<shared::plugin::yPluginApi::historization::CSignalPower>("signal power"))
{
}

void CEnOcean::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
	m_api = api;

	m_api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

	YADOMS_LOG(information) << "EnOcean is starting...";

	// Load configuration values (provided by database)
	m_configuration.initializeWith(m_api->getConfiguration());

	// Load known devices
	loadAllDevices();

	// Create the connection
	createConnection();

	// the main loop
	while (true)
	{
		try
		{
			// Wait for an event
			switch (m_api->getEventHandler().waitForEvents())
			{
			case yApi::IYPluginApi::kEventStopRequested:
			{
				YADOMS_LOG(information) << "Stop requested";
				m_api->setPluginState(yApi::historization::EPluginState::kStopped);
				return;
			}

			case yApi::IYPluginApi::kEventUpdateConfiguration:
			{
				// Configuration was updated
				m_api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
				const auto newConfigurationData = m_api->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>>();
				YADOMS_LOG(information) << "Update configuration...";
				BOOST_ASSERT(!newConfigurationData->empty());
				// newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

				// Close connection
				CConfiguration newConfiguration;
				newConfiguration.initializeWith(newConfigurationData);

				// If port has changed, destroy and recreate connection (if any)
				const auto needToReconnect = !connectionsAreEqual(m_configuration, newConfiguration) && !!m_port;

				if (needToReconnect)
					destroyConnection();

				// Update configuration
				m_configuration.initializeWith(newConfigurationData);

				if (needToReconnect)
					createConnection();
				else
					m_api->setPluginState(yApi::historization::EPluginState::kRunning);

				break;
			}

			case yApi::IYPluginApi::kEventDeviceCommand:
			{
				// Command received from Yadoms
				const auto command(m_api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>());
				YADOMS_LOG(information) << "Command received : " << yApi::IDeviceCommand::toString(command);

				processDeviceCommand(command);

				break;
			}

			case yApi::IYPluginApi::kEventDeviceRemoved:
			{
				const auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved>>();
				processDeviceRemoved(device->device());
				break;
			}

			case yApi::IYPluginApi::kSetDeviceConfiguration:
			{
				// Yadoms sent the new device configuration. Plugin must apply this configuration to device.
				const auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();
				processDeviceConfiguration(deviceConfiguration->name(), deviceConfiguration->configuration());
				break;
			}

			case kEvtPortConnection:
			{
				const auto notification = m_api->getEventHandler().getEventData<boost::shared_ptr<
					shared::communication::CAsyncPortConnectionNotification>>();

				if (notification && notification->isConnected())
				{
					processConnectionEvent();
				}
				else
				{
					processUnConnectionEvent(notification);
					m_api->getEventHandler().createTimer(kProtocolErrorRetryTimer,
														 shared::event::CEventTimer::kOneShot,
														 boost::posix_time::seconds(10));
				}

				break;
			}
			case kEvtPortDataReceived:
			{
				const auto message(m_api->getEventHandler().getEventData<boost::shared_ptr<const message::CEsp3ReceivedPacket>>());
				processDataReceived(message);
				break;
			}
			case kAnswerTimeout:
			{
				YADOMS_LOG(error) << "No answer received from EnOcean dongle (timeout)";
				protocolErrorProcess();
				break;
			}
			case kProtocolErrorRetryTimer:
			{
				createConnection();
				break;
			}
			case yApi::IYPluginApi::kEventExtraQuery:
			{
				// Command was received from Yadoms
				if (auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery>>())
				{
					if (extraQuery->getData()->query() == "pairing")
					{
						if (m_pairingHelper->isPairing())
						{
							stopPairing();
						}
						else
						{
							m_pairingQuery = extraQuery;
							startPairing(api);
						}
					}
					else
					{
						YADOMS_LOG(error) << "Unsupported query : " << extraQuery->getData()->query();
						extraQuery->sendError("customLabels.extraquery.ErrorInternal");
					}
				}
				else
				{
					YADOMS_LOG(error) << "Invalid query";
					extraQuery->sendError("customLabels.extraquery.ErrorInternal");
				}
				break;
			}
			case kProgressPairingTimer:
			{
				if (!m_pairingHelper->isPairing())
					m_progressPairingTimer.reset();

				--m_progressPairingCount;
				if (m_progressPairingCount == 0)
				{
					stopPairing();
				}
				else
				{
					if (m_pairingQuery)
						m_pairingQuery->reportProgress(
							static_cast<float>(PairingProgressNbMaxPeriods - m_progressPairingCount) * 100.0f / PairingProgressNbMaxPeriods,
							"customLabels.pairing.pairing");

					m_progressPairingTimer->start();
				}
				break;
			}
			default:
			{
				YADOMS_LOG(warning) << "Unknown message id " << api->getEventHandler().getEventId();
				break;
			}
			}
		}
		catch (std::logic_error& e)
		{
			YADOMS_LOG(error) << "Logical error : " << e.what();
		}
		catch (std::exception& e)
		{
			YADOMS_LOG(error) << "Error communicating with EnOcean dongle " << e.what();
			protocolErrorProcess();
		}
	}
}

void CEnOcean::loadAllDevices()
{
	const auto devices = m_api->getAllDevices();
	for (const auto& deviceId : devices)
	{
		try
		{
			const auto deviceConfiguration = m_api->getDeviceConfiguration(deviceId);
			if (deviceId == "pluginState" || deviceConfiguration->empty())
				continue; // Not configured device

			const CProfileHelper profileHelper(deviceConfiguration->get<std::string>("profile.activeSection"));
			const auto device = createDevice(deviceId,
											 profileHelper);

			createNewKeywords(deviceId, device);

			m_devices[deviceId] = device;
		}
		catch (shared::exception::CEmptyResult&) // NOLINT(bugprone-empty-catch)
		{
			// Don't add a not configured device to the m_devices list
		}
		catch (std::exception& e)
		{
			// Don't add a wrong configured device to the m_devices list
			YADOMS_LOG(error) << "Error loading device from database : device " << deviceId << " is malformed (" << e.what() << "), will be ignored";
		}
	}
}

void CEnOcean::createNewKeywords(const std::string& deviceName,
								 const boost::shared_ptr<IType>& loadedDevice) const
{
	for (const auto& historizer : loadedDevice->allHistorizers())
	{
		if (!m_api->keywordExists(deviceName, historizer->getKeyword()))
			m_api->declareKeyword(deviceName, historizer);
	}
}

void CEnOcean::createConnection()
{
	// Create the port instance
	m_port = CFactory::constructPort(m_configuration);

	const auto bufferLogger = CFactory::constructBufferLogger();

	m_messageHandler = CFactory::constructMessageHandler(m_port,
														 m_api->getEventHandler(),
														 kEvtPortDataReceived,
														 bufferLogger);

	m_pairingHelper = CFactory::constructPairingHelper(m_messageHandler);

	m_port->subscribeForConnectionEvents(m_api->getEventHandler(),
										 kEvtPortConnection);

	m_port->setReceiveBufferHandler(CFactory::constructReceiveBufferHandler(m_messageHandler));

	m_port->start();
}

void CEnOcean::destroyConnection()
{
	if (!m_port)
		return;
	m_port->setReceiveBufferHandler(boost::shared_ptr<shared::communication::IReceiveBufferHandler>());
	m_port.reset();
	m_messageHandler.reset();
}

bool CEnOcean::connectionsAreEqual(const CConfiguration& conf1,
								   const CConfiguration& conf2)
{
	return (conf1.getSerialPort() == conf2.getSerialPort());
}

boost::shared_ptr<IType> CEnOcean::createDevice(const std::string& deviceId,
												const CProfileHelper& profileHelper) const
{
	auto device = CRorgs::createRorg(profileHelper.rorg())->createFunc(profileHelper.func())->createType(profileHelper.type(),
																										 deviceId,
																										 m_api);

	return device;
}

std::string CEnOcean::generateModel(const std::string& model,
									const std::string& manufacturer,
									const CProfileHelper& profile) const
{
	if (!model.empty())
		return model;

	std::string generatedModel;

	if (!manufacturer.empty())
		generatedModel += manufacturer + std::string(" - ");

	generatedModel += createDevice(std::string(), profile)->title();

	return generatedModel;
}

void CEnOcean::processDeviceCommand(const boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand>& command)
{
	if (!m_port)
	{
		YADOMS_LOG(information) << "Unable to process command : dongle is not ready";
		return;
	}

	if (m_devices.find(command->getDevice()) == m_devices.end())
	{
		YADOMS_LOG(error) << "Unable to process command : device " << command->getDevice() << " unknown";
		return;
	}

	try
	{
		m_devices[command->getDevice()]->sendCommand(command->getKeyword(),
													 command->getBody(),
													 m_senderId,
													 m_messageHandler);
	}
	catch (std::exception& e)
	{
		YADOMS_LOG(error) << "Fail to send command " << yApi::IDeviceCommand::toString(command) << ", error : " << e.what();
	}
}

void CEnOcean::processConnectionEvent()
{
	YADOMS_LOG(information) << "EnOcean port opened";

	try
	{
		// USB dongle init
		requestDongleVersion();

		// Smart ack configuration
		readSmartAckLearnMode();
		enableSmartAckPostMaster(true);
		const auto smartAckClients = readSmartAckLearnedClients();
		for (const auto& smartAckClient : smartAckClients)
			readSmartAckClientMailboxStatus(smartAckClient);

		YADOMS_LOG(information) << "Ready";

		m_api->setPluginState(yApi::historization::EPluginState::kRunning);

		// Devices init
		for (const auto& device : m_devices)
		{
			device.second->readInitialState(m_senderId,
											m_messageHandler);
		}
	}
	catch (CProtocolException& e)
	{
		YADOMS_LOG(error) << "Protocol error : " << e.what();
		protocolErrorProcess();
	}
	catch (shared::communication::CPortException& e)
	{
		YADOMS_LOG(error) << "Error connecting to EnOcean dongle : " << e.what();
		// Disconnection will be notified, we just have to wait...
	}
	catch (std::exception& e)
	{
		YADOMS_LOG(error) << "Error connecting to EnOcean dongle : " << e.what();
		// Disconnection will be notified, we just have to wait...
	}
}

void CEnOcean::protocolErrorProcess()
{
	// Retry full connection
	processUnConnectionEvent();
	m_api->getEventHandler().createTimer(kProtocolErrorRetryTimer,
										 shared::event::CEventTimer::kOneShot,
										 boost::posix_time::seconds(10));
}

void CEnOcean::processUnConnectionEvent(const boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>& notification)
{
	YADOMS_LOG(information) << "EnOcean connection was lost";
	if (notification)
		m_api->setPluginState(yApi::historization::EPluginState::kError, notification->getErrorMessageI18n(),
							  notification->getErrorMessageI18nParameters());
	else
		m_api->setPluginState(yApi::historization::EPluginState::kCustom, "connectionFailed");

	destroyConnection();
}

void CEnOcean::processDeviceRemoved(const std::string& deviceId)
{
	if (m_devices.find(deviceId) == m_devices.end())
		return;

	m_devices.erase(deviceId);

	YADOMS_LOG(information) << "Device " << deviceId << " was removed";
}

void CEnOcean::processDeviceConfiguration(const std::string& deviceId,
										  const boost::shared_ptr<shared::CDataContainer>& configuration)
{
	try
	{
		const auto selectedProfile = CProfileHelper(configuration->get<std::string>("profile.activeSection"));
		auto manufacturer = configuration->get<std::string>("manufacturer");

		YADOMS_LOG(information) << "Device \"" << deviceId << "\" is configured as " << selectedProfile.profile();

		if (m_devices.find(deviceId) == m_devices.end() || m_devices[deviceId]->profile() != selectedProfile.profile())
		{
			// Device was not configured or profile changed, recreate local device and all keywords
			if (m_devices.find(deviceId) != m_devices.end())
				for (const auto& keyword : m_api->getAllKeywords(deviceId))
					m_api->removeKeyword(deviceId, keyword);

			// Don't recreate device in Yadoms, unless it will change of ID
			const auto device = createDevice(deviceId,
											 selectedProfile);
			m_api->declareKeywords(deviceId,
								   device->allHistorizers());
			m_devices[deviceId] = device;
		}

		// Send configuration to device
		try
		{
			if (configuration->exists("profile.content." + m_devices[deviceId]->profile() + ".content"))
				m_devices[deviceId]->sendConfiguration(
					configuration->get<shared::CDataContainer>("profile.content." + m_devices[deviceId]->profile() + ".content"),
					m_senderId,
					m_messageHandler);
		}
		catch (std::exception& e)
		{
			throw std::runtime_error(std::string("Unable to configure device : ") + e.what());
		}
	}
	catch (shared::exception::CEmptyResult&)
	{
		YADOMS_LOG(error) << "Unable to configure device : unknown device \"" << deviceId << "\"";
	}
	catch (std::exception& e)
	{
		YADOMS_LOG(error) << "Unable to configure device : " << e.what();
	}
}

void CEnOcean::processDataReceived(const boost::shared_ptr<const message::CEsp3ReceivedPacket>& message)
{
	try
	{
		switch (message->header().packetType()) // NOLINT(clang-diagnostic-switch-enum)
		{
		case message::RADIO_ERP1:
			processRadioErp1(message);
			break;
		case message::RESPONSE:
			processResponse(message);
			break;
		case message::EVENT:
			processEvent(message);
			break;
		default:
			throw CProtocolException((boost::format("Unknown or unsupported received packet type %1%") % message->header().packetType()).str());
		}
	}
	catch (CProtocolException& e)
	{
		YADOMS_LOG(error) << "Error processing received message : " << e.what();
	}
	catch (std::exception& e)
	{
		YADOMS_LOG(error) << "Error processing received message : " << e.what();
	}
}

void CEnOcean::addSignalPower(std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords,
							  const std::string& deviceId,
							  int signalPower) const
{
	if (!m_api->keywordExists(deviceId, m_signalPowerKeyword))
		m_api->declareKeyword(deviceId, m_signalPowerKeyword);

	m_signalPowerKeyword->set(signalPower);
	keywords.emplace_back(m_signalPowerKeyword);
}

int CEnOcean::dbmToSignalPower(int dBm)
{
	// Thresholds were determined empirically with DolphinView tool
	if (dBm >= -65)
		return 100;
	if (dBm >= -67)
		return 90;
	if (dBm >= -74)
		return 80;
	if (dBm >= -76)
		return 70;
	if (dBm >= -83)
		return 60;
	if (dBm >= -85)
		return 50;
	if (dBm >= -91)
		return 40;
	return 20;
}

void CEnOcean::processRadioErp1(const boost::shared_ptr<const message::CEsp3ReceivedPacket>& esp3Packet)
{
	message::radioErp1::CReceivedMessage erp1Message(esp3Packet);

	if (erp1Message.rorg() == CRorgs::kUTE_Telegram)
	{
		if (!m_pairingHelper->isPairing())
		{
			YADOMS_LOG(warning) << "A device asked for pairing by UTE telegram, but plugin is not in pairing mode";
			return;
		}

		const auto deviceTitle = processUTE(erp1Message);

		stopPairing(deviceTitle);

		return;
	}

	// Create associated RORG object
	const auto erp1UserData = bitset_from_bytes(erp1Message.userData());
	const auto erp1Status = bitset_from_byte(erp1Message.status());
	const auto rorg = CRorgs::createRorg(erp1Message.rorg());
	const auto deviceId = erp1Message.senderId();

	if (rorg->isTeachIn(erp1UserData))
	{
		// Teachin telegram

		if (!m_pairingHelper->isPairing())
		{
			YADOMS_LOG(warning) << "A device asked for pairing by Teach-In telegram, but plugin is not in pairing mode";
			return;
		}

		std::string pairedDeviceTitle;

		if (rorg->isEepProvided(erp1UserData))
		{
			if (rorg->id() != CRorgs::k4BS_Telegram)
				throw std::domain_error("Teach-in telegram is only supported for 4BS telegram for now. Please report to Yadoms-team.");

			pairedDeviceTitle = processEepTeachInMessage(message::radioErp1::C4BSTeachinRequest(erp1UserData),
														 rorg,
														 deviceId);
		}
		else
		{
			pairedDeviceTitle = processNoEepTeachInMessage(rorg,
														   deviceId);
		}

		stopPairing(pairedDeviceTitle);

		return;
	}
	else
	{
		// Data telegram
		processDataTelegram(erp1Message, erp1UserData, erp1Status, deviceId);
	}
}

void CEnOcean::declareDeviceWithoutProfile(const std::string& deviceId) const
{
	YADOMS_LOG(information) << "New device declared : ";
	YADOMS_LOG(information) << "  - Id           : " << deviceId;
	YADOMS_LOG(information) << "  - Profile      : Unknown. No historization until user enter profile.";

	m_api->declareDevice(deviceId,
						 std::string(),
						 "!! DEVICE NOT READY !! Please select the correct profile in configuration",
						 std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>());
}

void CEnOcean::processResponse(const boost::shared_ptr<const message::CEsp3ReceivedPacket>&)
{
	YADOMS_LOG(error) << "Unexpected response received";
}

std::string CEnOcean::processEepTeachInMessage(const message::radioErp1::C4BSTeachinRequest& teachInRequest,
											   const boost::shared_ptr<IRorg>& rorg,
											   const std::string& deviceId)
{
	try
	{
		const auto profile = CProfileHelper(rorg->id(),
											teachInRequest.funcId(),
											teachInRequest.typeId());
		const auto& manufacturerName = CManufacturers::name(teachInRequest.manufacturerId());

		const CDeviceConfigurationHelper deviceConfiguration(profile,
															 manufacturerName);

		if (m_api->deviceExists(deviceId))
		{
			// Device already exist, just reconfigure it
			m_api->updateDeviceConfiguration(deviceId,
											 deviceConfiguration.configuration());

			processDeviceConfiguration(deviceId,
									   deviceConfiguration.configuration());

			m_devices[deviceId]->readInitialState(m_senderId,
												  m_messageHandler);

			return {};
		}

		// Device not exist, declare it
		const auto& device = declareDevice(deviceId,
										   profile,
										   manufacturerName);

		if (!device)
			return {};

		m_api->updateDeviceConfiguration(deviceId,
										 deviceConfiguration.configuration());

		message::radioErp1::C4BSTeachinResponse teachinResponse(m_senderId,
																deviceId,
																0,
																teachInRequest.funcId(),
																teachInRequest.typeId(),
																teachInRequest.manufacturerId());
		message::CMessageHelpers::sendMessage(teachinResponse,
											  m_messageHandler,
											  "Teach-In response");

		return device->title();
	}
	catch (std::exception& e)
	{
		YADOMS_LOG(error) << "Unable to declare device : " << e.what();
		return {};
	}
}

std::string CEnOcean::processNoEepTeachInMessage(const boost::shared_ptr<IRorg>& rorg,
												 const std::string& deviceId)
{
	// Special case for the 1BS RORG : only one func and type exist, so profile can be known
	if (rorg->id() == CRorgs::k1BS_Telegram)
	{
		const auto profile = CProfileHelper(CRorgs::k1BS_Telegram,
											C1BSTelegram::kContacts_and_Switches,
											C1BS_0x00::k0x01);

		static const std::string ManufacturerName("Unknown manufacturer");
		const CDeviceConfigurationHelper deviceConfiguration(profile,
															 ManufacturerName);

		std::string pairedDeviceTitle;
		if (m_api->deviceExists(deviceId))
		{
			if (m_devices.find(deviceId) == m_devices.end())
			{
				// Device was declared without profile (data telegrams was probably received before teachin telegram)
				// In this case, remove device before recreating
				m_api->removeDevice(deviceId);

				const auto& device = declareDevice(deviceId,
												   profile,
												   ManufacturerName);

				if (!device)
					return {};

				device->readInitialState(m_senderId,
										 m_messageHandler);
			}
			else
			{
				// Device already well declared, just update model
				m_api->updateDeviceModel(deviceId,
										 generateModel(m_api->getDeviceModel(deviceId),
													   ManufacturerName,
													   profile));
			}
		}
		else
		{
			const auto& device = declareDevice(deviceId,
											   profile,
											   ManufacturerName);

			if (!device)
				return {};

			device->readInitialState(m_senderId,
									 m_messageHandler);

			pairedDeviceTitle = device->title();
		}

		m_api->updateDeviceConfiguration(deviceId,
										 deviceConfiguration.configuration());
		return pairedDeviceTitle;
	}

	if (m_api->deviceExists(deviceId))
	{
		// Device exist.
		// It is ether configured or not, but this teachin message give us nothing new (no EEP is provided), so ignore it
		YADOMS_LOG(information) << "Device " << deviceId << " already declared, teachin message ignored";
		return {};
	}

	declareDeviceWithoutProfile(deviceId);

	return deviceId;
}

void CEnOcean::processDataTelegram(const message::radioErp1::CReceivedMessage& erp1Message,
								   const boost::dynamic_bitset<>& erp1UserData,
								   const boost::dynamic_bitset<>& erp1Status,
								   const std::string& deviceId)
{
	if (m_devices.find(deviceId) == m_devices.end())
	{
		// Unknown device

		if (m_api->deviceExists(deviceId))
		{
			YADOMS_LOG(information) << "Device " << deviceId << " already declared but not configured, message ignored";
			return;
		}

		if (erp1Message.rorg() != CRorgs::kRPS_Telegram)
		{
			YADOMS_LOG(information) << "Data telegram received, but device " << deviceId << " unknown, message ignored";
			return;
		}

		if (!m_pairingHelper->isPairing())
			return;

		// Declare only RPS devices on data telegram (other devices have pairing procedure)
		declareDeviceWithoutProfile(deviceId);
		stopPairing(deviceId + "(set profile before use)");
		return;
	}

	const auto& device = m_devices[deviceId];

	auto keywordsToHistorize = device->states(static_cast<unsigned char>(erp1Message.rorg()),
											  erp1UserData,
											  erp1Status,
											  m_senderId,
											  m_messageHandler);
	if (keywordsToHistorize.empty())
	{
		YADOMS_LOG(information) << "Received message for id " << deviceId << ", but nothing to historize";
		return;
	}

	addSignalPower(keywordsToHistorize,
				   deviceId,
				   dbmToSignalPower(erp1Message.dBm()));

	YADOMS_LOG(information) << "Received message for id " << deviceId << " : ";
	for (const auto& kw : keywordsToHistorize)
	{
		YADOMS_LOG(information) << "  - " << kw->getKeyword() << " = " << kw->formatValue();
	}

	m_api->historizeData(deviceId, keywordsToHistorize);
}

void CEnOcean::processEvent(const boost::shared_ptr<const message::CEsp3ReceivedPacket>& esp3Packet)
{
	if (esp3Packet->header().dataLength() < 1)
		throw CProtocolException((boost::format("RadioERP1 message : wrong data size (%1%, < 1)") % esp3Packet->header().dataLength()).str());

	enum
	{
		SA_RECLAIM_NOT_SUCCESSFUL = 0x01,
		SA_CONFIRM_LEARN = 0x02,
		SA_LEARN_ACK = 0x03,
		CO_READY = 0x04,
		CO_EVENT_SECUREDEVICES = 0x05,
		CO_DUTYCYCLE_LIMIT = 0x06,
		CO_TRANSMIT_FAILED = 0x07
	};

	const auto eventCode = esp3Packet->data()[0];
	YADOMS_LOG(information) << "Event " << eventCode << " received";
}

std::string CEnOcean::processUTE(message::radioErp1::CReceivedMessage& erp1Message)
{
	const auto isReversed = message::radioErp1::CUTEGigaConceptReversedRequest::isCGigaConceptReversedUteMessage(erp1Message);

	const auto uteMessage = isReversed
		? boost::make_shared<message::radioErp1::CUTEGigaConceptReversedRequest>(erp1Message)
		: boost::make_shared<message::radioErp1::CUTERequest>(erp1Message);

	switch (uteMessage->teachInRequest()) // NOLINT(clang-diagnostic-switch-enum)
	{
	case message::radioErp1::CUTERequest::ETeachInRequest::kTeachInRequest:
	case message::radioErp1::CUTERequest::ETeachInRequest::kNotSpecified:
	{
		if (uteMessage->command() != message::radioErp1::CUTERequest::kTeachInQuery)
		{
			YADOMS_LOG(information) << "UTE message : command type " << static_cast<unsigned int>(uteMessage->command()) <<
				" not supported, message ignored";
			return {};
		}

		const auto deviceId = uteMessage->senderId();
		const auto profile = CProfileHelper(uteMessage->rorg(), uteMessage->func(), uteMessage->type());
		const auto manufacturerName = CManufacturers::name(uteMessage->manufacturerId());

		if (m_devices.find(deviceId) == m_devices.end())
		{
			try
			{
				const auto& device = declareDevice(deviceId,
												   profile,
												   manufacturerName,
												   generateModel(std::string(), manufacturerName, profile));

				if (!device)
					return {};

				m_api->updateDeviceConfiguration(deviceId,
												 CDeviceConfigurationHelper(profile, manufacturerName).configuration());
			}
			catch (std::exception& e)
			{
				YADOMS_LOG(error) << "Fail to declare device (Universal teachin) : " << e.what();
				sendUTEAnswer(message::radioErp1::CUTEResponse::kRequestNotAccepted,
							  uteMessage,
							  isReversed,
							  deviceId);
				return {};
			}
		}

		sendUTEAnswer(message::radioErp1::CUTEResponse::kRequestAccepted,
					  uteMessage,
					  isReversed,
					  deviceId);

		// Need to wait a bit before ask initial state (while EnOcean chip record its new association ?)
		boost::this_thread::sleep(boost::posix_time::milliseconds(500));
		m_devices[deviceId]->readInitialState(m_senderId,
											  m_messageHandler);

		return m_devices.at(deviceId)->title();
	}
	case message::radioErp1::CUTERequest::ETeachInRequest::kTeachInDeletionRequest:
	{
		removeDevice(uteMessage->senderId());

		sendUTEAnswer(message::radioErp1::CUTEResponse::kRequestAccepted,
					  uteMessage,
					  isReversed,
					  uteMessage->senderId());
		return {};
	}
	default:
	{
		YADOMS_LOG(information) << "UTE message : teach-in request type " << static_cast<int>(uteMessage->teachInRequest()) <<
			" not supported, message ignored";
		return {};
	}
	}
}

bool CEnOcean::sendUTEAnswer(message::radioErp1::CUTEResponse::EResponse response,
							 const boost::shared_ptr<const message::radioErp1::CUTERequest>& uteMessage,
							 bool isReversed,
							 const std::string& deviceId)
{
	if (!uteMessage->teachInResponseExpected())
		return true;

	const auto sendMessage = isReversed
		? boost::make_shared<message::radioErp1::CUTEGigaConceptReversedAnswer>(m_senderId,
																				deviceId,
																				static_cast<unsigned char>(0),
																				uteMessage->bidirectionalCommunication(),
																				response,
																				uteMessage->channelNumber(),
																				uteMessage->manufacturerId(),
																				uteMessage->type(),
																				uteMessage->func(),
																				uteMessage->rorg())
		: boost::make_shared<message::radioErp1::CUTEResponse>(m_senderId,
															   deviceId,
															   static_cast<unsigned char>(0),
															   uteMessage->bidirectionalCommunication(),
															   response,
															   uteMessage->channelNumber(),
															   uteMessage->manufacturerId(),
															   uteMessage->type(),
															   uteMessage->func(),
															   uteMessage->rorg());

	message::response::CReceivedMessage::EReturnCode returnCode;
	if (!m_messageHandler->send(*sendMessage,
								[](const boost::shared_ptr<const message::CEsp3ReceivedPacket>& esp3Packet)
	{
		return esp3Packet->header().packetType() == message::RESPONSE;
	},
								[&](const boost::shared_ptr<const message::CEsp3ReceivedPacket>& esp3Packet)
	{
		returnCode = message::response::CReceivedMessage(esp3Packet).returnCode();
	}))
		throw CProtocolException("Unable to send UTE response, timeout waiting acknowledge");

	if (returnCode != message::response::CReceivedMessage::RET_OK)
	{
		YADOMS_LOG(error) << "TeachIn response not successfully acknowledged : " << returnCode;
		return false;
	}

	return true;
}

boost::shared_ptr<IType> CEnOcean::declareDevice(const std::string& deviceId,
												 const CProfileHelper& profile,
												 const std::string& manufacturer,
												 const std::string& model)
{
	auto device = createDevice(deviceId,
							   profile);

	const auto modelLabel = generateModel(model,
										  manufacturer,
										  profile);

	const auto keywordsToDeclare = device->allHistorizers();
	if (keywordsToDeclare.empty())
	{
		std::stringstream s;
		s << "Can not declare device id " << deviceId
			<< " (" << profile.profile()
			<< ") : no keyword to declare";
		throw std::logic_error(s.str());
	}

	if (m_devices.find(deviceId) != m_devices.end())
		throw std::logic_error("Device " + deviceId + " already exist");

	m_api->declareDevice(deviceId,
						 modelLabel,
						 modelLabel,
						 keywordsToDeclare);

	YADOMS_LOG(information) << "New device declared : ";
	YADOMS_LOG(information) << "  - Id           : " << deviceId;
	YADOMS_LOG(information) << "  - Profile      : " << profile.profile();
	YADOMS_LOG(information) << "  - Manufacturer : " << manufacturer;
	YADOMS_LOG(information) << "  - Model        : " << modelLabel;
	YADOMS_LOG(information) << "  - RORG         : " << CRorgs::createRorg(profile.rorg())->title();
	YADOMS_LOG(information) << "  - FUNC         : " << CRorgs::createRorg(profile.rorg())->createFunc(profile.func())->title();
	YADOMS_LOG(information) << "  - TYPE         : " << device->title();

	m_devices[deviceId] = device;
	return device;
}

void CEnOcean::removeDevice(const std::string& deviceId)
{
	if (m_api->deviceExists(deviceId))
		m_api->removeDevice(deviceId);

	processDeviceRemoved(deviceId);
}

void CEnOcean::requestDongleVersion()
{
	message::CRequestDongleVersionCommand cmd(m_messageHandler);
	cmd.sendAndReceive();

	m_senderId = cmd.chipId();
}

void CEnOcean::enableSmartAckPostMaster(const bool enable) const
{
	const message::smart_ack::CEnablePostMasterCommand cmd(m_messageHandler);
	cmd.sendAndReceive(enable ? 20 : 0);
}

void CEnOcean::readSmartAckLearnMode() const
{
	message::smart_ack::CReadLearnModeCommand cmd(m_messageHandler);
	cmd.sendAndReceive();
}

std::vector<boost::shared_ptr<message::smart_ack::CClient>> CEnOcean::readSmartAckLearnedClients() const
{
	message::smart_ack::CReadLearnedClientsCommand cmd(m_messageHandler);
	cmd.sendAndReceive();
	return cmd.clients();
}

void CEnOcean::readSmartAckClientMailboxStatus(const boost::shared_ptr<message::smart_ack::CClient>& smartAckClient) const
{
	message::smart_ack::CReadClientMailboxStatusCommand cmd(m_messageHandler);
	cmd.sendAndReceive(smartAckClient->id(),
					   smartAckClient->controllerId());
}

void CEnOcean::startPairing(const boost::shared_ptr<yApi::IYPluginApi>& api)
{
	if (m_pairingHelper->start())
	{
		m_progressPairingCount = PairingProgressNbMaxPeriods;
		m_pairingQuery->reportProgress(1.0f, "customLabels.pairing.pairing");
		m_progressPairingTimer = api->getEventHandler().createTimer(kProgressPairingTimer,
																	shared::event::CEventTimer::kOneShot,
																	boost::posix_time::seconds(PairingPeriodTimeSeconds));
	}
	else if (m_progressPairingTimer)
	{
		m_progressPairingTimer.reset();
	}
}

void CEnOcean::stopPairing(const std::string& pairedDeviceTitle)
{
	m_pairingHelper->stop();
	m_progressPairingTimer->stop();

	if (m_pairingQuery)
	{
		m_pairingQuery->reportProgress(99.0f, pairedDeviceTitle.empty()
									   ? "customLabels.pairing.noDevicePaired"
									   : "customLabels.pairing.devicePaired");

		m_pairingQuery->sendSuccess(shared::CDataContainer::make());
	}
	m_pairingQuery.reset();
}

