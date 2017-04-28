#pragma once
#include "frames/incoming/Frame.h"

//--------------------------------------------------------------
/// \brief	Interface for message handler
//--------------------------------------------------------------
class IMessageHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	Virtual destructor
   //--------------------------------------------------------------
   virtual ~IMessageHandler() {}

   //--------------------------------------------------------------
   /// \brief	                     Function called when message is received
   /// \param [in] receivedMessage  Received message
   //--------------------------------------------------------------
   virtual void onReceived(boost::shared_ptr<const frames::incoming::CFrame> receivedMessage) = 0;

   //--------------------------------------------------------------
   /// \brief	                           Send a message to RfPlayer dongle and wait answer
   /// \param [in] sendMessage            message to send
   /// \param [in] isExpectedMessageFct   Lambda to identify the expected message. Must return true if received message is the expected one. (called by different thread).
   /// \param [in] onReceiveFct           Lambda to process received message. (called by different thread).
   /// \return                            false if timeout waiting answer
   //--------------------------------------------------------------
   virtual bool send(const std::string & sendMessage, boost::function<bool(boost::shared_ptr<const frames::incoming::CFrame>)> isExpectedMessageFct, boost::function<void(boost::shared_ptr<const frames::incoming::CFrame>)> onReceiveFct) = 0;

   //--------------------------------------------------------------
   /// \brief	                     Send a message to RfPlayer dongle
   /// \param [in] sendMessage      message to send
   //--------------------------------------------------------------
   virtual void send(const std::string& sendMessage) = 0;
   //--------------------------------------------------------------
   /// \brief	                     Send a buffer to RfPlayer dongle (binary mode)
   /// \param [in] sendMessage      bytes to send
   //--------------------------------------------------------------
   virtual void sendBinary(const shared::communication::CByteBuffer& buffer) = 0;

   //--------------------------------------------------------------
   /// \brief	                        Send a file content to RfPlayer dongle
   /// \param [in] sendMessage         file content
   /// \param [in] onProgressHandler   handler for progression notification
   //--------------------------------------------------------------
   virtual bool sendFile(const std::string & sendMessage, boost::function<void(float)> onProgressHandler) = 0;



};

