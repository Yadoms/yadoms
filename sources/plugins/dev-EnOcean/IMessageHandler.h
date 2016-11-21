#pragma once
#include "message/SendMessage.h"
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
   /// \param [in] isExpectedMessageFct  Lambda to identify the expected message. Must return true if received message is the expected one. (called by different thread).
   /// \param [in] onReceiveFct     Lambda to process received message. (called by different thread).
   /// \return                      false if timeout waiting answer
   //--------------------------------------------------------------
   virtual bool send(const message::CEsp3SendPacket& sendMessage,
                     boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> isExpectedMessageFct,
                     boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> onReceiveFct) = 0;
};

