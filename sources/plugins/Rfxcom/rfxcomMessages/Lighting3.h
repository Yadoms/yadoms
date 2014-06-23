#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxDefinitions.h"
#include "../ISequenceNumberProvider.h"
#include <shared/DataContainer.h>

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning3 protocol support
   //--------------------------------------------------------------
   class CLighting3 : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] command              The command
      /// \param[in] deviceParameters     The device parameters
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      //--------------------------------------------------------------
      CLighting3(const std::string& command, const shared::CDataContainer& deviceParameters, boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CLighting3();

      //--------------------------------------------------------------
      /// \brief	Get the buffer for serial link
      /// \return Const reference on buffer
      //--------------------------------------------------------------
      const boost::asio::const_buffer getBuffer() const;
      
   protected:
      //--------------------------------------------------------------
      /// \brief	                        Convert Yadoms command to protocol value
      /// \param[in] yadomsCommand        The command from Yadoms
      /// \return                         The value known by the protocol
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      //--------------------------------------------------------------
      unsigned char toLighting3Command(const std::string& yadomsCommand) const;

   private:
      //--------------------------------------------------------------
      /// \brief	The buffer (for serial link)
      //--------------------------------------------------------------
      RBUF m_buffer;
   };
} // namespace rfxcomMessages
