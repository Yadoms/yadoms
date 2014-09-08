#pragma once

#include "shared/event/EventHandler.hpp"
#include "IPortLogger.h"
#include "Buffer.hpp"

//--------------------------------------------------------------
/// \brief	This class manage a asynchronous communication port
// After configuring and starting the port management, the
// caller can be notified when connection is established or lost,
// and when data is received, asynchronously.
// The caller can also send data synchronously.
//--------------------------------------------------------------
class IAsyncPort
{  
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IAsyncPort() {}

   //--------------------------------------------------------------
   /// \brief	Set the receive max buffer size
   /// \param [in] size  The new buffer size
   //--------------------------------------------------------------
   virtual void setReceiveBufferSize(std::size_t size) = 0;

   //--------------------------------------------------------------
   /// \brief	Start the port management (try to connect)
   //--------------------------------------------------------------
   virtual void start() = 0;

   //--------------------------------------------------------------
   /// \brief	Stop the port management
   //--------------------------------------------------------------
   virtual void stop() = 0;

   //--------------------------------------------------------------
   /// \brief	Get the connection status
   /// \return true if connected, false else
   //--------------------------------------------------------------
   virtual bool isConnected() const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Subscribe/Unsubscribe to the connection/disconnection events
   /// \param [in] forEventHandler  The event handler to notify for these events
   /// \param [in] forId            The event id to send for these events (set kNoEvent to unsubscribe)
   /// \throw shared::exception::CInvalidParameter if try to subscribe on event for which a subscription already exists (user must unsubscribe first)
   /// \note The raised event contains a bool data : true if port was connected, false if port was disconnected
   /// \note Must be called before start
   //--------------------------------------------------------------
   virtual void subscribeConnectionState(shared::event::CEventHandler& forEventHandler, int forId) = 0;

   //--------------------------------------------------------------
   /// \brief	                     Subscribe/Unsubscribe to the data receive events
   /// \param [in] forEventHandler  The event handler to notify for these events
   /// \param [in] forId            The event id to send for these events (set kNoEvent to unsubscribe)
   /// \throw shared::exception::CInvalidParameter if try to subscribe on event for which a subscription already exists (user must unsubscribe first)
   /// \note The raised event contains the read data as string
   /// \note Must be called before start
   //--------------------------------------------------------------
   virtual void subscribeReceiveData(shared::event::CEventHandler& forEventHandler, int forId) = 0;

   //--------------------------------------------------------------
   /// \brief	                     Set the logger
   /// \param [in] logger           Logger to used (can be null is not log is expected)
   //--------------------------------------------------------------
   virtual void setLogger(boost::shared_ptr<IPortLogger> logger) = 0;

   //--------------------------------------------------------------
   /// \brief	                     Send a buffer on the port (synchronously)
   /// \param [in] buffer           The buffer to send
   /// \throw                       CPortException if error
   //--------------------------------------------------------------
   virtual void send(const CByteBuffer& buffer) = 0;
};



