#include "stdafx.h"
#include "RfxcomFactory.h"
#include "SerialPort.h"
#include "Transceiver.h"


CRfxcomFactory::~CRfxcomFactory()
{
}


boost::shared_ptr<IPort> CRfxcomFactory::constructPort(
   const IRfxcomConfiguration& configuration,
   shared::event::CEventHandler& eventHandler,
   int evtPortConnectionId,
   int evtPortDataReceived)
{
   boost::shared_ptr<IPort> port(new CSerialPort(configuration.getSerialPort(), boost::asio::serial_port_base::baud_rate(38400)));
   port->subscribeConnectionState(eventHandler, evtPortConnectionId);//TODO à revoir, les abonnements doivent être réalisés avant le démarrage du port COM sinon risque de perdre des evt
   port->subscribeReceiveData(eventHandler, evtPortDataReceived);

   return port;
}

boost::shared_ptr<ITransceiver> CRfxcomFactory::constructTransceiver(
   boost::shared_ptr<IPort> port)
{
   boost::shared_ptr<ITransceiver> transceiver(new CTransceiver(port));
   return transceiver;
}
