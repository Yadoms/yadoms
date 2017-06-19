#pragma once

#include "IDecoder.h"
#include "Keywords/RunningPeriod.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class implement the Linky protocol
//--------------------------------------------------------------
class CDecoder : public IDecoder
{
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   //--------------------------------------------------------------
   explicit CDecoder(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CDecoder();

   // IDecoder implementation
   void decodeLinkyMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                           const boost::shared_ptr<std::map<std::string, std::string>>& messages) override;
   bool isERDFCounterDesactivated() const override;
   // [END] IDecoder implementation

private:
   //--------------------------------------------------------------
   /// \brief	                     Create the Device with the counter Id
   //--------------------------------------------------------------
   void createDeviceAndKeywords(bool apparentPowerPresent);


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

   boost::shared_ptr<yApi::IYPluginApi> m_api;
   std::string m_deviceName;

   bool m_isdeveloperMode;

   bool m_linkyEnableInCounter; // TODO : Check if this one always exist

   bool m_deviceCreated;
   
   enum
   {
      OP_NOT_DEFINED,
      OP_BASE,
      OP_CREUSE,
      OP_EJP,
      OP_TEMPO
   } m_optarif;
};
