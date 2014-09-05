#include "stdafx.h"
#include "RfxcomFactory.h"
#include "AsyncSerialPort.h"
#include "Transceiver.h"
#include "PortLogger.h"


CRfxcomFactory::~CRfxcomFactory()
{
}


boost::shared_ptr<IAsyncPort> CRfxcomFactory::constructPort(
   const IRfxcomConfiguration& configuration,
   shared::event::CEventHandler& eventHandler,
   int evtPortConnectionId,
   int evtPortDataReceived,
   boost::shared_ptr<IPortLogger> logger)
{
   boost::shared_ptr<IAsyncPort> port(new CAsyncSerialPort(configuration.getSerialPort(), boost::asio::serial_port_base::baud_rate(38400)));
   port->subscribeConnectionState(eventHandler, evtPortConnectionId);
   port->subscribeReceiveData(eventHandler, evtPortDataReceived);
   port->setLogger(logger);

   return port;
}

boost::shared_ptr<ITransceiver> CRfxcomFactory::constructTransceiver()
{
   boost::shared_ptr<ITransceiver> transceiver(new CTransceiver());
   return transceiver;
}

boost::shared_ptr<IPortLogger> CRfxcomFactory::constructPortLogger()
{
   boost::shared_ptr<IPortLogger> portLogger(new CPortLogger("RfxCom"));
   return portLogger;
}