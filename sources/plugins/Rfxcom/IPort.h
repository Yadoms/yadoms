#pragma once

#include "shared/event/EventHandler.hpp"

//--------------------------------------------------------------
/// \brief	This class manage a communication port
//--------------------------------------------------------------
class IPort
{  
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IPort() {}

   //--------------------------------------------------------------
   /// \brief	Establish the connection
   /// \return true if connected, false else
   //--------------------------------------------------------------
   virtual bool connect() = 0;

   //--------------------------------------------------------------
   /// \brief	Close the connection
   //--------------------------------------------------------------
   virtual void disconnect() = 0;

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
   //--------------------------------------------------------------
   virtual void subscribeConnectionState(boost::shared_ptr<shared::event::CEventHandler> forEventHandler, int forId) = 0;

   //--------------------------------------------------------------
   /// \brief	                     Subscribe/Unsubscribe to the data receive events
   /// \param [in] forEventHandler  The event handler to notify for these events
   /// \param [in] forId            The event id to send for these events (set kNoEvent to unsubscribe)
   /// \throw shared::exception::CInvalidParameter if try to subscribe on event for which a subscription already exists (user must unsubscribe first)
   /// \note The raised event contains the read data as string
   //--------------------------------------------------------------
   virtual void subscribeReceiveData(boost::shared_ptr<shared::event::CEventHandler> forEventHandler, int forId) = 0;

   //--------------------------------------------------------------
   /// \brief	                  Flush the input buffer
   /// \note This function flush the input buffer, and erase pending received data events
   //--------------------------------------------------------------
   virtual void flush() = 0;

   //--------------------------------------------------------------
   /// \brief	                  Send a buffer on the port (synchronously)
   /// \param [in] buffer        The buffer to send
   /// \throw                    CPortException if error
   //--------------------------------------------------------------
   virtual void send(const boost::asio::const_buffer& buffer) = 0;

   //--------------------------------------------------------------
   /// \brief	                  Send a string message on the port
   /// \param [in] buffer        The buffer to send
   //--------------------------------------------------------------
   virtual void asyncSend(const boost::asio::const_buffer& buffer) = 0;

   //--------------------------------------------------------------
   /// \brief	                  Receive a buffer from the port (synchronously)
   /// \param [in] buffer        The buffer to fill with received data
   /// \throw                    CPortException if error
   //--------------------------------------------------------------
   virtual void receive(boost::asio::mutable_buffer& buffer) = 0;
};



