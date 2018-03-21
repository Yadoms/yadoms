#pragma once

#include "ISomfySituoConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include "SomfySituoReceiveBufferHandler.h"

//--------------------------------------------------------------
/// \brief	Megatec protocol End Of Frame character
//--------------------------------------------------------------
#define SOMFY_ADAPTER_EOF '\r'

//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CSomfySituoFactory
{
public:
	//--------------------------------------------------------------
	/// \brief	    Destructor
	//--------------------------------------------------------------
	virtual ~CSomfySituoFactory();

	//--------------------------------------------------------------
	/// \brief	                           Create a port instance and connect to the UPS
	/// \param[in] configuration           Plugin instance configuration (contains the serial port)
	/// \param[in] eventHandler            The event handler to receive notifications from port
	/// \param[in] evtPortConnectionId     The event id raised on connection events
	/// \return                            The created port
	//--------------------------------------------------------------
	static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(
		const ISomfySituoConfiguration& configuration,
		shared::event::CEventHandler& eventHandler,
		boost::shared_ptr<shared::communication::IReceiveBufferHandler> receiveBufferHandler,
		int evtPortConnectionId);

	//--------------------------------------------------------------
	/// \brief	                          Return the buffer handler of the teleInfo
	/// \param [in] evtPortDataReceived    event sent when a packet is ready
	/// \return                            The Buffer handler
	//--------------------------------------------------------------
	static boost::shared_ptr<CSomfySituoReceiveBufferHandler> GetBufferHandler(shared::event::CEventHandler& eventHandler,
	                                                                           int evtPortDataReceived,
	                                                                           bool isDeveloperMode);
};
