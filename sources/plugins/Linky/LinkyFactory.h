#pragma once

#include "ILinkyConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include "IDecoder.h"

//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CLinkyFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLinkyFactory();

   //--------------------------------------------------------------
   /// \brief	                           Create a port instance and connect to the Linky Receiver
   /// \param[in] configuration           Plugin instance configuration (contains the serial port)
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortConnectionId     The event id raised on connection events
   /// \param[in] receiveBufferHandler    Receive buffer handler
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(const ILinkyConfiguration& configuration,
                                                                             shared::event::CEventHandler& eventHandler,
                                                                             boost::shared_ptr<shared::communication::IReceiveBufferHandler> receiveBufferHandler,
                                                                             int evtPortConnectionId);

   //--------------------------------------------------------------
   /// \brief	                          Create a decoder instance
   /// \param [in] api                    Plugin execution context (Yadoms API)
   /// \return                            The created transceiver
   //--------------------------------------------------------------
   static boost::shared_ptr<IDecoder> constructDecoder(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                          Return the buffer handler of the linky
   /// \param [in] eventhandler           eventHandler used by the bufferHandler
   /// \param [in] evtPortDataReceived    event sent when a packet is ready
   /// \param [in] developerMode          running in developer mode
   /// \return                            The Buffer handler
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IReceiveBufferHandler> GetBufferHandler(shared::event::CEventHandler& eventHandler,
                                                                                           int evtPortDataReceived);
};