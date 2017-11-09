#include "stdafx.h"
#include "RfxcomFactory.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/AsyncTcpClient.h>
#include "Transceiver.h"
#include "RfxComReceiveBufferHandler.h"
#include "RfxcomFirmwareUpdater.h"
#include <shared/Log.h>

CRfxcomFactory::~CRfxcomFactory()
{
}


boost::shared_ptr<shared::communication::IAsyncPort> CRfxcomFactory::constructPort(const IRfxcomConfiguration& configuration,
                                                                                   shared::event::CEventHandler& eventHandler,
                                                                                   int evtPortConnectionId,
                                                                                   int evtPortDataReceived) const
{
   boost::shared_ptr<shared::communication::IAsyncPort> port;
   if (configuration.comIsEthernet())
   {
      YADOMS_LOG(information) << "Connecting RFXCom on network at " << configuration.getEthernetAddress() << ", port " << configuration.getEthernetPort() << "...";
      port = boost::make_shared<shared::communication::CAsyncTcpClient>(configuration.getEthernetAddress(),
                                                                        configuration.getEthernetPort());
   }
   else
   {
      YADOMS_LOG(information) << "Connecting RFXCom on serial port " << configuration.getSerialPort() << "...";
      port = boost::make_shared<shared::communication::CAsyncSerialPort>(configuration.getSerialPort(),
                                                                         boost::asio::serial_port_base::baud_rate(38400));
   }

   port->subscribeForConnectionEvents(eventHandler, evtPortConnectionId);

   auto receiveBufferHandler = boost::make_shared<CRfxcomReceiveBufferHandler>(eventHandler,
                                                                               evtPortDataReceived);
   port->setReceiveBufferHandler(receiveBufferHandler);

   return port;
}

boost::shared_ptr<ITransceiver> CRfxcomFactory::constructTransceiver() const
{
   return boost::make_shared<CTransceiver>();
}

boost::shared_ptr<IRfxcomFirmwareUpdater> CRfxcomFactory::constructFirmwareUpdater(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                                   boost::shared_ptr<yApi::IExtraQuery> extraQuery,
                                                                                   const std::string& serialPort) const
{
   return boost::make_shared<CRfxcomFirmwareUpdater>(api,
                                                     extraQuery,
                                                     serialPort);
}

