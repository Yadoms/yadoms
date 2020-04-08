#pragma once

#include "ICartelectronicSubtype.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Cartelectronic-Encoder protocol support
   //--------------------------------------------------------------
   class CCartelectronicEncoder : public ICartelectronicSubtype
   {
   public:

      /// Constructor used for received messages
      CCartelectronicEncoder(const RBUF& rbuf, size_t rbufSize);

      /// Constructor used for device configuration
      explicit CCartelectronicEncoder(const boost::shared_ptr<const yApi::ISetDeviceConfiguration>& deviceConfiguration);

      virtual ~CCartelectronicEncoder();

      // ICartelectronicSubtype implementation
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords(
         const shared::CDataContainerSharedPtr& deviceConfiguration) override;
      std::string idFromProtocol(const RBUF& rbuf) const override;
      char batteryLevelFromProtocol(const RBUF& rbuf) override;
      char signalPowerFromProtocol(const RBUF& rbuf) override;
      std::string getModel() const override;
      // [END] ICartelectronicSubtype implementation

   private:
      boost::shared_ptr<yApi::historization::CCounter> m_counterKw1;
      boost::shared_ptr<yApi::historization::CEnergy> m_energyKw1;
      boost::shared_ptr<yApi::historization::CVolume> m_waterLiterKw1;
      boost::shared_ptr<yApi::historization::CVolume> m_waterQuarterLiterKw1;
      boost::shared_ptr<yApi::historization::CVolume> m_gasDeciM3Kw1;
      boost::shared_ptr<yApi::historization::CVolume> m_gasCentiM3Kw1;
      boost::shared_ptr<yApi::historization::CCounter> m_counterKw2;
      boost::shared_ptr<yApi::historization::CEnergy> m_energyKw2;
      boost::shared_ptr<yApi::historization::CVolume> m_waterLiterKw2;
      boost::shared_ptr<yApi::historization::CVolume> m_waterQuarterLiterKw2;
      boost::shared_ptr<yApi::historization::CVolume> m_gasDeciM3Kw2;
      boost::shared_ptr<yApi::historization::CVolume> m_gasCentiM3Kw2;

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages
