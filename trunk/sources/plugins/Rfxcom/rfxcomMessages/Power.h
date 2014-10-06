#pragma once

#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yadomsApi/IYadomsApi.h>

namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The power protocol support (reception only)
   //--------------------------------------------------------------
   class CPower : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] context              Yadoms APi context
      /// \param[in] rbuf                 The received buffer
      /// \param[in] seqNumberProvider    The sequence number provider
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CPower(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CPower();

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
      /// \brief	The voltage (V)
      //--------------------------------------------------------------
      yApi::commands::CVoltage m_voltage;

      //--------------------------------------------------------------
      /// \brief	The current (A)
      //--------------------------------------------------------------
      yApi::commands::CCurrent m_current;

      //--------------------------------------------------------------
      /// \brief	The instant power (W)
      //--------------------------------------------------------------
      yApi::commands::CPower m_instantPower;

      //--------------------------------------------------------------
      /// \brief	The total power (Wh)
      //--------------------------------------------------------------
      yApi::commands::CPower m_totalPower;

      //--------------------------------------------------------------
      /// \brief	The power factor (-1 to 1)
      //--------------------------------------------------------------
      yApi::commands::CPowerFactor m_powerFactor;

      //--------------------------------------------------------------
      /// \brief	The frequency (Hz)
      //--------------------------------------------------------------
      yApi::commands::CFrequency m_frequency;

      //--------------------------------------------------------------
      /// \brief	The RSSI (percent)
      //--------------------------------------------------------------
      yApi::commands::CRssi m_rssi;
   };
} // namespace rfxcomMessages
