#pragma once

#include "../rfxcomMessages/IRfxcomMessage.h"

namespace xplMessages
{

   //--------------------------------------------------------------
   /// \brief	The XPL to RFXCom message converter interface
   //--------------------------------------------------------------
   class IXplMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IXplMessage() {}

      //--------------------------------------------------------------
      /// \brief	Translate the XPL message to Rfxcom format
      /// \return the create XPL message
      /// \throw shared::exception::CInvalidParameter if no compatible RFXCom message was found
      //--------------------------------------------------------------
      virtual boost::shared_ptr<rfxcomMessages::IRfxcomMessage> toRfxComMessage() const = 0;
   };

} // namespace xplMessages