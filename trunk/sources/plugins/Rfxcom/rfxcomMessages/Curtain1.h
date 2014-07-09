#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yadomsApi/IYadomsApi.h>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Curtain1 protocol support
   //--------------------------------------------------------------
   class CCurtain1 : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] command              The command
      /// \param[in] deviceParameters     The device parameters
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      /// \note                           Use this constructor for command (to build RFXCom message)
      //--------------------------------------------------------------
      CCurtain1(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] buffer               The received buffer
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CCurtain1(const RBUF& buffer);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CCurtain1();

      // IRfxcomMessage implementation
      virtual const CByteBuffer encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const;
      // [END] IRfxcomMessage implementation
      
   protected:
      //--------------------------------------------------------------
      /// \brief	                        Convert Yadoms command to protocol value
      /// \param[in] yadomsCommand        The command from Yadoms
      /// \return                         The value known by the protocol
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      //--------------------------------------------------------------
      static unsigned char toProtocolState(const shared::CDataContainer& yadomsCommand);
      
      //--------------------------------------------------------------
      /// \brief	                        Convert protocol value to Yadoms state
      /// \param[in] protocolState        The value known by the protocol
      /// \return                         The command for Yadoms
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      //--------------------------------------------------------------
      static std::string toYadomsState(unsigned char protocolState);

   private:
      //--------------------------------------------------------------
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

      //--------------------------------------------------------------
      /// \brief	The device house code
      //--------------------------------------------------------------
      unsigned char m_houseCode;

      //--------------------------------------------------------------
      /// \brief	The device unit code
      //--------------------------------------------------------------
      unsigned char m_unitCode;

      //--------------------------------------------------------------
      /// \brief	The state
      //--------------------------------------------------------------
      unsigned char m_state;
   };
} // namespace rfxcomMessages
