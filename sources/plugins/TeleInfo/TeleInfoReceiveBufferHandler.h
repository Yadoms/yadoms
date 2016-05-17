#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/communication/IReceiveBufferHandler.h>
#include <shared/communication/Buffer.hpp>

//--------------------------------------------------------------
/// \brief	Receive buffer handler for TeleInfo
///
/// This class manages the TeleInfo receive buffer.
/// A message is considered complete from its content (see protocol specification)
//--------------------------------------------------------------
class CTeleInfoReceiveBufferHandler : public shared::communication::IReceiveBufferHandler
{  
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   /// \param[in] receiveDataEventHandler The event handler to notify for received data event
   /// \param[in] receiveDataEventId      The event id to notify for received data event
   //--------------------------------------------------------------
   CTeleInfoReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler, int receiveDataEventId);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CTeleInfoReceiveBufferHandler();

   // ITeleInfoReceiveBufferHandler implementation
   virtual void push(const shared::communication::CByteBuffer& buffer);
   virtual void flush();
   // [END] ITeleInfoReceiveBufferHandler implementation

   void suspend ( void );
   void resume  ( void );

protected:
   //--------------------------------------------------------------
   /// \brief	                     Check if we got a complete message
   /// \return                      true if a message is complete
   //--------------------------------------------------------------
   bool isComplete() const;

   //--------------------------------------------------------------
   /// \brief	                     Pop the next message from the receive buffer
   /// \return                      The next complete message
   //--------------------------------------------------------------
   boost::shared_ptr<const shared::communication::CByteBuffer> popNextMessage();

   //--------------------------------------------------------------
   /// \brief	                     Send a message to the target event handler
   /// \param[in] buffer            Buffer to send
   //--------------------------------------------------------------
   void notifyEventHandler(boost::shared_ptr<const shared::communication::CByteBuffer> buffer);

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
   /// \brief	The event id to notify for received data event  
   //--------------------------------------------------------------
   int m_receiveDataEventId;

   bool m_receptionSuspended;
};



