#pragma once

#include <shared/communication/IAsyncPort.h>
#include "ZiBlueConfiguration.h"
#include "IZiBlueMessageHandler.h"

//--------------------------------------------------------------
/// \brief	Configuration of the xpl plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CZiBlueFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CZiBlueFactory();

   //--------------------------------------------------------------
   /// \brief	                           Create a port instance and connect to the dongle
   /// \param[in] configuration           Plugin instance configuration (contains the serial port)
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(const CZiBlueConfiguration& configuration);

   //--------------------------------------------------------------
   /// \brief	                           Create the receive buffer handler
   /// \param[in] messageHandler          The message handler to redirect a received message
   /// \return                            The receive buffer handler
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IReceiveBufferHandler> constructReceiveBufferHandler(boost::shared_ptr<IZiBlueMessageHandler> messageHandler);

   //--------------------------------------------------------------
   /// \brief	                           Create the message handler
   /// \param[in] port                    Serial port used to send/receive to dongle
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   /// \param[in] bufferLogger            The buffer loger
   /// \return                            The message handler
   //--------------------------------------------------------------
   static boost::shared_ptr<IZiBlueMessageHandler> constructMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port, shared::event::CEventHandler& eventHandler, int evtPortDataReceived);


};

