#include "stdafx.h"
#include "RfxcomFactory.h"
#include <shared/communication/AsyncSerialPort.h>
#include "Transceiver.h"
#include "RfxComReceiveBufferHandler.h"


CRfxcomFactory::~CRfxcomFactory()
{
}


boost::shared_ptr<shared::communication::IAsyncPort> CRfxcomFactory::constructPort(
   const IRfxcomConfiguration& configuration,
   shared::event::CEventHandler& eventHandler,
   int evtPortConnectionId,
   int evtPortDataReceived)
{
   boost::shared_ptr<shared::communication::IAsyncPort> port(new shared::communication::CAsyncSerialPort(
      configuration.getSerialPort(),
      boost::asio::serial_port_base::baud_rate(38400)));

   port->subscribeForConnectionEvents(eventHandler, evtPortConnectionId);

   boost::shared_ptr<shared::communication::IReceiveBufferHandler> receiveBufferHandler(new CRfxcomReceiveBufferHandler(eventHandler, evtPortDataReceived));
   port->setReceiveBufferHandler(receiveBufferHandler);

   return port;
}

boost::shared_ptr<ITransceiver> CRfxcomFactory::constructTransceiver()
{
   boost::shared_ptr<ITransceiver> transceiver(new CTransceiver());
   return transceiver;
}
