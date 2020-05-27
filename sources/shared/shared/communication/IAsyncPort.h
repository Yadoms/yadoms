#pragma once
#include <shared/event/EventHandler.hpp>
#include "IReceiveBufferHandler.h"
#include "Buffer.hpp"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This interface manage a asynchronous communication port.
      ///
      /// An implementation of this interface is responsible to manage the
      /// connection (auto-retry when connection fails), and the
      /// communication (synchronous send, asynchronous receive)
      ///
      /// It uses CEventHandler to :
      /// - notify for connect/disconnect events (optional)
      /// - notify for data received (optional)
      ///
      /// When data is received, data are transfered to the receive buffer
      /// handler, who is responsible to decide when to notify event handler
      /// that a full message is received.
      ///
      //--------------------------------------------------------------
      class IAsyncPort
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~IAsyncPort()
         {
         }

         //--------------------------------------------------------------
         /// \brief	Set the receive max buffer size
         /// \param [in] size  The new buffer size
         //--------------------------------------------------------------
         virtual void setReceiveBufferMaxSize(std::size_t size) = 0;

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
         /// \brief	                     Subscribe to the connection/disconnection events
         /// \param [in] forEventHandler  The event handler to notify for these events
         /// \param [in] forId            The event id to send for these events (set kNoEvent to unsubscribe)
         /// \throw exception::CInvalidParameter if try to subscribe on event for which a subscription already exists (user must unsubscribe first)
         /// \note The raised event contains a boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> data
         /// \note Must be called before start
         //--------------------------------------------------------------
         virtual void subscribeForConnectionEvents(event::CEventHandler& forEventHandler, int forId) = 0;

         //--------------------------------------------------------------
         /// \brief	                     Set the receive buffer handler (= received messages subscription)
         /// \param [in] handler          The receive buffer handler
         //--------------------------------------------------------------
         virtual void setReceiveBufferHandler(boost::shared_ptr<IReceiveBufferHandler> handler) = 0;

         //--------------------------------------------------------------
         /// \brief	                     Send a buffer on the port (synchronously)
         /// \param [in] buffer           The buffer to send
         /// \throw                       CPortException if error
         //--------------------------------------------------------------
         virtual void send(const CByteBuffer& buffer) = 0;

         //--------------------------------------------------------------
         /// \brief	                     Send text on the port (synchronously)
         /// \param [in] content          The text to write
         /// \throw                       CPortException if error
         //--------------------------------------------------------------
         virtual void sendText(const std::string & content) = 0;

         //--------------------------------------------------------------
         /// \brief	                     Flush the receive buffer
         //--------------------------------------------------------------
         virtual void flush() = 0;
      };
   }
} // namespace shared::communication