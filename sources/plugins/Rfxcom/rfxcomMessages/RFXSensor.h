#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The RFXSensor protocol support (reception only)
   //--------------------------------------------------------------
   class CRFXSensor : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] rbuf                 The received buffer
      /// \param[in] rbufSize             Message size, received from Rfxcom
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CRFXSensor(boost::shared_ptr<yApi::IYPluginApi> api, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CRFXSensor();

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

      //--------------------------------------------------------------
      /// \brief	                        Build the sensor model
      //--------------------------------------------------------------
      void buildDeviceModel();

      //--------------------------------------------------------------
      /// \brief	                        Process the message sub-type
      /// \param[in] msgId                Message ID
      //--------------------------------------------------------------
      void processMessage(unsigned int msgId);

   private:
      //--------------------------------------------------------------
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

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
      /// \brief	The temperature (°C)
      //--------------------------------------------------------------
      yApi::historization::CTemperature m_temperature;
      bool m_temperatureAvailable;

      //--------------------------------------------------------------
      /// \brief	The A/D voltage (V)
      //--------------------------------------------------------------
      yApi::historization::CVoltage m_adVoltage;
      bool m_adVoltageAvailable;

      //--------------------------------------------------------------
      /// \brief	The voltage (V)
      //--------------------------------------------------------------
      yApi::historization::CVoltage m_voltage;
      bool m_voltageAvailable;

      //--------------------------------------------------------------
      /// \brief	The RSSI (percent)
      //--------------------------------------------------------------
      yApi::historization::CRssi m_rssi;
   };
} // namespace rfxcomMessages
