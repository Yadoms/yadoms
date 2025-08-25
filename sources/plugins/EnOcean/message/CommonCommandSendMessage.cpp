#include "stdafx.h"
#include "CommonCommandSendMessage.h"

namespace message
{
    CCommonCommandSendMessage::CCommonCommandSendMessage(const ECommonCommand command,
                                                         const std::vector<unsigned char>& data)
        : CEsp3SendPacket(COMMON_COMMAND),
          m_command(command),
          _data(data)
    {
    }

    boost::shared_ptr<const std::vector<unsigned char>> CCommonCommandSendMessage::buffer()
    {
        std::vector<unsigned char> data;

        data.push_back(static_cast<unsigned char>(m_command));
        if (!m_data.empty())
            data.insert(data.end(), m_data.begin(), m_data.end());

        CEsp3SendPacket::data(data);

        return CEsp3SendPacket::buffer();
    }
} // namespace message
