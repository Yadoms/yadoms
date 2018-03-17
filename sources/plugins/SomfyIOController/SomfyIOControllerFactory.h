#pragma once

#include "ISomfyIOControllerConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include "SomfyIOControllerReceiveBufferHandler.h"

//--------------------------------------------------------------
/// \brief	Megatec protocol End Of Frame character
//--------------------------------------------------------------
#define SOMFY_ADAPTER_EOF '\r'

//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CSomfyIOControllerFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSomfyIOControllerFactory();

   //--------------------------------------------------------------
   /// \brief	                           Create a port instance and connect to the UPS
   /// \param[in] configuration           Plugin instance configuration (contains the serial port)
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortConnectionId     The event id raised on connection events
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(const ISomfyIOControllerConfiguration& configuration,
																			 shared::event::CEventHandler& eventHandler,
																			 boost::shared_ptr<shared::communication::IReceiveBufferHandler> receiveBufferHandler,
																			 int evtPortConnectionId);

   //--------------------------------------------------------------
   /// \brief	                          Return the buffer handler of the teleInfo
   /// \param [in] eventhandler           eventHandler used by the bufferHandler
   /// \param [in] evtPortDataReceived    event sent when a packet is ready
   /// \param [in] developerMode          running in developer mode
   /// \return                            The Buffer handler
   //--------------------------------------------------------------
   static boost::shared_ptr<CSomfyIOControllerReceiveBufferHandler> GetBufferHandler(shared::event::CEventHandler& eventHandler,
	   int evtPortDataReceived,
	   const bool isDeveloperMode);
};