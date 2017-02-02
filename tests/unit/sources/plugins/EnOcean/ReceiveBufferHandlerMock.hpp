#include "stdafx.h"

#include "../../../../sources/shared/shared/communication/IReceiveBufferHandler.h"
#include "../../../../sources/plugins/EnOcean/message/Esp3ReceivedPacket.h"
#include "../../../../sources/plugins/EnOcean/IMessageHandler.h"


class ReceiveBufferHandlerMock : public shared::communication::IReceiveBufferHandler
{
public:
   explicit ReceiveBufferHandlerMock(boost::shared_ptr<IMessageHandler> messageHandler)
      : m_messageHandler(messageHandler)
   {
   }

   virtual ~ReceiveBufferHandlerMock()
   {
   }

   void push(const shared::communication::CByteBuffer& buffer) override
   {
      std::vector<unsigned char> message;
      for (const auto& byte : buffer)
         message.push_back(byte);
      m_messageHandler->onReceived(boost::make_shared<message::CEsp3ReceivedPacket>(message));
   }

   void flush() override
   {
   }

private:
   boost::shared_ptr<IMessageHandler> m_messageHandler;
};

