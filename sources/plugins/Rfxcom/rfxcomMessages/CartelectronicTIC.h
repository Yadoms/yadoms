#pragma once

#include "ICartelectronicSubtype.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "specificHistorizers/Color.h"
#include "specificHistorizers/Period.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

   //--------------------------------------------------------------
   /// \brief	The Cartelectronic-TIC protocol support
   //--------------------------------------------------------------
   class CCartelectronicTIC  : public ICartelectronicSubtype
   {
   public:

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] rbuf                 The received buffer
      /// \param[in] rbufSize             Message size, received from Rfxcom
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CCartelectronicTIC( const RBUF& rbuf, size_t rbufSize );

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CCartelectronicTIC();

      // ICartelectronicSubtype implementation
      void declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const override;
      void historize(std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > &KeywordList) const override;
      std::string idFromProtocol( const RBUF& rbuf ) const override;
      const char BatteryLevelFromProtocol( const RBUF& rbuf ) override;
	  const char RssiFromProtocol( const RBUF& rbuf ) override;
	  std::string getModel() const override;
      // [END] ICartelectronicSubtype implementation

   private:

      //Contract Options
      typedef enum {
		   OP_NOT_DEFINED,
		   OP_BASE,
		   OP_CREUSE,
		   OP_EJP,
		   OP_TEMPO
      } Contract;

	  Contract m_SubscribeContract;

      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned int m_id;

      //--------------------------------------------------------------
      /// \brief	The keyword Counter 1
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CEnergy> m_Counter1;

      //--------------------------------------------------------------
      /// \brief	The keyword Counter 2
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CEnergy> m_Counter2;

      //--------------------------------------------------------------
      /// \brief	The Apparent Power
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CApparentPower> m_ApparentePower;

	  //--------------------------------------------------------------
	  /// \brief	The Color for Tomorow if any
	  //--------------------------------------------------------------
	  boost::shared_ptr<teleInfo::CColor> m_Forecast;

	  //--------------------------------------------------------------
	  /// \brief	The running period time
	  //--------------------------------------------------------------
	  boost::shared_ptr<teleInfo::CPeriod> m_Period;

   };
} // namespace rfxcomMessages
