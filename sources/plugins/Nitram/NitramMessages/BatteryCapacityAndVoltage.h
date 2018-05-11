#pragma once

#include "INitramMessage.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace nitramMessages
{
   //--------------------------------------------------------------
   /// \brief	The Barometric protocol support (reception only)
   //--------------------------------------------------------------
   class CBatteryCapacityAndVoltage : public INitramMessage
   {
   public:

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CBatteryCapacityAndVoltage(std::string deviceName);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CBatteryCapacityAndVoltage();

      // INitramMessage implementation
      void decode(boost::shared_ptr<yApi::IYPluginApi> api,
                  const shared::communication::CByteBuffer& buffer) override;
      boost::shared_ptr<shared::communication::CByteBuffer> encode() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) override;
      bool onlyAck() const override;
      // [END] INitramMessage implementation

   protected:

   private:

      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	The battery level (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CBatteryLevel> m_batteryLevel;

      //--------------------------------------------------------------
      /// \brief	The signal power (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CVoltage> m_batteryVoltage;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace nitramMessages
