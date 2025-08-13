#include "stdafx.h"
#include "CommonCommandSendMessage.h"

namespace message
{
    CCommonCommandSendMessage::CCommonCommandSendMessage(const ECommonCommand command)
        : CEsp3SendPacket(COMMON_COMMAND),
          m_command(command)
    {
    }

    boost::shared_ptr<const std::vector<unsigned char>> CCommonCommandSendMessage::buffer()
    {
        std::vector<unsigned char> data;

        data.push_back(static_cast<unsigned char>(m_command));

        CEsp3SendPacket::data(data);

        return CEsp3SendPacket::buffer();
    }
} // namespace message
