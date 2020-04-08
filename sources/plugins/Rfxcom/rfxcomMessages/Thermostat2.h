#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat2 protocol support
   //--------------------------------------------------------------
   class CThermostat2 : public IRfxcomMessage
   {
   public:
      CThermostat2(boost::shared_ptr<yApi::IYPluginApi> api,
                   const std::string& command,
                   const shared::CDataContainerSharedPtr& deviceDetails);

      CThermostat2(boost::shared_ptr<yApi::IYPluginApi> api,
                   unsigned int subType,
                   const std::string& name,
                   const shared::CDataContainerSharedPtr& manuallyDeviceCreationConfiguration);

      CThermostat2(boost::shared_ptr<yApi::IYPluginApi> api,
                   const RBUF& rbuf,
                   size_t rbufSize);

      virtual ~CThermostat2();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      void filter() const override;
      void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      const std::string& getDeviceName() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() override;
      // [END] IRfxcomMessage implementation

   protected:
      //--------------------------------------------------------------
      /// \brief	                        Build the device name
      //--------------------------------------------------------------
      void buildDeviceName();

      //--------------------------------------------------------------
      /// \brief	                        Build the sensor model
      //--------------------------------------------------------------
      void buildDeviceModel();

      void buildDeviceDetails();

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
      /// \brief	The device unit code
      //--------------------------------------------------------------
      unsigned int m_unitCode;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	The device model
      //--------------------------------------------------------------
      std::string m_deviceModel;

      shared::CDataContainerSharedPtr m_deviceDetails;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with state
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_state;

      //--------------------------------------------------------------
      /// \brief	The signalPower (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSignalPower> m_signalPower;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages


