#pragma once

#include "ITransceiver.h"
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
   explicit CTransceiver(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CTransceiver();

   // ITransceiver implementation
   void decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                              const boost::shared_ptr<std::map<std::string, std::string>>& messages) override;
   bool isInformationUpdated() const override;
   void ResetRefreshTags() override;
   bool isERDFCounterDesactivated() const override;
   // [END] ITransceiver implementation

private:
   //--------------------------------------------------------------
   /// \brief	                     Create the Device with the counter Id
   //--------------------------------------------------------------
   void createDeviceAndKeywords(void);


   void createKeywordList(const std::string& tariff);
   void processMessage(const std::string& key,
                       const std::string& value);

   //--------------------------------------------------------------
   /// \brief  Keywords list
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

   //--------------------------------------------------------------
   /// \brief  Details for the device
   //--------------------------------------------------------------
   shared::CDataContainer m_DeviceDetails;

   //--------------------------------------------------------------
   /// \brief  Keywords
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CEnergy> m_baseCounter;
   boost::shared_ptr<yApi::historization::CEnergy> m_lowCostCounter;
   boost::shared_ptr<yApi::historization::CEnergy> m_normalCostCounter;
   boost::shared_ptr<yApi::historization::CEnergy> m_EJPPeakPeriod;
   boost::shared_ptr<yApi::historization::CEnergy> m_EJPNormalPeriod;
   boost::shared_ptr<yApi::historization::CEnergy> m_tempoBlueDaysLowCostPeriod;
   boost::shared_ptr<yApi::historization::CEnergy> m_tempoBlueDaysNormalCostPeriod;
   boost::shared_ptr<yApi::historization::CEnergy> m_tempoRedDaysLowCostPeriod;
   boost::shared_ptr<yApi::historization::CEnergy> m_tempoRedDaysNormalCostPeriod;
   boost::shared_ptr<yApi::historization::CEnergy> m_tempoWhiteDaysLowCostPeriod;
   boost::shared_ptr<yApi::historization::CEnergy> m_tempoWhiteDaysNormalCostPeriod;
   boost::shared_ptr<yApi::historization::CCurrent> m_instantCurrent;
   boost::shared_ptr<yApi::historization::CApparentPower> m_apparentPower;


   boost::shared_ptr<CRunningPeriod> m_TimePeriod;
   boost::shared_ptr<CForecastTomorrow> m_ForecastPeriod;

   boost::shared_ptr<yApi::IYPluginApi> m_api;
   std::string m_deviceName;

   bool m_isdeveloperMode;

   //Contract Options -> Abonnement
   typedef enum
   {
      OP_NOT_DEFINED,
      OP_BASE,
      OP_CREUSE,
      OP_EJP,
      OP_TEMPO
   } Abonnement;

   bool m_adcoUpdated; //TODO faire ménage dans booléens
   bool m_baseUpdated;
   bool m_lowCostUpdated;
   bool m_normalCostUpdated;
   bool m_instantCurrentUpdated;
   bool m_apparentPowerUpdated;

   bool m_eJPPeakPeriodUpdated;
   bool m_eJPNormalPeriodUpdated;

   bool m_tempoBlueDaysLowCostUpdated;
   bool m_tempoBlueDaysNormalCostUpdated;

   bool m_tempoWhiteDaysLowCostUpdated;
   bool m_tempoWhiteDaysNormalCostUpdated;

   bool m_tempoRedDaysLowCostUpdated;
   bool m_tempoRedDaysNormalCostUpdated;

   bool m_timePeriodUpdated;
   bool m_forecastPeriodUpdated;

   bool m_deviceCreated;

   Abonnement m_optarif;
};
