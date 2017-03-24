#include "stdafx.h"
#include "SmallHeaderMessageAssembler.h"
#include "SmallHeaderMessageAssemblerPart.h"
#include <shared/Log.h>

namespace shared
{
   namespace communication
   {
      SmallHeaderMessageAssembler::SmallHeaderMessageAssembler(size_t maxPartSize)
         : m_maxPartSize(maxPartSize),
           m_fullMessageSize(0),
           m_fullMessageIndex(0),
           m_completed(false)
      {
      }

      SmallHeaderMessageAssembler::~SmallHeaderMessageAssembler()
      {
      }

      void SmallHeaderMessageAssembler::appendPart(boost::shared_ptr<const unsigned char[]> message,
                                                   size_t messageSize)
      {
         if (messageSize <= SmallHeaderMessageAssemblerPart::headerSize())
         {
            YADOMS_LOG(error) << "Receive empty or bad formatted message ==> ignored";
            return;
         }

         SmallHeaderMessageAssemblerPart part(message,
                                              messageSize);

         if (part.isFirst())
         {
            allocFullMessage(part.partCount());
            m_fullMessageIndex = 0;
            m_completed = false;
         }

         if ((m_fullMessageIndex + part.usefulSize()) > m_fullMessageSize)
         {
            YADOMS_LOG(error) << "Receive bad formatted message (too big) ==> ignored";
            return;
         }

         memcpy(&m_fullMessage[m_fullMessageIndex],
                part.usefulMessage(),
                part.usefulSize());
         m_fullMessageIndex += part.usefulSize();

         if (part.isLast())
            m_completed = true;
      }

      bool SmallHeaderMessageAssembler::isCompleted() const
      {
         return m_completed;
      }

      boost::shared_ptr<const unsigned char[]> SmallHeaderMessageAssembler::message() const
      {
         return m_fullMessage;
      }

      size_t SmallHeaderMessageAssembler::messageSize() const
      {
         return m_fullMessageIndex;
      }

      void SmallHeaderMessageAssembler::allocFullMessage(size_t partCount)
      {
         auto newSize = partCount * (m_maxPartSize - SmallHeaderMessageAssemblerPart::headerSize());
         if (!m_fullMessage || newSize > m_fullMessageSize)
         {
            m_fullMessageSize = newSize;
            m_fullMessage = boost::make_shared<unsigned char[]>(newSize);
         }
      }
   }
} // namespace shared::communication


