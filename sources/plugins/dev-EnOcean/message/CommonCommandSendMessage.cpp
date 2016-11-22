#include "stdafx.h"
#include "CommonCommandSendMessage.h"

namespace message
{
   CCommonCommandSendMessage::CCommonCommandSendMessage()
      : CEsp3SendPacket(COMMON_COMMAND)
   {
   }

   CCommonCommandSendMessage::~CCommonCommandSendMessage()
   {
   }
} // namespace message


