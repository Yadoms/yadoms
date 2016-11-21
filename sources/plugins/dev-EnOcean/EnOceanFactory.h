#pragma once

#include "IEnOceanConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include "IEnOceanReceiveThread.h"
#include "IMessageHandler.h"


//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CEnOceanFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CEnOceanFactory();

   //--------------------------------------------------------------
   /// \brief	                           Create a port instance and connect to the dongle
   /// \param[in] configuration           Plugin instance configuration (contains the serial port)
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortConnectionId     The event id raised on connection events
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(const IEnOceanConfiguration& configuration,
                                                                             shared::event::CEventHandler& eventHandler,
                                                                             int evtPortConnectionId,
                                                                             int evtPortDataReceived);

   //--------------------------------------------------------------
   /// \brief	                           Create the receive thread
   /// \param[in] eventHandler            The event handler to receive notifications in main thread
   /// \param[in] mainEvtPortConnection   The event id raised on connection events on main event handler
   /// \param[in] mainEvtPortDataReceived The event id raised on data receive events on main event handler
   /// \return                            The created thread
   //--------------------------------------------------------------
   static boost::shared_ptr<IEnOceanReceiveThread> constructReceiverThread(shared::event::CEventHandler& eventHandler,
                                                                           int mainEvtPortConnection,
                                                                           int mainEvtPortDataReceived);

   //--------------------------------------------------------------
   /// \brief	                           Create the message handler
   /// \param[in] port                    Serial port used to send/receive to dongle
   /// \param[in] receiverThread          The receiver thread
   /// \return                            The message handler
   //--------------------------------------------------------------
   static boost::shared_ptr<IMessageHandler> constructMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port,
                                                                     boost::shared_ptr<IEnOceanReceiveThread> receiverThread);
};

