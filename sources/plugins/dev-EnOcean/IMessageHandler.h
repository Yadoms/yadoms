#pragma once
#include "message/Esp3SendPacket.h"
#include "message/Esp3ReceivedPacket.h"

//--------------------------------------------------------------
/// \brief	Interface of message handler
//--------------------------------------------------------------
class IMessageHandler
{
public:
   virtual ~IMessageHandler()
   {
   }

   //--------------------------------------------------------------
   /// \brief	                     Send a message to EnOcean dongle
   /// \param [in] sendMessage      message to send
   //--------------------------------------------------------------
   virtual void send(message::CEsp3SendPacket& sendMessage) = 0;

   //--------------------------------------------------------------
   /// \brief	                     Send a message to EnOcean dongle and wait answer
   /// \param [in] sendMessage      message to send
   /// \param [in] isExpectedMessageFct  Lambda to identify the expected message. Must return true if received message is the expected one. (called by different thread).
   /// \param [in] onReceiveFct     Lambda to process received message. (called by different thread).
   /// \return                      false if timeout waiting answer
   //--------------------------------------------------------------
   virtual bool send(message::CEsp3SendPacket& sendMessage,
                     boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> isExpectedMessageFct,
                     boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> onReceiveFct) = 0;

   //--------------------------------------------------------------
   /// \brief	                     Function called when message is received
   /// \param [in] receivedMessage  Received message
   //--------------------------------------------------------------
   virtual void onReceived(boost::shared_ptr<const message::CEsp3ReceivedPacket> receivedMessage) = 0;
};

