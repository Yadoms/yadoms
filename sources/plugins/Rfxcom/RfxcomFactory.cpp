#include "stdafx.h"
#include "RfxcomFactory.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/AsyncTcpClient.h>
#include "Transceiver.h"
#include "RfxComReceiveBufferHandler.h"
#include <shared/Log.h>

CRfxcomFactory::~CRfxcomFactory()
{
}


boost::shared_ptr<shared::communication::IAsyncPort> CRfxcomFactory::constructPort(
   const IRfxcomConfiguration& configuration,
   shared::event::CEventHandler& eventHandler,
   int evtPortConnectionId,
   int evtPortDataReceived)
{
   boost::shared_ptr<shared::communication::IAsyncPort> port;
   if (configuration.comIsEthernet())
   {
      YADOMS_LOG(information) << "Connecting RFXCom on network at " << configuration.getEthernetAddress() << ", port " << configuration.getEthernetPort() << "..." ;
      port.reset(new shared::communication::CAsyncTcpClient(
         configuration.getEthernetAddress(),
         configuration.getEthernetPort()));
   }
   else
   {
      YADOMS_LOG(information) << "Connecting RFXCom on serial port " << configuration.getSerialPort() << "..." ;
      port.reset(new shared::communication::CAsyncSerialPort(
         configuration.getSerialPort(),
         boost::asio::serial_port_base::baud_rate(38400)));
   }

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
