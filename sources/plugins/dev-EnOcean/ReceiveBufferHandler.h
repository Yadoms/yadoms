#pragma once
#include <shared/communication/IReceiveBufferHandler.h>
#include "message/ReceivedMessage.h"

//--------------------------------------------------------------
/// \brief	Receive buffer handler for EnOcean
///
/// This class manages the EnOcean receive buffer.
/// A message is considered complete from its content (see protocol specification)
//--------------------------------------------------------------
class CReceiveBufferHandler : public shared::communication::IReceiveBufferHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   /// \param[in] messageHandler          The called message handler when a message is received
   //--------------------------------------------------------------
   explicit CReceiveBufferHandler(boost::shared_ptr<IMessageHandler> messageHandler);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CReceiveBufferHandler();

   // IReceiveBufferHandler implementation
   void push(const shared::communication::CByteBuffer& buffer) override;
   void flush() override;
   // [END] IReceiveBufferHandler implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Returns the message if complete
   /// \return                      The complete message, or null
   //--------------------------------------------------------------
   boost::shared_ptr<const message::CEsp3ReceivedPacket> getCompleteMessage();

private:
   //--------------------------------------------------------------
   /// \brief	Last received time for timout computation
   //--------------------------------------------------------------
   boost::posix_time::ptime m_lastReceivedTime;

   //--------------------------------------------------------------
   /// \brief	Buffer content
   //--------------------------------------------------------------
   std::vector<unsigned char> m_content;

   //--------------------------------------------------------------
   /// \brief	The called message handler when a message is received
   //--------------------------------------------------------------
   boost::shared_ptr<IMessageHandler> m_messageHandler;
};

