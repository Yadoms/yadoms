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
   /// \param[in] suspendDelay            Mute delay, used to filter messages
   //--------------------------------------------------------------
   CTeleInfoReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                 int receiveDataEventId,
                                 const boost::posix_time::time_duration suspendDelay);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CTeleInfoReceiveBufferHandler();

   // ITeleInfoReceiveBufferHandler implementation
   void push(const shared::communication::CByteBuffer& buffer) override;
   void flush() override;
   // [END] ITeleInfoReceiveBufferHandler implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Check if we got a complete message
   /// \return                      The complet message
   //--------------------------------------------------------------
   boost::shared_ptr<std::map<std::string, std::string>> getCompleteMessage();

   static  boost::shared_ptr<std::map<std::string, std::string>> getMessages(boost::shared_ptr<const std::vector<unsigned char>> frame);

   static bool isCheckSumOk(const std::string& message);

   //--------------------------------------------------------------
   /// \brief	                     Send a message to the target event handler
   /// \param[in] messages          Received messages as map(key, value)
   //--------------------------------------------------------------
   void notifyEventHandler(boost::shared_ptr<std::map<std::string, std::string>> messages) const;

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
   /// \brief	Management of suspend delay between 2 messages
   //--------------------------------------------------------------
   boost::posix_time::ptime m_nextSendMessageDate;
   const boost::posix_time::time_duration m_suspendDelay;
};
