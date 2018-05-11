#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/communication/IReceiveBufferHandler.h>
#include <shared/communication/Buffer.hpp>
#include <shared/communication/IBufferLogger.h>

//--------------------------------------------------------------
/// \brief	Receive buffer handler for Nitram
///
/// This class manages the TeleInfo receive buffer.
/// A message is considered complete from its content (see protocol specification)
//--------------------------------------------------------------
class CNitramReceiveBufferHandler : public shared::communication::IReceiveBufferHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   /// \param[in] receiveDataEventHandler The event handler to notify for received data event
   /// \param[in] receiveDataEventId      The event id to notify for received data event
   /// \param[in] logger                  logger in developer mode
   //--------------------------------------------------------------
   CNitramReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                               int receiveDataEventId,
                               boost::shared_ptr<shared::communication::IBufferLogger> logger);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CNitramReceiveBufferHandler();

   // INitramReceiveBufferHandler implementation
   void push(const shared::communication::CByteBuffer& buffer) override;
   void flush() override;
   // [END] INitramReceiveBufferHandler implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Check if we got a complete message
   /// \return                      The complet message
   //--------------------------------------------------------------
   boost::shared_ptr<const shared::communication::CByteBuffer> getMessage();

   //--------------------------------------------------------------
   /// \brief	                     Check if the CRC is ok
   /// \param[in] message           a message to be check
   /// \return                      true if the checksum is ok
   //--------------------------------------------------------------
   static bool isCheckSumOk(boost::shared_ptr<const shared::communication::CByteBuffer> message);

   //--------------------------------------------------------------
   /// \brief	                     Send a message to the target event handler
   /// \param[in] messages          Received messages as map(key, value)
   //--------------------------------------------------------------
   void notifyEventHandler(boost::shared_ptr<const shared::communication::CByteBuffer> buffer) const;

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

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IBufferLogger> m_logger;
};