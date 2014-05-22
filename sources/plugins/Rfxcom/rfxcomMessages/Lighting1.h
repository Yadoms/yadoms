#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxDefinitions.h"

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning1 protocol support
   //--------------------------------------------------------------
   class CLighting1 : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	               Constructor
      /// \param [in] subType    The message subtype
      /// \param [in] houseCode  The house code
      /// \param [in] unitCode   The unit code
      /// \param [in] cmnd       The command
      //--------------------------------------------------------------
      CLighting1(unsigned char subType, unsigned char houseCode, unsigned char unitCode, unsigned char cmnd);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CLighting1();

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
