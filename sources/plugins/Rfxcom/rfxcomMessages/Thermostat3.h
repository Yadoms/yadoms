#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "IThermostat3Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat3 protocol support
   //--------------------------------------------------------------
   class CThermostat3 : public IRfxcomMessage
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
      CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& keyword, const std::string& command, const shared::CDataContainer& deviceDetails);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] subType              Device subType
      /// \param[in] manuallyDeviceCreationConfiguration The device concfiguration
      /// \throw                          shared::exception::CInvalidParameter or shared::exception::COutOfRange if fail to interpret configuration
      /// \note                           Use this constructor for manually device creation
      //--------------------------------------------------------------
      CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] rbuf                 The received buffer
      /// \param[in] rbufSize             Message size, received from Rfxcom
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CThermostat3();

      // IRfxcomMessage implementation
      virtual boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const;
      virtual const std::string& getDeviceName() const;
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
      /// \brief	The sub-type management
      //--------------------------------------------------------------
      boost::shared_ptr<IThermostat3Subtype> m_subTypeManager;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with rssi
      //--------------------------------------------------------------
      yApi::historization::CRssi m_rssi;
   };
} // namespace rfxcomMessages
