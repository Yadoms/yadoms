#include "stdafx.h"
#include "TeleInfoReceiveBufferHandler.h"

// TODO_V2 create a receiver Handler that begin a message to a STX, and send the buffer at a EOF.

enum
{
   kSTX = 0x02,
   kETX = 0x03,

   kStartMessage = 0x0a,
   kEndMessage = 0x0d
};

enum
{
   maxMessageSize = 1000
};

CTeleInfoReceiveBufferHandler::CTeleInfoReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                                             int receiveDataEventId,
                                                             const boost::posix_time::time_duration suspendDelay)
   : m_receiveDataEventHandler(receiveDataEventHandler),
     m_receiveDataEventId(receiveDataEventId),
     m_nextSendMessageDate(shared::currentTime::Provider().now())
{
}

CTeleInfoReceiveBufferHandler::~CTeleInfoReceiveBufferHandler()
{
}

void CTeleInfoReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   if (shared::currentTime::Provider().now() < m_nextSendMessageDate)
      return;

   for (size_t idx = 0; idx < buffer.size(); ++ idx)
      m_content.push_back(buffer[idx]);

   // Send message if complete (separate aggregated messages)
   while (true)
   {
      auto completeMessage = getCompleteMessage();
      if (!completeMessage)
         break;
      notifyEventHandler(completeMessage);

      m_nextSendMessageDate = shared::currentTime::Provider().now() + m_suspendDelay;
   }
}

void CTeleInfoReceiveBufferHandler::flush()
{
   m_content.clear();
}

boost::shared_ptr<const std::vector<unsigned char>> CTeleInfoReceiveBufferHandler::getCompleteMessage()
{
   static const boost::shared_ptr<const std::vector<unsigned char>> uncompleteMessage;

   if (m_content.empty())
      return uncompleteMessage;

   // Remove first bytes if not sync byte
   while (!m_content.empty() && m_content[0] != kSTX)
      m_content.erase(m_content.begin());

   if (m_content.empty())
      return uncompleteMessage;

   auto etxIterator = std::find(m_content.rbegin(), m_content.rend(), kETX);
   if (etxIterator == m_content.rend())
   {
      if (m_content.size() > maxMessageSize)
         m_content.clear();

      return uncompleteMessage;
   }

   size_t etxPosition = std::distance(std::begin(m_content), etxIterator.base()) - 1;

   // The message is complete
   auto frame = boost::make_shared<std::vector<unsigned char>>(m_content.begin(), m_content.begin() + etxPosition + 1);
   m_content.erase(m_content.begin(), m_content.begin() + etxPosition);

   if (!checkIntegrity(frame))
      return uncompleteMessage;

   return frame;
}

bool CTeleInfoReceiveBufferHandler::checkIntegrity(boost::shared_ptr<const std::vector<unsigned char>> frame)
{
   auto endPos = frame->begin();
   while (true)
   {
      auto startPos = std::find(endPos, frame->end(), kStartMessage);
      if (startPos == frame->end())
         return false;

      endPos = std::find(startPos, frame->end(), kEndMessage) + 1;

      const auto message = std::vector<unsigned char>(startPos, endPos);
      if (!isCheckSumOk(message))
         return false;

      if (endPos + 1 == frame->end())
         return true;
   }
}

bool CTeleInfoReceiveBufferHandler::isCheckSumOk(const std::vector<unsigned char>& message)
{
   if (message.size() < 4)
      return false;

   unsigned int checksum = 0x00;
   for (auto byte = message.begin() + 1; byte != (message.end() - 3); ++byte)
      checksum += *byte;

   checksum = (checksum & 0x3F) + 0x20;

   return checksum == message[message.size() - 2];
}

void CTeleInfoReceiveBufferHandler::notifyEventHandler(boost::shared_ptr<const std::vector<unsigned char>> buffer) const
{
   m_receiveDataEventHandler.postEvent<const std::vector<unsigned char>>(m_receiveDataEventId,
                                                                         *buffer);
}
