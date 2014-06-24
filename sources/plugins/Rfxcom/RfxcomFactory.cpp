#include "stdafx.h"
#include "RfxcomFactory.h"
#include "SerialPort.h"
#include "Transceiver.h"
#include "PortLogger.h"


CRfxcomFactory::~CRfxcomFactory()
{
}


boost::shared_ptr<IPort> CRfxcomFactory::constructPort(
   const IRfxcomConfiguration& configuration,
   shared::event::CEventHandler& eventHandler,
   int evtPortConnectionId,
   int evtPortDataReceived,
   boost::shared_ptr<IPortLogger> logger)
{
   boost::shared_ptr<IPort> port(new CSerialPort(configuration.getSerialPort(), boost::asio::serial_port_base::baud_rate(38400)));
   port->subscribeConnectionState(eventHandler, evtPortConnectionId);
   port->subscribeReceiveData(eventHandler, evtPortDataReceived);
   port->setLogger(logger);
   port->start();

   return port;
}

boost::shared_ptr<ITransceiver> CRfxcomFactory::constructTransceiver(boost::shared_ptr<IPort> port)
{
   boost::shared_ptr<ITransceiver> transceiver(new CTransceiver(port));
   return transceiver;
}

boost::shared_ptr<IPortLogger> CRfxcomFactory::constructPortLogger()
{
   boost::shared_ptr<IPortLogger> portLogger(new CPortLogger("RfxCom"));
   return portLogger;
}