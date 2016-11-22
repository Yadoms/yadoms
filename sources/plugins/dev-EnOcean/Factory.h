#pragma once

#include "IConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include "IMessageHandler.h"


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
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortConnectionId     The event id raised on connection events
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(const IConfiguration& configuration,
                                                                             shared::event::CEventHandler& eventHandler,
                                                                             int evtPortConnectionId,
                                                                             int evtPortDataReceived);

   //--------------------------------------------------------------
   /// \brief	                           Create the receive buffer handler
   /// \param[in] messageHandler          The message handler to redirect a received message
   /// \return                            The receive buffer handler
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IReceiveBufferHandler> CFactory::constructReceiveBufferHandler(boost::shared_ptr<IMessageHandler> messageHandler);

   //--------------------------------------------------------------
   /// \brief	                           Create the message handler
   /// \param[in] port                    Serial port used to send/receive to dongle
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   /// \return                            The message handler
   //--------------------------------------------------------------
   static boost::shared_ptr<IMessageHandler> constructMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port,
                                                                     shared::event::CEventHandler& eventHandler,
                                                                     int evtPortDataReceived);
};

