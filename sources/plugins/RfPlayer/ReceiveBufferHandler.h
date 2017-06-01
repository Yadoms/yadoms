#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/communication/IReceiveBufferHandler.h>
#include <shared/communication/Buffer.hpp>

#include "frames/incoming/Frame.h"
#include "IMessageHandler.h"

//--------------------------------------------------------------
/// \brief	Receive buffer handler for RfPlayer
///
/// This class manages the RfPlayer receive buffer.
/// A message is considered complete from its content (see protocol specification, either ASCII or binary format)
//--------------------------------------------------------------
class CReceiveBufferHandler : public shared::communication::IReceiveBufferHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	                  Constructor
   /// \param[in] messageHandler The called message handler when a message is received
   //--------------------------------------------------------------
   CReceiveBufferHandler(boost::shared_ptr<IMessageHandler> messageHandler);

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
   /// \brief	   Check if we got a complete message
   /// \return    true if a message is complete
   //--------------------------------------------------------------
   bool isComplete();

   //--------------------------------------------------------------
   /// \brief	   Get the binary frame size (only if current frame is synchronized and binary)
   /// \return    The binary frame size
   //--------------------------------------------------------------
   unsigned int getCurrentBinaryFrameSize();

   //--------------------------------------------------------------
   /// \brief	   Pop the next message from the receive buffer
   /// \return    The next complete message
   //--------------------------------------------------------------
   boost::shared_ptr<frames::incoming::CFrame> popNextMessage();

   //--------------------------------------------------------------
   /// \brief	   Identifies the first frame in buffer
   /// \return    The frame type
   //--------------------------------------------------------------
   frames::incoming::EFrameType identifyFrameType();

   //--------------------------------------------------------------
   /// \brief	   Sync buffer to start of frame
   /// \return    true if SYNC pattern has been found, false in other cases
   //--------------------------------------------------------------
   bool syncToStartOfFrame();
private:
   //--------------------------------------------------------------
   /// \brief	Buffer content
   //--------------------------------------------------------------
   std::vector<unsigned char> m_content;

   //--------------------------------------------------------------
   /// \brief	The message handler
   //--------------------------------------------------------------
   boost::shared_ptr<IMessageHandler> m_messageHandler;
};

