#include "stdafx.h"
#include "ReceivedMessage.h"
//TODO renommer le fichier

namespace message
{
   CRadioErp1ReceivedMessage::CRadioErp1ReceivedMessage(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
      : m_rorg(CRorgs::toRorgId(esp3Packet->data()[0])),
        m_senderId(esp3Packet->data()[esp3Packet->data().size() - 5] << 24 | esp3Packet->data()[esp3Packet->data().size() - 4] << 16 | esp3Packet->data()[esp3Packet->data().size() - 3] << 8 | esp3Packet->data()[esp3Packet->data().size() - 2]),
        m_senderIdAsString(senderIdToString(m_senderId)),
        m_status(esp3Packet->data()[esp3Packet->data().size() - 1]),
        m_data(esp3Packet->data().begin() + 1, esp3Packet->data().begin() + esp3Packet->data().size() - 6 + 1)
   {
   }

   CRadioErp1ReceivedMessage::~CRadioErp1ReceivedMessage()
   {
   }

   CRorgs::ERorgIds CRadioErp1ReceivedMessage::rorg() const
   {
      return m_rorg;
   }

   unsigned int CRadioErp1ReceivedMessage::senderId() const
   {
      return m_senderId;
   }

   std::string CRadioErp1ReceivedMessage::senderIdAsString() const
   {
      return m_senderIdAsString;
   }

   const std::vector<unsigned char>& CRadioErp1ReceivedMessage::data() const
   {
      return m_data;
   }

   unsigned char CRadioErp1ReceivedMessage::status() const
   {
      return m_status;
   }

   std::string CRadioErp1ReceivedMessage::senderIdToString(unsigned int senderId)
   {
      std::stringstream ss;
      ss << std::setfill('0') << std::setw(8) << std::uppercase << std::hex << senderId;
      return ss.str();
   }
} // namespace message


