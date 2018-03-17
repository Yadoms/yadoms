#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/communication/IReceiveBufferHandler.h>
#include <shared/communication/Buffer.hpp>
#include <shared/communication/IBufferLogger.h>

//--------------------------------------------------------------
/// \brief	Receive buffer handler for SomfyIOController
///
/// This class manages the SomfyIOController receive buffer.
/// A message is considered complete from its content (see protocol specification)
//--------------------------------------------------------------
class CSomfyIOControllerReceiveBufferHandler : public shared::communication::IReceiveBufferHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   /// \param[in] receiveDataEventHandler The event handler to notify for received data event
   /// \param[in] receiveDataEventId      The event id to notify for received data event
   /// \param[in] logger                  logger in developer mode
   /// \param[in] isDeveloperMode         developer mode is active
   //--------------------------------------------------------------
   CSomfyIOControllerReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                              int receiveDataEventId,
                              boost::shared_ptr<shared::communication::IBufferLogger> logger,
                              const bool isDeveloperMode);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CSomfyIOControllerReceiveBufferHandler();

   // ISomfyIOControllerReceiveBufferHandler implementation
   void push(const shared::communication::CByteBuffer& buffer) override;
   void flush() override;
   // [END] ISomfyIOControllerReceiveBufferHandler implementation

   void activate();
   void desactivate();

protected:
   //--------------------------------------------------------------
   /// \brief	                     Check if we got a complete message
   /// \return                      The complet message
   //--------------------------------------------------------------
	boost::shared_ptr<const shared::communication::CByteBuffer> getCompleteMessage();


   //--------------------------------------------------------------
   /// \brief	                     Send a message to the target event handler
   /// \param[in] messages          Received messages as map(key, value)
   //--------------------------------------------------------------
	void notifyEventHandler(const boost::shared_ptr<const shared::communication::CByteBuffer> buffer) const;

	std::string msgToString(const shared::communication::CByteBuffer& data);

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

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IBufferLogger> m_logger;

   bool m_isDeveloperMode;
};

