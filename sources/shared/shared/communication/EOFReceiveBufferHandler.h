#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/communication/IReceiveBufferHandler.h>
#include <shared/communication/Buffer.hpp>
#include <shared/Log.h>

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	Receive buffer handler for messages ending with a special character
      ///
      /// This class implements a IReceiveBufferHandler for messages with a special End-Of-Frame character
      /// A message is then considered complete when the End-Of-Frame character is received
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT CEOFReceiveBufferHandler : public IReceiveBufferHandler
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                           Constructor
         /// \param[in] receiveDataEventHandler The event handler to notify for received data event
         /// \param[in] receiveDataEventId      The event id to notify for received data event
         /// \param[in] eofCharacter            The expected End-Of-Frame character
         //--------------------------------------------------------------
         CEOFReceiveBufferHandler(event::CEventHandler& receiveDataEventHandler,
                                  int receiveDataEventId,
                                  char eofCharacter);

         //--------------------------------------------------------------
         /// \brief	                           Destructor
         //--------------------------------------------------------------
         virtual ~CEOFReceiveBufferHandler();

         // IReceiveBufferHandler implementation
         void push(const CByteBuffer& buffer) override;
         void flush() override;
         // [END] IReceiveBufferHandler implementation

      protected:
         //--------------------------------------------------------------
         /// \brief	                     Pop the next message from the receive buffer
         /// \return                      The next complete message
         //--------------------------------------------------------------
         boost::shared_ptr<const CByteBuffer> popNextMessage();

         //--------------------------------------------------------------
         /// \brief	                     Send a message to the target event handler
         /// \param[in] buffer            Buffer to send
         //--------------------------------------------------------------
         void notifyEventHandler(boost::shared_ptr<const CByteBuffer> buffer) const;

      private:
         //--------------------------------------------------------------
         /// \brief	Buffer content
         //--------------------------------------------------------------
         mutable boost::recursive_mutex m_contentMutex;
         std::vector<unsigned char> m_content;

         //--------------------------------------------------------------
         /// \brief	The event handler to notify for received data event   
         //--------------------------------------------------------------
         event::CEventHandler& m_receiveDataEventHandler;

         //--------------------------------------------------------------
         /// \brief	The event id to notify for received data event  
         //--------------------------------------------------------------
         int m_receiveDataEventId;

         //--------------------------------------------------------------
         /// \brief	The expected end-of-frame character
         //--------------------------------------------------------------
         const char m_eofCharacter;
      };
   }
} // namespace shared::communication
