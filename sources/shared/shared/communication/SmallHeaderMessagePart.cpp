#include "stdafx.h"
#include "SmallHeaderMessagePart.h"

namespace shared
{
   namespace communication
   {
      SmallHeaderMessagePart::SmallHeaderMessagePart(unsigned char partNumber,
                                                     unsigned char partCount,
                                                     const unsigned char* usefulData,
                                                     size_t usefulSize)
         : m_header(partNumber,
                    partCount),
           m_usefulData(usefulData),
           m_usefulSize(usefulSize)
      {
         formatMessage();
      }

      SmallHeaderMessagePart::~SmallHeaderMessagePart()
      {
      }

      size_t SmallHeaderMessagePart::headerSize()
      {
         return Header::size();
      }

      const unsigned char* SmallHeaderMessagePart::formattedMessage() const
      {
         return m_formattedMessage.get();
      }

      size_t SmallHeaderMessagePart::formattedSize() const
      {
         return m_formattedSize;
      }

      void SmallHeaderMessagePart::formatMessage()
      {
         m_formattedSize = sizeof(Header) + m_usefulSize;
         m_formattedMessage = boost::make_shared<unsigned char[]>(m_formattedSize);

         auto index = 0;
         m_formattedMessage[index] = m_header.m_partNumber;
         index += sizeof(m_header.m_partNumber);
         m_formattedMessage[index] = m_header.m_partCount;
         index += sizeof(m_header.m_partCount);
         memcpy(&m_formattedMessage[index], m_usefulData, m_usefulSize);
      }
   }
} // namespace shared::communication


