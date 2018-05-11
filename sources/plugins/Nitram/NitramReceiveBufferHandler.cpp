#include "stdafx.h"
#include "NitramReceiveBufferHandler.h"
#include "NitramMessages/MessagesDefinition.h"
#include "NitramMessages/NitramHelpers.h"
#include <shared/Log.h>

enum
{
   maxMessageSize = 1000
};

CNitramReceiveBufferHandler::CNitramReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                                         int receiveDataEventId,
                                                         boost::shared_ptr<shared::communication::IBufferLogger> logger)
   : m_receiveDataEventHandler(receiveDataEventHandler),
     m_receiveDataEventId(receiveDataEventId),
     m_logger (logger)
{
}

CNitramReceiveBufferHandler::~CNitramReceiveBufferHandler()
{
}

void CNitramReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   for (size_t idx = 0; idx < buffer.size(); ++idx)
	   m_content.push_back(buffer[idx]);

   // Send message if complete (separate aggregated messages)
   while (true)
   {
      auto message = getMessage();
      if (!message)
         break;
      notifyEventHandler(message);
   }
}

void CNitramReceiveBufferHandler::flush()
{
   m_content.clear();
}

boost::shared_ptr<const shared::communication::CByteBuffer> CNitramReceiveBufferHandler::getMessage()
{
   static const boost::shared_ptr<const shared::communication::CByteBuffer> uncompleteMessage;

   if (m_content.empty())
      return uncompleteMessage;

   size_t extractedMessageSize = 0;

   // Vérification du status
   size_t payload = readHeaderPayloadSize(m_content[0]);

   // Retourne le status d'état si ACK ou NACK
   if (payload == 0) 
   {
      auto frame = boost::make_shared<shared::communication::CByteBuffer>(m_content);
      m_content.clear();
      return frame;
   }

   if (m_content.size()==1)
      return uncompleteMessage;

   switch (m_content[1])
   {
   case nitramMessages::ENitramMessageID::BatteryCapacityAndVoltage:
      extractedMessageSize = 5; // 4 Message + 1 CRC
      break;
   case nitramMessages::ENitramMessageID::Runtime:
      extractedMessageSize = 4; // 3 Message + 1 CRC
      break;
   case nitramMessages::ENitramMessageID::SummaryStatus:
      extractedMessageSize = 3; // 2 Message + 1 CRC
      break;
   default: // If the first character is not recognize we flush the reception
      m_content.clear();
      return uncompleteMessage;
      break;
   }

   if (m_content.size() < (1+extractedMessageSize)) // Ajout de la taille du header
      return uncompleteMessage;

   switch (m_content[1])
   {
   case nitramMessages::ENitramMessageID::BatteryCapacityAndVoltage:
   case nitramMessages::ENitramMessageID::Runtime:
   case nitramMessages::ENitramMessageID::SummaryStatus:
   {
      auto frameReceived = boost::make_shared<shared::communication::CByteBuffer>(m_content);
      if (isCheckSumOk(frameReceived))
      {
         //extract only the data 
         // TODO : Check if the CRC is in it or not. Should be removed !
         auto frame = boost::make_shared<shared::communication::CByteBuffer>(std::vector<unsigned char>(m_content.begin(),
                                                                                                        m_content.begin() + extractedMessageSize));

         // remove the message reading
         m_content.erase(m_content.begin(), m_content.begin() + extractedMessageSize);

         return frame;
      }
      else {
         // remove the wrong message
         m_content.erase(m_content.begin(), m_content.begin() + extractedMessageSize);
      }
      break;
   }
   default:
      break;
   }

   return uncompleteMessage;
}

bool CNitramReceiveBufferHandler::isCheckSumOk(boost::shared_ptr<const shared::communication::CByteBuffer> message)
{
   return (calculateCheckSum(*message, message->size()-1) == (*message)[message->size()-1]);
}

void CNitramReceiveBufferHandler::notifyEventHandler(boost::shared_ptr<const shared::communication::CByteBuffer> message) const
{
   m_receiveDataEventHandler.postEvent<const shared::communication::CByteBuffer>(m_receiveDataEventId, *message);
}