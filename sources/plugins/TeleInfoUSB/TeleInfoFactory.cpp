#include "stdafx.h"
#include "TeleInfoFactory.h"
#include <shared/Log.h>
#include <shared/communication/AsyncSerialPort.h>
#include "Transceiver.h"

CTeleInfoFactory::~CTeleInfoFactory()
{
}


boost::shared_ptr<shared::communication::IAsyncPort> CTeleInfoFactory::constructPort(
   const ITeleInfoConfiguration& configuration,
   shared::event::CEventHandler& eventHandler,
   boost::shared_ptr<CTeleInfoReceiveBufferHandler> receiveBufferHandler,
   int evtPortConnectionId)
{
   boost::shared_ptr<shared::communication::IAsyncPort> port;

      YADOMS_LOG(information) << "Connecting TeleInfo on serial port " << configuration.getSerialPort() << "...";
      port.reset(new shared::communication::CAsyncSerialPort(
         configuration.getSerialPort(),
         boost::asio::serial_port_base::baud_rate( 1200 ),
         boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::even),
         boost::asio::serial_port_base::character_size(7),
         boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
         boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none)
		 ));

   port->subscribeForConnectionEvents(eventHandler, evtPortConnectionId);

   port->setReceiveBufferHandler(receiveBufferHandler);

   return port;
}

boost::shared_ptr<CTeleInfoReceiveBufferHandler> CTeleInfoFactory::GetBufferHandler( shared::event::CEventHandler& eventHandler, int evtPortDataReceived )
{
	boost::shared_ptr<CTeleInfoReceiveBufferHandler> receiveBufferHandler(new CTeleInfoReceiveBufferHandler(eventHandler, evtPortDataReceived));
	return receiveBufferHandler;
}

boost::shared_ptr<ITransceiver> CTeleInfoFactory::constructTransceiver()
{
   boost::shared_ptr<ITransceiver> transceiver(new CTransceiver());
   return transceiver;
}