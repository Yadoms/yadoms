#pragma once
#include "ICartelectronicSubtype.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "specificHistorizers/Color.h"
#include "specificHistorizers/TeleInfoStatus.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Cartelectronic-Linky protocol support
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
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override;
      std::string idFromProtocol(const RBUF& rbuf) const override;
      char BatteryLevelFromProtocol(const RBUF& rbuf) override;
      char signalPowerFromProtocol(const RBUF& rbuf) override;
      std::string getModel() const override;
      // [END] ICartelectronicSubtype implementation

   private:

      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned int m_id;

      //--------------------------------------------------------------
      /// \brief	The keyword Counter 1
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CEnergy> m_counter1;

      //--------------------------------------------------------------
      /// \brief	The keyword Counter 2
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CEnergy> m_counter2;

      //--------------------------------------------------------------
      /// \brief	The Apparent Power
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CApparentPower> m_apparentePower;

      //--------------------------------------------------------------
      /// \brief	The Color for Tomorow if any
      //--------------------------------------------------------------
      boost::shared_ptr<teleInfo::specificHistorizers::CColor> m_forecast;

      //--------------------------------------------------------------
      /// \brief	TeleInfo Status
      //--------------------------------------------------------------
      boost::shared_ptr<teleInfo::specificHistorizers::CTeleInfoStatus> m_teleInfoStatus;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages