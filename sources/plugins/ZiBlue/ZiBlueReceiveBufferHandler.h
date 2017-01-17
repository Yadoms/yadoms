#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/communication/IReceiveBufferHandler.h>
#include <shared/communication/Buffer.hpp>
#include <shared/communication/StringBuffer.h>

#include "frames/FrameType.h"
#include "frames/AsciiFrame.h"
#include "frames/BinaryFrame.h"

//--------------------------------------------------------------
/// \brief	Receive buffer handler for ZiBlue
///
/// This class manages the ZiBlue receive buffer.
/// A message is considered complete from its content (see protocol specification, either ASCII or binary format)
//--------------------------------------------------------------
class CZiBlueReceiveBufferHandler : public shared::communication::IReceiveBufferHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	                              Constructor
   /// \param[in] receiveDataEventHandler    The event handler to notify for received data event
   /// \param[in] receiveBinaryFrameEventId  The event id to notify for received data event (binary format)
   /// \param[in] receiveAsciiFrameEventId   The event id to notify for received command answer event
   //--------------------------------------------------------------
   CZiBlueReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler, int receiveBinaryFrameEventId, int receiveAsciiFrameEventId);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CZiBlueReceiveBufferHandler();

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

   typedef struct BufferContainer
   {
      frames::EFrameType frameType;
      boost::shared_ptr<frames::CBinaryFrame> binaryBuffer;
      boost::shared_ptr<frames::CAsciiFrame> asciiBuffer;
   }BufferContainer;

   //--------------------------------------------------------------
   /// \brief	                     Pop the next message from the receive buffer
   /// \return                      The next complete message
   //--------------------------------------------------------------
   BufferContainer popNextMessage();

   //--------------------------------------------------------------
   /// \brief	                     Send a message to the target event handler
   /// \param[in] buffer            Buffer to send
   //--------------------------------------------------------------
   void notifyEventHandler(BufferContainer & buffer) const;

   //--------------------------------------------------------------
   /// \brief	         Identifies the first frame in buffer
   /// \return          The frame type
   //--------------------------------------------------------------
   frames::EFrameType identifyFrameType();

   //--------------------------------------------------------------
   /// \brief	         Sync buffer to start of frame
   /// \return          true if SYNC pattern has been found, false in other cases
   //--------------------------------------------------------------
   bool syncToStartOfFrame();
private:
   //--------------------------------------------------------------
   /// \brief	Buffer content
   //--------------------------------------------------------------
   std::vector<unsigned char> m_content;

   //--------------------------------------------------------------
   /// \brief	The event handler to notify for received data event   
   //--------------------------------------------------------------
   shared::event::CEventHandler& m_receiveDataEventHandler;

   //--------------------------------------------------------------
   /// \brief	The event id to notify for received binary data event  
   //--------------------------------------------------------------
   int m_receiveBinaryFrameEventId;  

   //--------------------------------------------------------------
   /// \brief	The event id to notify for received command answer event  
   //--------------------------------------------------------------
   int m_receiveAsciiFrameEventId;
};

