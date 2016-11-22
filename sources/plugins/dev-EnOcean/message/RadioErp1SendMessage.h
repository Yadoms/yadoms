#pragma once
#include "SendMessage.h"

namespace message
{
   //--------------------------------------------------------------
   /// \brief	EnOcean send command message
   //--------------------------------------------------------------
   class CRadioErp1SendMessage : public CEsp3SendPacket
   {
   public:
      CRadioErp1SendMessage();
      virtual ~CRadioErp1SendMessage();
   };
} // namespace message
