#include "stdafx.h"
#include "RadioErp1SendMessage.h"

namespace message
{
   CRadioErp1SendMessage::CRadioErp1SendMessage()
      : CEsp3SendPacket(RADIO_ERP1)
   {
   }

   CRadioErp1SendMessage::~CRadioErp1SendMessage()
   {
   }
} // namespace message


