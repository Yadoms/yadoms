#include "stdafx.h"
#include "ReceivedMessage.h"

using namespace message::radioErp1;

CReceivedMessage::CReceivedMessage(const boost::shared_ptr<const CEsp3ReceivedPacket>& esp3Packet)
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
      m_dBm(-esp3Packet->optional()[5]),
      m_status(esp3Packet->data()[esp3Packet->data().size() - 1]),
      m_userData(esp3Packet->data().begin() + 1, esp3Packet->data().begin() + esp3Packet->data().size() - 6 + 1)
{
}

CReceivedMessage::~CReceivedMessage()
{
}

CRorgs::ERorgIds CReceivedMessage::rorg() const
{
    return m_rorg;
}

std::string CReceivedMessage::senderId() const
{
    return m_senderId;
}

std::string CReceivedMessage::destinationId() const
{
    return m_destinationId;
}

int CReceivedMessage::dBm() const
{
    return m_dBm;
}

const std::vector<unsigned char>& CReceivedMessage::userData() const
{
    return m_userData;
}

unsigned char CReceivedMessage::status() const
{
    return m_status;
}
