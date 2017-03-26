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

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] rbuf                 The received buffer
      /// \param[in] rbufSize             Message size, received from Rfxcom
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CCartelectronicEncoder(const RBUF& rbuf, size_t rbufSize);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CCartelectronicEncoder();

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
      boost::shared_ptr<yApi::historization::CCounter> m_counter1;

      //--------------------------------------------------------------
      /// \brief	The keyword Counter 2
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CCounter> m_counter2;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


