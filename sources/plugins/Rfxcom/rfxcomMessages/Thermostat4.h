#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat4 protocol support
   //--------------------------------------------------------------
   class CThermostat4 : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] keyword              Keyword concerned by the command
      /// \param[in] command              The command
      /// \param[in] deviceDetails        The device parameters
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      /// \note                           Use this constructor for command (to build RFXCom message)
      //--------------------------------------------------------------
      CThermostat4(boost::shared_ptr<yApi::IYPluginApi> api,
                   const std::string& keyword,
                   const std::string& command,
                   const shared::CDataContainer& deviceDetails);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] subType              Device subType
      /// \param[in] name                 Device name
      /// \param[in] manuallyDeviceCreationConfiguration The device concfiguration
      /// \throw                          shared::exception::CInvalidParameter or shared::exception::COutOfRange if fail to interpret configuration
      /// \note                           Use this constructor for manually device creation
      //--------------------------------------------------------------
      CThermostat4(boost::shared_ptr<yApi::IYPluginApi> api,
                   unsigned int subType,
         const std::string& name,
                   const shared::CDataContainer& manuallyDeviceCreationConfiguration);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] rbuf                 The received buffer
      /// \param[in] rbufSize             Message size, received from Rfxcom
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CThermostat4(boost::shared_ptr<yApi::IYPluginApi> api,
                   const RBUF& rbuf,
                   size_t rbufSize);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CThermostat4();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
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
      /// \brief	                        Normalize the fan speed (% ==> 0-6 value
      //--------------------------------------------------------------
      static unsigned char normalizeFanSpeed(int dimmableValue);

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

      shared::CDataContainer m_deviceDetails;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with on/off state
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_onOff;

      //--------------------------------------------------------------
      /// \brief	The flame power
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CDimmable> m_flame;

      //--------------------------------------------------------------
      /// \brief	Fans management
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CDimmable> m_fan1;
      boost::shared_ptr<yApi::historization::CSwitch> m_fan1AutoMode;
      boost::shared_ptr<yApi::historization::CDimmable> m_fan2;
      boost::shared_ptr<yApi::historization::CSwitch> m_fan2AutoMode;
      boost::shared_ptr<yApi::historization::CDimmable> m_fan3;
      boost::shared_ptr<yApi::historization::CSwitch> m_fan3AutoMode;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages
