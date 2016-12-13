#include "stdafx.h"
#include "RadioErp1ReceivedMessage.h"

namespace message
{
   CRadioErp1ReceivedMessage::CRadioErp1ReceivedMessage(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
      : m_rorg(CRorgs::toRorgId(esp3Packet->data()[0])),
        m_senderId(deviceIdToString(
           esp3Packet->data()[esp3Packet->data().size() - 5] << 24
           | esp3Packet->data()[esp3Packet->data().size() - 4] << 16
           | esp3Packet->data()[esp3Packet->data().size() - 3] << 8
           | esp3Packet->data()[esp3Packet->data().size() - 2])),
        m_destinationId(deviceIdToString(
           esp3Packet->optional()[1] << 24
           | esp3Packet->optional()[2] << 16
           | esp3Packet->optional()[3] << 8
           | esp3Packet->optional()[4])),
        m_status(esp3Packet->data()[esp3Packet->data().size() - 1]),
        m_userData(esp3Packet->data().begin() + 1, esp3Packet->data().begin() + esp3Packet->data().size() - 6 + 1)
   {
   }

   CRadioErp1ReceivedMessage::~CRadioErp1ReceivedMessage()
   {
   }

   CRorgs::ERorgIds CRadioErp1ReceivedMessage::rorg() const
   {
      return m_rorg;
   }

   std::string CRadioErp1ReceivedMessage::senderId() const
   {
      return m_senderId;
   }

   std::string CRadioErp1ReceivedMessage::destinationId() const
   {
      return m_destinationId;
   }

   const std::vector<unsigned char>& CRadioErp1ReceivedMessage::userData() const
   {
      return m_userData;
   }

   unsigned char CRadioErp1ReceivedMessage::status() const
   {
      return m_status;
   }
} // namespace message
