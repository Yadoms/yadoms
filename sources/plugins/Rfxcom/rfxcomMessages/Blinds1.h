#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yadomsApi/IYadomsApi.h>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Blinds1 protocol support
   //--------------------------------------------------------------
   class CBlinds1 : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] command              The command
      /// \param[in] deviceDetails        The device parameters
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      /// \note                           Use this constructor for command (to build RFXCom message)
      //--------------------------------------------------------------
      CBlinds1(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceDetails);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] subType              Device subType
      /// \param[in] manuallyDeviceCreationConfiguration The device concfiguration
      /// \throw                          shared::exception::CInvalidParameter or shared::exception::COutOfRange if fail to interpret configuration
      /// \note                           Use this constructor for manually device creation
      //--------------------------------------------------------------
      CBlinds1(boost::shared_ptr<yApi::IYadomsApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] rbuf                 The received buffer
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CBlinds1(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CBlinds1();

      // IRfxcomMessage implementation
      virtual boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const;
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
      /// \brief	                        Convert protocol value to Yadoms state
      /// \param[in] protocolState        The value known by the protocol
      /// \return                         The Yadoms compliant value
      /// \throw                          shared::exception::CInvalidParameter if fails to interpret command
      //--------------------------------------------------------------
      static yApi::historization::ECurtainCommand fromProtocolState(unsigned char protocolCmnd);

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
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned int m_id;

      //--------------------------------------------------------------
      /// \brief	The device unit code
      //--------------------------------------------------------------
      unsigned char m_unitCode;

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

      //--------------------------------------------------------------
      /// \brief	The battery level (percent)
      //--------------------------------------------------------------
      yApi::historization::CBatteryLevel m_batteryLevel;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with rssi
      //--------------------------------------------------------------
      yApi::historization::CRssi m_rssi;
   };
} // namespace rfxcomMessages
