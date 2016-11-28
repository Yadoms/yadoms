#include "stdafx.h"
#include "Esp3ReceivedPacket.h"

namespace message
{
   CEsp3ReceivedPacket::CEsp3ReceivedPacket(const std::vector<unsigned char>& receivedBuffer)
      : m_receivedBuffer(receivedBuffer),
        m_header(receivedBuffer),
        m_data(receivedBuffer.begin() + m_header.offsetData(), receivedBuffer.begin() + m_header.offsetData() + m_header.dataLength()),
        m_optional(receivedBuffer.begin() + m_header.offsetOptional(), receivedBuffer.begin() + m_header.offsetOptional() + m_header.optionalLength())
   {
   }

   CEsp3ReceivedPacket::~CEsp3ReceivedPacket()
   {
   }

   const CHeader& CEsp3ReceivedPacket::header() const
   {
      return m_header;
   }

   const std::vector<unsigned char>& CEsp3ReceivedPacket::data() const
   {
      return m_data;
   }

   const std::vector<unsigned char>& CEsp3ReceivedPacket::optional() const
   {
      return m_optional;
   }

   const std::vector<unsigned char>& CEsp3ReceivedPacket::buffer() const
   {
      return m_receivedBuffer;
   }
} // namespace message
