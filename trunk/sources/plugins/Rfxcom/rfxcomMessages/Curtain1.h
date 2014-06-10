#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxDefinitions.h"
#include "../ISequenceNumberProvider.h"

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Curtain1 protocol support
   //--------------------------------------------------------------
   class CCurtain1 : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	               Constructor
      /// \param [in] data       The message data
      /// \param[in] seqNumberProvider The sequence number provider
      //--------------------------------------------------------------
      CCurtain1(const boost::property_tree::ptree& data, boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CCurtain1();

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
