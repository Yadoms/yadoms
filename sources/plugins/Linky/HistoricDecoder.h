#pragma once

#include "IDecoder.h"
#include "Keywords/RunningPeriod.h"
#include "specificHistorizer/Color.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class implement the TeleInfo protocol
//--------------------------------------------------------------
class CHistoricDecoder : public IDecoder
{
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   //--------------------------------------------------------------
   explicit CHistoricDecoder(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CHistoricDecoder();

   // IDecoder implementation
   void decodeMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                      const boost::shared_ptr<std::map<std::string, std::vector<std::string>>>& messages) override;
   bool isERDFCounterDesactivated() const override;
   // [END] IDecoder implementation

private:

   enum EContracts
   {
      OP_NOT_DEFINED,
      OP_BASE,
      OP_CREUSE,
      OP_EJP,
      OP_TEMPO
   };

   //--------------------------------------------------------------
   /// \brief	                     Create the Device with the counter Id
   ///\param[in] monoPhase          the counter is mono phase
   //-----------------------------------------------------
   void createDeviceAndKeywords(const bool monoPhase);

   //--------------------------------------------------------------
   /// \brief	                     Create the keyword list depending of the tariff
   ///\param[in] contract             contract of the tele-info
   ///\param[in] monoPhase            the counter is mono phase
   //-----------------------------------------------------
   void constructKeywordList(const EContracts contract, const bool monoPhase);

   //--------------------------------------------------------------
   /// \brief	                     Create the keyword list depending of the tariff
   ///\param[in] tariff             contract of the tele-info
   //-----------------------------------------------------
   void declareKeywordList(const std::string& tariff);

   //--------------------------------------------------------------
   /// \brief	                     Process each tele-Info message
   ///\param[in] key                identifier of the message
   ///\param[in] value              value of the identifier
   //-----------------------------------------------------
   void processMessage(const std::string& key,
                       const std::vector<std::string>& value);

   //--------------------------------------------------------------
   /// \brief  Keywords list
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

   //--------------------------------------------------------------
   /// \brief  Details for the device
   //--------------------------------------------------------------
   shared::CDataContainerSharedPtr m_DeviceDetails;

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
   boost::shared_ptr<yApi::historization::CCurrent> m_instantCurrentPhase[3];
   boost::shared_ptr<yApi::historization::CApparentPower> m_apparentPower;


   boost::shared_ptr<CRunningPeriod> m_TimePeriod;
   boost::shared_ptr<linky::specificHistorizers::CColor> m_ForecastPeriod;
   boost::shared_ptr<yApi::historization::CSwitch> m_warningEJP;

   boost::shared_ptr<yApi::IYPluginApi> m_api;
   std::string m_deviceName;
   bool m_teleinfoEnableInCounter;
   bool m_deviceCreated;
   EContracts m_optarif;
   bool m_todayColorChanged;
   bool m_firstRun;
   bool m_newWarningEJPValue;
   bool m_ADCOalreadyReceived;

   static const std::string m_tag_ADCO;
   static const std::string m_tag_OPTARIF;
   static const std::string m_tag_BASE;
   static const std::string m_tag_HCHC;
   static const std::string m_tag_HCHP;
   static const std::string m_tag_EJPHPM;
   static const std::string m_tag_EJPHN;
   static const std::string m_tag_BBRHCJB;
   static const std::string m_tag_BBRHPJB;
   static const std::string m_tag_BBRHCJW;
   static const std::string m_tag_BBRHPJW;
   static const std::string m_tag_BBRHCJR;
   static const std::string m_tag_BBRHPJR;
   static const std::string m_tag_PTEC;
   static const std::string m_tag_IINST;
   static const std::string m_tag_IINST1;
   static const std::string m_tag_IINST2;
   static const std::string m_tag_IINST3;
   static const std::string m_tag_PAPP;
   static const std::string m_tag_DEMAIN;
   static const std::string m_tag_PEJP;
};