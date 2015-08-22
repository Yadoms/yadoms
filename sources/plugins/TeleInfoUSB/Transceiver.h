#pragma once

#include "ITransceiver.h"
#include <shared/communication/IAsyncPort.h>
#include "ITeleInfoMessage.h"
#include "TeleInfotrxHelpers.h"
#include "ISequenceNumberProvider.h"
#include "Keywords/PowerMeter.h"
#include "Keywords/CurrentMeter.h"
#include "Keywords/ApparentPowerMeter.h"
#include "Keywords/CurrentMeter.h"
#include "Keywords/RunningPeriod.h"
#include "Keywords/ForecastTomorrow.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class implement the TeleInfo protocol
//--------------------------------------------------------------
class CTransceiver : public ITransceiver
{  
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   //--------------------------------------------------------------
   CTransceiver();

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CTransceiver();

   // ITransceiver implementation
   virtual void decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> context,
	                                  std::string & PluginName,
                                      const shared::communication::CByteBuffer& data);
   // [END] ITransceiver implementation
   
private:
   //--------------------------------------------------------------
   /// \brief  The message sequence number
   //--------------------------------------------------------------
   boost::shared_ptr<ISequenceNumberProvider> m_seqNumberProvider;

   //--------------------------------------------------------------
   /// \brief	                     calculate et check the checksum of the message
   /// \param [in] m_buffer          Buffer of the message
   /// \return                       true if the checksum is valid
   //--------------------------------------------------------------

   bool isCheckSumOk(const unsigned char *m_buffer) const;

   //--------------------------------------------------------------
   /// \brief	                     Parse the message received
   /// \param [in] context           YADOMS API
   /// \param [in] PluginName        The plugin Name
   /// \param [in] pData             Pointer to the buffer
   /// \param [in] Len               Length of the buffer
   //--------------------------------------------------------------

   void ParseData(boost::shared_ptr<yApi::IYPluginApi> context, std::string & PluginName, const unsigned char *pData, int Len);

   //--------------------------------------------------------------
   /// \brief	                     Identify the message selected
   /// \param [in] m_buffer          The buffer that contains the message
   /// \param [in] context           YADOMS API
   /// \param [in] PluginName        The plugin Name
   //--------------------------------------------------------------

   void MatchLine( const unsigned char* m_buffer, 
	               boost::shared_ptr<yApi::IYPluginApi> context, 
				   std::string & PluginName );

   //--------------------------------------------------------------
   /// \brief  Keywords list
   //--------------------------------------------------------------

   std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > m_KeywordList;

   //--------------------------------------------------------------
   /// \brief  Keywords
   //--------------------------------------------------------------

	boost::shared_ptr<CPowerMeter> m_Base;

	//Low cost / Normal cost counters
	boost::shared_ptr<CPowerMeter> m_LowCost;
	boost::shared_ptr<CPowerMeter> m_NormalCost;

	//Yellow counters
	boost::shared_ptr<CPowerMeter> m_EJPPeakPeriod;
	boost::shared_ptr<CPowerMeter> m_EJPNormalPeriod;

	//Blue tempo counters
	boost::shared_ptr<CPowerMeter> m_TempoBlueDaysLowCostPeriod;
	boost::shared_ptr<CPowerMeter> m_TempoBlueDaysNormalCostPeriod;

	//White tempo counters
	boost::shared_ptr<CPowerMeter> m_TempoWhiteDaysLowCostPeriod;
	boost::shared_ptr<CPowerMeter> m_TempoWhiteDaysNormalCostPeriod;

	//Red tempo counters
	boost::shared_ptr<CPowerMeter> m_TempoRedDaysLowCostPeriod;
	boost::shared_ptr<CPowerMeter> m_TempoRedDaysNormalCostPeriod;

	boost::shared_ptr<CCurrentMeter> m_InstantCurrent;
	boost::shared_ptr<CCurrentMeter> m_MaxCurrent;
	boost::shared_ptr<CApparentPowerMeter> m_ApparentPower;

	boost::shared_ptr<CRunningPeriod> m_TimePeriod;
	boost::shared_ptr<CForecastTomorrow> m_ForecastPeriod;
};
