#include "stdafx.h"
#include "SomfySituoReceiveBufferHandler.h"
#include <shared/Log.h>

enum
{
   kSTX = 0x02,
   kETX = 0x03
};

enum
{
   maxMessageSize = 3000
};

CSomfySituoReceiveBufferHandler::CSomfySituoReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                                                 int receiveDataEventId,
                                                                 boost::shared_ptr<shared::communication::IBufferLogger>
                                                                 logger,
                                                                 const bool isDeveloperMode)
   : m_receiveDataEventHandler(receiveDataEventHandler),
     m_receiveDataEventId(receiveDataEventId),
     m_logger(logger),
     m_isDeveloperMode(isDeveloperMode)
{
}

CSomfySituoReceiveBufferHandler::~CSomfySituoReceiveBufferHandler()
{
}

void CSomfySituoReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   YADOMS_LOG(information) << "COM : push buffer size " << buffer.size();
   static const auto InterByteTimeout = boost::posix_time::milliseconds(100);

   // Manage timeout
   const auto now = shared::currentTime::Provider().now();
   if (m_content.size() != 0)
   {
      // Reset data if too old
      if (now - m_lastReceivedTime > InterByteTimeout)
         m_content.clear();
   }
   m_lastReceivedTime = now;

   std::string str;
   for (size_t idx = 0; idx < buffer.size(); ++idx)
   {
      m_content.push_back(buffer[idx]);
      str.push_back(buffer[idx]);
   }
   if (m_isDeveloperMode) m_logger->logReceived(buffer);
   YADOMS_LOG(information) << "COM : push buffer " << str;

   // Send message if complete (separate aggregated messages)
   while (true)
   {
      const auto completeMessage = getCompleteMessage();
      if (!completeMessage)
         break;
      notifyEventHandler(completeMessage);

      m_logger->logReceived(shared::communication::CByteBuffer(m_content));
   }
}

void CSomfySituoReceiveBufferHandler::flush()
{
   m_content.clear();
}

boost::shared_ptr<const shared::communication::CByteBuffer> CSomfySituoReceiveBufferHandler::getCompleteMessage()
{
   static const boost::shared_ptr<const shared::communication::CByteBuffer> UncompleteMessage;

   if (m_content.empty())
      return UncompleteMessage;

   // Remove first bytes if not sync byte
   while (!m_content.empty() && m_content[0] != kSTX)
      m_content.erase(m_content.begin());

   if (m_content.empty())
      return UncompleteMessage;

   auto etxIterator = std::find(m_content.rbegin(), m_content.rend(), kETX);
   if (etxIterator == m_content.rend())
   {
      if (m_content.size() > maxMessageSize)
         m_content.clear();

      return UncompleteMessage;
   }

   // remove STX
   m_content.erase(m_content.begin());

   etxIterator = std::find(m_content.rbegin(), m_content.rend(), kETX);
   // We have to do it again because we suppress a character
   size_t etxPosition = std::distance(std::begin(m_content), etxIterator.base()) - 1; //we suppress STX and ETX

   // The message is complete
   auto message = boost::make_shared<const shared::communication::CByteBuffer>(std::vector<unsigned char>(
      m_content.begin(),
      m_content.begin() + etxPosition));

   // Delete extracted data
   m_content.erase(m_content.begin(), m_content.begin() + etxPosition);

   return message;
}


void CSomfySituoReceiveBufferHandler::notifyEventHandler(
   const boost::shared_ptr<const shared::communication::CByteBuffer> buffer) const
{
   m_receiveDataEventHandler.postEvent<const shared::communication::CByteBuffer>(m_receiveDataEventId, *buffer);
}
