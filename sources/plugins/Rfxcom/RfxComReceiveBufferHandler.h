#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/communication/IReceiveBufferHandler.h>
#include <shared/communication/Buffer.hpp>

//--------------------------------------------------------------
/// \brief	Receive buffer handler for RFXCom
///
/// This class manages the Rfxcom receive buffer.
/// A message is considered complete from its content (see protocol specification)
//--------------------------------------------------------------
class CRfxcomReceiveBufferHandler : public shared::communication::IReceiveBufferHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   /// \param[in] receiveDataEventHandler The event handler to notify for received data event
   /// \param[in] receiveDataEventId      The event id to notify for received data event
   //--------------------------------------------------------------
   CRfxcomReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                               int receiveDataEventId);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CRfxcomReceiveBufferHandler();

   // IReceiveBufferHandler implementation
   void push(const shared::communication::CByteBuffer& buffer) override;
   void flush() override;
   // [END] IReceiveBufferHandler implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Returns the message if complete
   /// \return                      The complete message, or null
   //--------------------------------------------------------------
   boost::shared_ptr<const shared::communication::CByteBuffer> getCompleteMessage();

   //--------------------------------------------------------------
   /// \brief	                     Send a message to the target event handler
   /// \param[in] buffer            Buffer to send
   //--------------------------------------------------------------
   void notifyEventHandler(boost::shared_ptr<const shared::communication::CByteBuffer> buffer) const;

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
   /// \brief	The event handler to notify for received data event   
   //--------------------------------------------------------------
   shared::event::CEventHandler& m_receiveDataEventHandler;

   //--------------------------------------------------------------
   /// \brief	The event id to notify for received data event  
   //--------------------------------------------------------------
   int m_receiveDataEventId;
};

