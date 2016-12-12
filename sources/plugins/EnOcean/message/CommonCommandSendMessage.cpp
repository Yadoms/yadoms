#include "stdafx.h"
#include "CommonCommandSendMessage.h"

namespace message
{
   CCommonCommandSendMessage::CCommonCommandSendMessage(ECommonCommand command)
      : CEsp3SendPacket(COMMON_COMMAND),
        m_command(command)
   {
   }

   CCommonCommandSendMessage::~CCommonCommandSendMessage()
   {
   }

   boost::shared_ptr<const std::vector<unsigned char>> CCommonCommandSendMessage::buffer()
   {
      auto data = std::vector<unsigned char>(1);

      size_t bufferIndex = 0;
      data[bufferIndex++] = static_cast<unsigned char>(m_command);

      CEsp3SendPacket::data(data);

      return CEsp3SendPacket::buffer();
   }
} // namespace message
