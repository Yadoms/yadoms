#include "stdafx.h"
#include "SomfySituoFactory.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/AsciiBufferLogger.h>
#include <shared/communication/EOFReceiveBufferHandler.h>

CSomfySituoFactory::~CSomfySituoFactory()
{
}

boost::shared_ptr<shared::communication::IAsyncPort> CSomfySituoFactory::constructPort(const ISomfySituoConfiguration& configuration,
	shared::event::CEventHandler& eventHandler,
	boost::shared_ptr<shared::communication::IReceiveBufferHandler> receiveBufferHandler,
	int evtPortConnectionId)
{
	YADOMS_LOG(information) << "Connecting UPS on serial port " << configuration.getSerialPort() << "...";
	auto port = boost::make_shared<shared::communication::CAsyncSerialPort>(configuration.getSerialPort(),
		boost::asio::serial_port_base::baud_rate(9600),
		boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
		boost::asio::serial_port_base::character_size(8),
		boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
		boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

	port->subscribeForConnectionEvents(eventHandler, evtPortConnectionId);

	port->setReceiveBufferHandler(receiveBufferHandler);

	return port;
}

boost::shared_ptr<CSomfySituoReceiveBufferHandler> CSomfySituoFactory::GetBufferHandler(shared::event::CEventHandler& eventHandler,
	int evtPortDataReceived,
	const bool isDeveloperMode)
{
	boost::shared_ptr<shared::communication::IBufferLogger> logger;

	logger = boost::make_shared<shared::communication::CAsciiBufferLogger>("trace");

	return boost::make_shared<CSomfySituoReceiveBufferHandler>(eventHandler,
		evtPortDataReceived,
		logger,
		isDeveloperMode);
}