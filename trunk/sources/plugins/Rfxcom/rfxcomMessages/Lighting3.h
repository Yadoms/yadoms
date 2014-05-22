#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxDefinitions.h"

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning3 protocol support
   //--------------------------------------------------------------
   class CLighting3 : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	               Constructor
      /// \param [in] system     The system code
      /// \param [in] channel    The channel code
      /// \param [in] cmnd       The command
      //--------------------------------------------------------------
      CLighting3(unsigned char system, unsigned short channel, unsigned char cmnd);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CLighting3();

      //--------------------------------------------------------------
      /// \brief	Get the buffer for serial link
      /// \return Const reference on buffer
      //--------------------------------------------------------------
      const boost::asio::const_buffer getBuffer() const;
      
   private:

      //--------------------------------------------------------------
      /// \brief	The buffer (for serial link)
      //--------------------------------------------------------------
      RBUF m_buffer;
   };
} // namespace rfxcomMessages
