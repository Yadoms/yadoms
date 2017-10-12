#pragma once

#include "ICartelectronicSubtype.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "specificHistorizers/TeleInfoStatus.h"
#include "specificHistorizers/Color.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Cartelectronic-Encoder protocol support
   //--------------------------------------------------------------
   class CCartelectronicLinky : public ICartelectronicSubtype
   {
   public:

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] rbuf                 The received buffer
      /// \param[in] rbufSize             Message size, received from Rfxcom
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CCartelectronicLinky(const RBUF& rbuf, size_t rbufSize);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CCartelectronicLinky();

      // ICartelectronicSubtype implementation
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      std::string idFromProtocol(const RBUF& rbuf) const override;
      char BatteryLevelFromProtocol(const RBUF& rbuf) override;
      char signalPowerFromProtocol(const RBUF& rbuf) override;
      std::string getModel() const override;
      // [END] ICartelectronicSubtype implementation

   private:
      unsigned int m_id;
      
      boost::shared_ptr<teleInfo::specificHistorizers::CTeleInfoStatus> m_teleInfoStatus;
      boost::shared_ptr<yApi::historization::CVoltage> m_voltage;
      boost::shared_ptr<yApi::historization::CPower> m_power;
      boost::shared_ptr<teleInfo::specificHistorizers::CColor> m_forecast;

      //TODO manque state et contract_type

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages


