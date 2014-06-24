#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yadomsApi/IYadomsApi.h>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning6 protocol support
   //--------------------------------------------------------------
   class CLighting6 : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] command              The command
      /// \param[in] deviceParameters     The device parameters
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      /// \note                           Use this constructor for command (to build RFXCom message)
      //--------------------------------------------------------------
      CLighting6(const std::string& command, const shared::CDataContainer& deviceParameters);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] buffer               The received buffer
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CLighting6(const RBUF& buffer);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CLighting6();

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
      static unsigned char toProtocolState(const std::string& yadomsCommand);
      
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
      /// \brief	The device group code
      //--------------------------------------------------------------
      unsigned char m_groupCode;

      //--------------------------------------------------------------
      /// \brief	The device unit code
      //--------------------------------------------------------------
      unsigned char m_unitCode;

      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned short m_id;

      //--------------------------------------------------------------
      /// \brief	The state
      //--------------------------------------------------------------
      unsigned char m_state;

      //--------------------------------------------------------------
      /// \brief	The RSSI (received messages only)
      //--------------------------------------------------------------
      unsigned char m_rssi;
   };
} // namespace rfxcomMessages
