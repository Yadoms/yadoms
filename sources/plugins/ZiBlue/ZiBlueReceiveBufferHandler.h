#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/communication/IReceiveBufferHandler.h>
#include <shared/communication/Buffer.hpp>

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
   /// \param[in] receiveBinaryDataEventId   The event id to notify for received data event (binary format)
   /// \param[in] receiveDataEventId         The event id to notify for received command answer event
   //--------------------------------------------------------------
   CZiBlueReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler, int receiveBinaryDataEventId, int receiveCommandAnswerEventId);

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
   /// \brief	                     Check if we got a complete message
   /// \return                      true if a message is complete
   //--------------------------------------------------------------
   bool isComplete() const;

   typedef enum EMessageType
   {
      kBinaryData,
      kCommandAnswer
   }EMessageType;

   typedef struct BufferContainer
   {
      EMessageType bufferType;
      boost::shared_ptr<const shared::communication::CByteBuffer> buffer;
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
   int m_receiveBinaryDataEventId;  

   //--------------------------------------------------------------
   /// \brief	The event id to notify for received command answer event  
   //--------------------------------------------------------------
   int m_receiveCommandAnswerEventId;
};

