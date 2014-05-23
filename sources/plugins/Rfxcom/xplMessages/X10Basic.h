#pragma once

#include "IXplMessage.h"
#include "../rfxcomMessages/RFXtrxDefinitions.h"
#include <shared/xpl/XplMessage.h>


namespace xplMessages
{

   //--------------------------------------------------------------
   /// \brief	The Lightning1 protocol support
   //--------------------------------------------------------------
   class CXplMsgX10Basic : public IXplMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor from the XPL message
      /// \param [in] xplMessage       The Xpl message to transmit
      //--------------------------------------------------------------
      CXplMsgX10Basic(const shared::xpl::CXplMessage& xplMessage);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CXplMsgX10Basic();

      // IXplMessage Implementation
      virtual boost::shared_ptr<rfxcomMessages::IRfxcomMessage> toRfxComMessage() const;
      // [END] IXplMessage Implementation

   protected:
      //--------------------------------------------------------------
      /// \brief	Create a RFXCom Loghtning1 message from the XPL message
      /// \return the created RFXCom message
      /// \throw shared::xpl::CXplException if fails to get data from the XPL message
      //--------------------------------------------------------------
      boost::shared_ptr<rfxcomMessages::IRfxcomMessage> createLighting1Msg() const;

      //--------------------------------------------------------------
      /// \brief	Create a RFXCom Loghtning3 message from the XPL message
      /// \return the created RFXCom message
      /// \throw shared::xpl::CXplException if fails to get data from the XPL message
      //--------------------------------------------------------------
      boost::shared_ptr<rfxcomMessages::IRfxcomMessage> createLighting3Msg() const;

      //--------------------------------------------------------------
      /// \brief	Create a RFXCom Curtain1 message from the XPL message
      /// \return the created RFXCom message
      /// \throw shared::xpl::CXplException if fails to get data from the XPL message
      //--------------------------------------------------------------
      boost::shared_ptr<rfxcomMessages::IRfxcomMessage> createCurtain1Msg() const;

   private:
      //--------------------------------------------------------------
      /// \brief	The XPl message
      //--------------------------------------------------------------
      const shared::xpl::CXplMessage& m_xplMessage;
   };

} // namespace xplMessages
