#include "stdafx.h"
#include "SmallHeaderMessageAssemblerPart.h"

namespace shared
{
   namespace communication
   {
      SmallHeaderMessageAssemblerPart::SmallHeaderMessageAssemblerPart(boost::shared_ptr<const unsigned char[]> message,
                                                                       size_t messageSize)
         : m_usefulData(nullptr),
           m_usefulSize(0)
      {
         auto index = 0;
         m_header.m_partNumber = message[index];
         index += sizeof(m_header.m_partNumber);
         m_header.m_partCount = message[index];
         index += sizeof(m_header.m_partCount);

         m_usefulData = &message[index];

         m_usefulSize = messageSize - SmallHeader::size();
      }

      SmallHeaderMessageAssemblerPart::~SmallHeaderMessageAssemblerPart()
      {
      }

      size_t SmallHeaderMessageAssemblerPart::headerSize()
      {
         return SmallHeader::size();
      }

      bool SmallHeaderMessageAssemblerPart::isFirst() const
      {
         return m_header.m_partNumber == 0;
      }

      bool SmallHeaderMessageAssemblerPart::isLast() const
      {
         return m_header.m_partNumber == m_header.m_partCount - 1;
      }

      size_t SmallHeaderMessageAssemblerPart::partCount() const
      {
         return m_header.m_partCount;
      }

      const unsigned char* SmallHeaderMessageAssemblerPart::usefulMessage() const
      {
         return m_usefulData;
      }

      size_t SmallHeaderMessageAssemblerPart::usefulSize() const
      {
         return m_usefulSize;
      }
   }
} // namespace shared::communication


