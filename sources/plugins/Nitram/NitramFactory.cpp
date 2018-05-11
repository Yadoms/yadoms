#include "stdafx.h"
#include "NitramFactory.h"
#include <shared/communication/AsyncSerialPort.h>
#include "NitramReceiveBufferHandler.h"
#include <shared/Log.h>
#include <shared/communication/BufferLogger.h>

CNitramFactory::~CNitramFactory()
{
}


boost::shared_ptr<shared::communication::IAsyncPort> CNitramFactory::constructPort(const INitramConfiguration& configuration,
                                                                                   shared::event::CEventHandler& eventHandler,
                                                                                   int evtPortConnectionId,
                                                                                   int evtPortDataReceived)
{
   YADOMS_LOG(information) << "Connecting UPS on serial port " << configuration.getSerialPort() << "...";
   auto port = boost::make_shared<shared::communication::CAsyncSerialPort>(configuration.getSerialPort(),
                                                                           boost::asio::serial_port_base::baud_rate(2400),
                                                                           boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                                                                           boost::asio::serial_port_base::character_size(8),
                                                                           boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
                                                                           boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

   port->subscribeForConnectionEvents(eventHandler, evtPortConnectionId);

   boost::shared_ptr<shared::communication::IBufferLogger> logger;

   logger = boost::make_shared<shared::communication::CBufferLogger>("trace");

   port->setReceiveBufferHandler(boost::make_shared<CNitramReceiveBufferHandler>(eventHandler,
                                                                                 evtPortDataReceived,
                                                                                 logger));

   return port;
}

