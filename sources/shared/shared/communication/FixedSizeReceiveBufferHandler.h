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
      /// \brief	Receive buffer handler for fixed size messages
      ///
      /// This class implements a IReceiveBufferHandler for fixeds size messages.
      /// A message is then considered complete when its size is the construct-time given size
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT CFixedSizeReceiveBufferHandler : public IReceiveBufferHandler
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                           Constructor
         /// \param[in] receiveDataEventHandler The event handler to notify for received data event
         /// \param[in] receiveDataEventId      The event id to notify for received data event
         /// \param[in] messageSize             The number of bytes expected for a message
         //--------------------------------------------------------------
         CFixedSizeReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                        int receiveDataEventId,
                                        size_t messageSize);

         //--------------------------------------------------------------
         /// \brief	                           Destructor
         //--------------------------------------------------------------
         virtual ~CFixedSizeReceiveBufferHandler();

         // IReceiveBufferHandler implementation
         void push(const CByteBuffer& buffer) override;
         void flush() override;
         // [END] IReceiveBufferHandler implementation

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
   //TODO :need a mutex here
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
         /// \brief	The expected message size
         //--------------------------------------------------------------
         const size_t m_messageSize;
      };
   }
} // namespace shared::communication
