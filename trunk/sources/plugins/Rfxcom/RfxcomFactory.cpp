#include "stdafx.h"
#include "RfxcomFactory.h"
#include "SerialPort.h"
#include "Transceiver.h"


CRfxcomFactory::~CRfxcomFactory()
{
}


boost::shared_ptr<IPort> CRfxcomFactory::constructPort(
   const IRfxcomConfiguration& configuration,
   boost::asio::io_service& iOService,
   boost::shared_ptr<shared::event::CEventHandler> eventHandler,
   int evtPortConnectionId,
   int evtPortDataReceived)
{
   boost::shared_ptr<IPort> port(new CSerialPort(iOService, configuration.getSerialPort(), boost::asio::serial_port_base::baud_rate(38400)));
   port->subscribeConnectionState(eventHandler, evtPortConnectionId);
   port->subscribeReceiveData(eventHandler, evtPortDataReceived);

   return port;
}

boost::shared_ptr<ITransceiver> CRfxcomFactory::constructTransceiver(
   boost::shared_ptr<IPort> port)
{
   boost::shared_ptr<ITransceiver> transceiver(new CTransceiver(port));
   return transceiver;
}
