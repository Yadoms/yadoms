#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

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
      /// \param[in] api                  Yadoms APi context
      /// \param[in] command              The command
      /// \param[in] deviceDetails        The device parameters
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      /// \note                           Use this constructor for command (to build RFXCom message)
      //--------------------------------------------------------------
      CLighting6(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& command,
                 const shared::CDataContainer& deviceDetails);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] subType              Device subType
      /// \param[in] manuallyDeviceCreationConfiguration The device concfiguration
      /// \throw                          shared::exception::CInvalidParameter or shared::exception::COutOfRange if fail to interpret configuration
      /// \note                           Use this constructor for manually device creation
      //--------------------------------------------------------------
      CLighting6(boost::shared_ptr<yApi::IYPluginApi> api,
                 unsigned int subType,
                 const shared::CDataContainer& manuallyDeviceCreationConfiguration);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] rbuf                 The received buffer
      /// \param[in] rbufSize             Message size, received from Rfxcom
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CLighting6(boost::shared_ptr<yApi::IYPluginApi> api,
                 const RBUF& rbuf,
                 size_t rbufSize);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CLighting6();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      const std::string& getDeviceName() const override;
      // [END] IRfxcomMessage implementation

   protected:
      //--------------------------------------------------------------
      /// \brief	Global initialization method
      /// \param[in] api                  Yadoms APi context
      //--------------------------------------------------------------
      void Init(boost::shared_ptr<yApi::IYPluginApi> api);

      //--------------------------------------------------------------
      /// \brief	                        Build the device name
      //--------------------------------------------------------------
      void buildDeviceName();

      //--------------------------------------------------------------
      /// \brief	                        Build the sensor model
      //--------------------------------------------------------------
      void buildDeviceModel();

      //--------------------------------------------------------------
      /// \brief	                        Convert Yadoms state command to protocol value
      /// \param[in] switchState          The state from Yadoms
      /// \return                         The value known by the protocol
      //--------------------------------------------------------------
      static unsigned char toProtocolState(const yApi::historization::CSwitch& switchState);

      //--------------------------------------------------------------
      /// \brief	                        Convert protocol value to Yadoms state
      /// \param[in] protocolState        The value known by the protocol
      /// \return                         The Yadoms compliant value
      /// \throw                          shared::exception::CInvalidParameter if fails to interpret command
      //--------------------------------------------------------------
      static bool fromProtocolState(unsigned char protocolState);

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
      boost::shared_ptr<yApi::historization::CSwitch> m_state;

      //--------------------------------------------------------------
      /// \brief	The RSSI (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CRssi> m_rssi;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


