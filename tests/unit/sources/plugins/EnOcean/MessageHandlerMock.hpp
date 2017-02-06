#include "stdafx.h"

// Includes needed to compile tested classes
#include "../../../../sources/plugins/EnOcean/ReceiveBufferHandler.h"
#include "../../../../sources/plugins/EnOcean/profiles/generated-eep.h"


inline CRorgs::ERorgIds CRorgs::toRorgId(unsigned int)
{
   return k4BS_Telegram;
}

class CMessageHandlerMock : public IMessageHandler
{
public:
   explicit CMessageHandlerMock(shared::event::CEventHandler& evtHandler, int evtId)
      : m_evtHandler(evtHandler),
        m_evtId(evtId)
   {
   }

   virtual ~CMessageHandlerMock()
   {
   }

   void send(message::CEsp3SendPacket&) override
   {
   }

   bool send(message::CEsp3SendPacket&,
             boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)>,
             boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)>) override
   {
      return false;
   }

   void onReceived(boost::shared_ptr<const message::CEsp3ReceivedPacket> receivedMessage) override
   {
      m_evtHandler.postEvent(m_evtId, receivedMessage);
   }

private:
   shared::event::CEventHandler& m_evtHandler;
   int m_evtId;
};

