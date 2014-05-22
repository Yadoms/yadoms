#pragma once


namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The XPL to RFXCom message converter interface
   //--------------------------------------------------------------
   class IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IRfxcomMessage() {}

      //--------------------------------------------------------------
      /// \brief	Get the buffer for serial link
      /// \return Const reference on buffer
      //--------------------------------------------------------------
      virtual const boost::asio::const_buffer getBuffer() const = 0;
   };
}
