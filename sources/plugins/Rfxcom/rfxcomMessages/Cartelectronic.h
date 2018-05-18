#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "ICartelectronicSubtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Chime protocol support
   //--------------------------------------------------------------
   class CCartelectronic : public IRfxcomMessage
   {
   public:
      CCartelectronic(boost::shared_ptr<yApi::IYPluginApi> api,
                      const RBUF& rbuf,
                      size_t rbufSize);
      CCartelectronic(boost::shared_ptr<yApi::IYPluginApi> api,
                      unsigned char subType,
                      const boost::shared_ptr<const yApi::ISetDeviceConfiguration>& newDeviceConfiguration);

      virtual ~CCartelectronic();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      void filter() const override;
      void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      const std::string& getDeviceName() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() override;
      // [END] IRfxcomMessage implementation

   protected:
      void createSubType(boost::shared_ptr<yApi::IYPluginApi> api,
                         unsigned char subType,
                         const RBUF& rbuf,
                         size_t rbufSize);
      void createSubType(unsigned char subType,
                         const boost::shared_ptr<const yApi::ISetDeviceConfiguration>& newDeviceConfiguration);

      //--------------------------------------------------------------
      /// \brief	Declare the device
      /// \param[in] api                  Yadoms APi context
      //--------------------------------------------------------------
      void declare(boost::shared_ptr<yApi::IYPluginApi> api) const;

   private:
      //--------------------------------------------------------------
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      std::string m_id;

      //--------------------------------------------------------------
      /// \brief	The sub-type management
      //--------------------------------------------------------------
      boost::shared_ptr<ICartelectronicSubtype> m_subTypeManager;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with signalPower
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSignalPower> m_signalPower;

      //--------------------------------------------------------------
      /// \brief	The battery level (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CBatteryLevel> m_batteryLevel;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages
