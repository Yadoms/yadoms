#include "stdafx.h"

#include "../../../../sources/shared/shared/communication/IAsyncPort.h"


class CAsyncPortMock : public shared::communication::IAsyncPort
{
public:
   CAsyncPortMock()
   {
   }

   virtual ~CAsyncPortMock()
   {
   }

   void setReceiveBufferMaxSize(std::size_t size) override
   {
   }

   void start() override
   {
   }

   void stop() override
   {
   }

   bool isConnected() const override
   {
      return true;
   }

   void sendText(const std::string & content) override
   {}

   void subscribeForConnectionEvents(shared::event::CEventHandler& forEventHandler,
                                     int forId) override
   {
   }

   void setReceiveBufferHandler(boost::shared_ptr<shared::communication::IReceiveBufferHandler> handler) override
   {
      m_receiveBufferHandler = handler;
   }

   void send(const shared::communication::CByteBuffer& buffer) override
   {
      m_lastSendBuffer = boost::make_shared<shared::communication::CByteBuffer>(buffer);

      if (m_autoReceiveMessage)
         m_receiveBufferHandler->push(*m_autoReceiveMessage);
   }

   void flush() override
   {
      m_lastSendBuffer.reset();
   }

   void setAutoReceiveMessage(boost::shared_ptr<shared::communication::CByteBuffer> buffer)
   {
      m_autoReceiveMessage = buffer;
   }

   boost::shared_ptr<shared::communication::CByteBuffer> m_lastSendBuffer;

   boost::shared_ptr<shared::communication::IReceiveBufferHandler> m_receiveBufferHandler;
   boost::shared_ptr<shared::communication::CByteBuffer> m_autoReceiveMessage;
};

