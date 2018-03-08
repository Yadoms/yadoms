#include "stdafx.h"
#include "PicBootReceiveBufferHandler.h"
#include "PicBootPacketControlCharacters.h"
#include <shared/Log.h>


CPicBootReceiveBufferHandler::CPicBootReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                                           const int receiveDataEventId,
                                                           const boost::posix_time::time_duration readTimeOut,
                                                           shared::communication::IBufferLogger& logger)
   : m_receiveDataEventHandler(receiveDataEventHandler),
     m_receiveDataEventId(receiveDataEventId),
     m_logger(logger),
     m_readTimeOut(readTimeOut),
     m_lastReceivedTime(shared::currentTime::Provider().now()),
     m_nextCharacterIsEscaped(false)
{
}

CPicBootReceiveBufferHandler::~CPicBootReceiveBufferHandler()
{
}

void CPicBootReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   // Manage timeout
   const auto now = shared::currentTime::Provider().now();
   if (m_content.size() != 0)
   {
      // Reset data if too old
      if (now - m_lastReceivedTime > m_readTimeOut)
         m_content.clear();
   }
   m_lastReceivedTime = now;

   boost::lock_guard<boost::recursive_mutex> lock(m_contentMutex);

   for (unsigned int idx = 0; idx < buffer.size(); ++idx)
      m_content.push_back(buffer[idx]);

   // Send message if complete (separate aggregated messages)
   while (true)
   {
      const auto completeMessage = getCompleteMessage();
      if (!completeMessage)
         break;
      notifyEventHandler(completeMessage);
   }
}

void CPicBootReceiveBufferHandler::flush()
{
   boost::lock_guard<boost::recursive_mutex> lock(m_contentMutex);
   m_content.clear();
}

boost::shared_ptr<const std::vector<unsigned char>> CPicBootReceiveBufferHandler::getCompleteMessage()
{
   static const boost::shared_ptr<const std::vector<unsigned char>> UncompleteMessage;

   if (m_content.size() < 2)
      return UncompleteMessage;

   // Remove first bytes if not start bytes (message must begin by 2 STX)
   while (!m_content.empty() && (m_content[0] != kSTX || m_content[1] != kSTX))
      m_content.erase(m_content.begin());

   if (m_content.size() < 2)
      return UncompleteMessage;

   // Process the rest of the message
   for (auto contentIterator = m_content.begin() + 2; contentIterator != m_content.end(); ++contentIterator)
   {
      if (m_nextCharacterIsEscaped)
      {
         // This character was escaped, so must always be accepted
         m_nextCharacterIsEscaped = false;
      }
      else
      {
         switch (*contentIterator)
         {
         case kETX:
            {
               // The message is complete
               const auto& fullMessage = std::vector<unsigned char>(m_content.begin(),
                                                                    contentIterator + 1);
               m_logger.logReceived(shared::communication::CByteBuffer(fullMessage));
               const auto message = extractUsefulMessagePart(fullMessage);

               // Remove consumpted data
               m_content.erase(m_content.begin(), contentIterator + 1);

               if (!message)
               {
                  // Bad message (size, checksum...)
                  // Retry with rest of data
                  return getCompleteMessage();
               }

               return message;
            }
         case kDLE:
            {
               // DLE is used to escape a character, who must not be interpreted
               // as control characters. So next byte must always be accepted
               m_nextCharacterIsEscaped = true;
               break;
            }
         case kSTX:
            {
               // Got STX before ETX, data before the STX is invalid
               // Delete invalid data and retry with rest of data
               m_content.erase(m_content.begin(), contentIterator);
               return getCompleteMessage();
            }
         default:
            // Data byte, continue
            break;
         }
      }
   }

   // STX not found
   return UncompleteMessage;
}

boost::shared_ptr<const std::vector<unsigned char>> CPicBootReceiveBufferHandler::extractUsefulMessagePart(const std::vector<unsigned char>& fullMessage) const
{
   static const boost::shared_ptr<const std::vector<unsigned char>> UncompleteMessage;

   if (fullMessage.size() < 4)
   {
      YADOMS_LOG(error) << "CPicBootReceiveBufferHandler::extractUsefulMessagePart : invalid message size " << fullMessage.size();
      return UncompleteMessage;
   }

   auto message = boost::make_shared<std::vector<unsigned char>>(fullMessage);

   // Remove the 2 STX
   message->erase(message->begin(), message->begin() + 2);

   // Remove the ETX
   message->pop_back();

   // Remove the DLE
   auto it = message->begin();
   while (it != message->end())
   {
      // Remove DLE
      if (*it == kDLE)
         it = message->erase(it);

      if (it != message->end())
         ++it;
   }

   const auto readChecksum = message->back();

   // Remove the checksum
   message->pop_back();

   // Compute checksum
   unsigned long computedChecksum = 0;
   for (const auto byte:*message)
      computedChecksum += byte;
   computedChecksum = (~computedChecksum + 1) & 0xFF;

   if (readChecksum != computedChecksum)
   {
      YADOMS_LOG(error) << "CPicBootReceiveBufferHandler::extractUsefulMessagePart : bad checksum "
         << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(readChecksum)
         << ", expected "
         << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(computedChecksum);
      return UncompleteMessage;
   }

   return message;
}

void CPicBootReceiveBufferHandler::notifyEventHandler(const boost::shared_ptr<const std::vector<unsigned char>> buffer) const
{
   m_receiveDataEventHandler.postEvent<const std::vector<unsigned char>>(m_receiveDataEventId, *buffer);
}

