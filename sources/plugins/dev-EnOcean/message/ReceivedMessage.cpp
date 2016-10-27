#include "../stdafx.h"
#include "ReceivedMessage.h"
#include "types.h"

namespace message
{
   CHeader::CHeader(const std::vector<unsigned char>& receivedBuffer)
      : m_dataLength(toWord(receivedBuffer, kOffsetDataLength)),
        m_optionalLength(receivedBuffer[kOffsetOptionalLength]),
        m_packetType(static_cast<EPacketType>(receivedBuffer[kOffsetPacketType])),
        m_offsetData(kOffsetData),
        m_offsetOptional(m_offsetData + m_dataLength)
   {
   }

   unsigned short CHeader::dataLength() const
   {
      return m_dataLength;
   }

   unsigned char CHeader::optionalLength() const
   {
      return m_optionalLength;
   }

   EPacketType CHeader::packetType() const
   {
      return m_packetType;
   }

   int CHeader::offsetData() const
   {
      return m_offsetData;
   }

   int CHeader::offsetOptional() const
   {
      return m_offsetOptional;
   }


   CReceivedEsp3Packet::CReceivedEsp3Packet(const std::vector<unsigned char>& receivedBuffer)
      : m_header(receivedBuffer),
        m_data(receivedBuffer.begin() + m_header.offsetData(), receivedBuffer.begin() + m_header.offsetData() + m_header.dataLength()),
        m_optional(receivedBuffer.begin() + m_header.offsetOptional(), receivedBuffer.begin() + m_header.offsetOptional() + m_header.optionalLength())
   {
   }

   CReceivedEsp3Packet::~CReceivedEsp3Packet()
   {
   }

   const CHeader& CReceivedEsp3Packet::header() const
   {
      return m_header;
   }

   const std::vector<unsigned char>& CReceivedEsp3Packet::data() const
   {
      return m_data;
   }

   const std::vector<unsigned char>& CReceivedEsp3Packet::optional() const
   {
      return m_optional;
   }


   CRadioErp1Message::CRadioErp1Message(const CReceivedEsp3Packet& esp3Packet)
      : m_rorg(CRorgs::toRorgId(esp3Packet.data()[0])),
        m_senderId(esp3Packet.data()[esp3Packet.data().size() - 5] << 24 | esp3Packet.data()[esp3Packet.data().size() - 4] << 16 | esp3Packet.data()[esp3Packet.data().size() - 3] << 8 | esp3Packet.data()[esp3Packet.data().size() - 2]),
        m_senderIdAsString(senderIdToString(m_senderId)),
        m_status(esp3Packet.data()[esp3Packet.data().size() - 1]),
        m_data(esp3Packet.data().begin() + 1, esp3Packet.data().begin() + esp3Packet.data().size() - 6 + 1)
   {
   }

   CRadioErp1Message::~CRadioErp1Message()
   {
   }

   CRorgs::ERorgIds CRadioErp1Message::rorg() const
   {
      return m_rorg;
   }

   unsigned int CRadioErp1Message::senderId() const
   {
      return m_senderId;
   }

   std::string CRadioErp1Message::senderIdAsString() const
   {
      return m_senderIdAsString;
   }

   const std::vector<unsigned char>& CRadioErp1Message::data() const
   {
      return m_data;
   }

   std::string CRadioErp1Message::dump() const
   {
      std::stringstream ss;
      ss << std::setfill('0') << std::setw(2) << std::hex;
      for (auto byte = m_data.begin(); byte != m_data.end(); ++byte)
         ss << *byte << " ";
      return ss.str();
   }

   std::string CRadioErp1Message::senderIdToString(unsigned senderId)
   {
      std::stringstream ss;
      ss << std::setfill('0') << std::setw(8) << std::uppercase << std::hex << senderId;
      return ss.str();
   }
} // namespace message


