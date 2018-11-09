#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Wind protocol support (reception only)
   //--------------------------------------------------------------
   class CWind : public IRfxcomMessage
   {
   public:
      CWind(boost::shared_ptr<yApi::IYPluginApi> api,
            const RBUF& rbuf,
            size_t rbufSize);

      virtual ~CWind();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      void filter() const override;
      void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      const std::string& getDeviceName() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() override;
      // [END] IRfxcomMessage implementation

   protected:
      void buildDeviceName();
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
      /// \brief	The wind direction
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CDirection> m_windDirection;

      //--------------------------------------------------------------
      /// \brief	The wind average speed
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSpeed> m_windAverageSpeed;

      //--------------------------------------------------------------
      /// \brief	The wind max speed
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSpeed> m_windMaxSpeed;

      //--------------------------------------------------------------
      /// \brief	The temperature
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CTemperature> m_temperature;

      //--------------------------------------------------------------
      /// \brief	The chill temperature
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CTemperature> m_chillTemperature;

      //--------------------------------------------------------------
      /// \brief	The battery level (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CBatteryLevel> m_batteryLevel;

      //--------------------------------------------------------------
      /// \brief	The signalPower (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSignalPower> m_signalPower;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


