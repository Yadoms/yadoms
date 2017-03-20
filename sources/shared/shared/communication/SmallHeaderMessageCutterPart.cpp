#include "stdafx.h"
#include "SmallHeaderMessageCutterPart.h"

namespace shared
{
   namespace communication
   {
      SmallHeaderMessageCutterPart::SmallHeaderMessageCutterPart(unsigned char partNumber,
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

      SmallHeaderMessageCutterPart::~SmallHeaderMessageCutterPart()
      {
      }

      size_t SmallHeaderMessageCutterPart::headerSize()
      {
         return SmallHeader::size();
      }

      const unsigned char* SmallHeaderMessageCutterPart::formattedMessage() const
      {
         return m_formattedMessage.get();
      }

      size_t SmallHeaderMessageCutterPart::formattedSize() const
      {
         return m_formattedSize;
      }

      void SmallHeaderMessageCutterPart::formatMessage()
      {
         m_formattedSize = headerSize() + m_usefulSize;
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


