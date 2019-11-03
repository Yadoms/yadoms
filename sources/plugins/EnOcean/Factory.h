#pragma once
#include "IConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/IBufferLogger.h>
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IMessageHandler.h"
#include "PairingHelper.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFactory();

   //--------------------------------------------------------------
   /// \brief	                           Create a port instance and connect to the dongle
   /// \param[in] configuration           Plugin instance configuration (contains the serial port)
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(const IConfiguration& configuration);

   //--------------------------------------------------------------
   /// \brief	                           Create the receive buffer handler
   /// \param[in] messageHandler          The message handler to redirect a received message
   /// \return                            The receive buffer handler
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IReceiveBufferHandler> constructReceiveBufferHandler(boost::shared_ptr<IMessageHandler> messageHandler);

   //--------------------------------------------------------------
   /// \brief	                           Create the buffer logger (use to log in/out messages)
   /// \return                            The buffer loger
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IBufferLogger> constructBufferLogger();

   //--------------------------------------------------------------
   /// \brief	                           Create the message handler
   /// \param[in] port                    Serial port used to send/receive to dongle
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   /// \param[in] bufferLogger            The buffer loger
   /// \return                            The message handler
   //--------------------------------------------------------------
   static boost::shared_ptr<IMessageHandler> constructMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port,
                                                                     shared::event::CEventHandler& eventHandler,
                                                                     int evtPortDataReceived,
                                                                     boost::shared_ptr<shared::communication::IBufferLogger> bufferLogger);

   //--------------------------------------------------------------
   /// \brief	                           Create the pairing helper
   /// \param[in] api                     The Yadoms plugin API
   /// \param[in] pairingMode             Pairing mode
   /// \return                            The pairing helper
   //--------------------------------------------------------------
   static boost::shared_ptr<CPairingHelper> constructPairingHelper(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                   IPairingHelper::EPairingMode pairingMode);
};
