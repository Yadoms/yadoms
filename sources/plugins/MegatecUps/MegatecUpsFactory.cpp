#include "stdafx.h"
#include "MegatecUpsFactory.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/EOFReceiveBufferHandler.h>


CMegatecUpsFactory::~CMegatecUpsFactory()
{
}


boost::shared_ptr<shared::communication::IAsyncPort> CMegatecUpsFactory::constructPort(const IMegatecUpsConfiguration& configuration,
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

   port->setReceiveBufferHandler(boost::make_shared<shared::communication::CEOFReceiveBufferHandler>(eventHandler,
                                                                                                     evtPortDataReceived,
                                                                                                     MEGATEC_EOF));

   return port;
}

