#include "stdafx.h"
#include "CommandSendMessage.h"

using namespace message::smart_ack;

CCommandSendMessage::CCommandSendMessage(const ESmartAckCommand command,
                                         const std::vector<unsigned char>& data)
    : CEsp3SendPacket(SMART_ACK_COMMAND),
      m_command(command),
      m_data(data)
{
}

boost::shared_ptr<const std::vector<unsigned char>> CCommandSendMessage::buffer()
{
    std::vector<unsigned char> data;

    data.push_back(static_cast<unsigned char>(m_command));
    if (!m_data.empty())
        data.insert(data.end(), m_data.begin(), m_data.end());

    CEsp3SendPacket::data(data);

    return CEsp3SendPacket::buffer();
}
