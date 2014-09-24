#include "stdafx.h"
#include "RfxcomFactory.h"
#include "AsyncSerialPort.h"
#include "Transceiver.h"
#include "RfxComReceiveBufferHandler.h"


CRfxcomFactory::~CRfxcomFactory()
{
}


boost::shared_ptr<IAsyncPort> CRfxcomFactory::constructPort(
   const IRfxcomConfiguration& configuration,
   shared::event::CEventHandler& eventHandler,
   int evtPortConnectionId,
   int evtPortDataReceived)
{
   boost::shared_ptr<IReceiveBufferHandler> receiveBufferHandler(new CRfxcomReceiveBufferHandler(eventHandler, evtPortDataReceived));
   boost::shared_ptr<IAsyncPort> port(new CAsyncSerialPort(
      configuration.getSerialPort(),
      receiveBufferHandler,
      boost::asio::serial_port_base::baud_rate(38400)));
   port->subscribeConnectionState(eventHandler, evtPortConnectionId);

   return port;
}

boost::shared_ptr<ITransceiver> CRfxcomFactory::constructTransceiver()
{
   boost::shared_ptr<ITransceiver> transceiver(new CTransceiver());
   return transceiver;
}
