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
   class CRfy : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] command              The command
      /// \param[in] deviceParameters     The device parameters
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      /// \note                           Use this constructor for command (to build RFXCom message)
      //--------------------------------------------------------------
      CRfy(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] rbuf                 The received buffer
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CRfy(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CRfy();

      // IRfxcomMessage implementation
      virtual const shared::communication::CByteBuffer encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const;
      // [END] IRfxcomMessage implementation
      
   protected:
      //--------------------------------------------------------------
      /// \brief	Global initialization method
      /// \param[in] context              Yadoms APi context
      //--------------------------------------------------------------
      void Init(boost::shared_ptr<yApi::IYadomsApi> context);

      //--------------------------------------------------------------
      /// \brief	                        Build the device name
      //--------------------------------------------------------------
      void buildDeviceName();

      //--------------------------------------------------------------
      /// \brief	                        Build the sensor model
      //--------------------------------------------------------------
      void buildDeviceModel();

      //--------------------------------------------------------------
      /// \brief	                        Convert Yadoms command to protocol value
      /// \param[in] curtainState         The state from Yadoms
      /// \return                         The value known by the protocol
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      //--------------------------------------------------------------
      static unsigned char toProtocolState(const yApi::historization::CCurtain& curtainState);

   private:
      //--------------------------------------------------------------
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

      //--------------------------------------------------------------
      /// \brief	The device unit code
      //--------------------------------------------------------------
      unsigned char m_unitCode;

      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned int m_id;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	The device model
      //--------------------------------------------------------------
      std::string m_deviceModel;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with state
      //--------------------------------------------------------------
      yApi::historization::CCurtain m_state;
   };
} // namespace rfxcomMessages
