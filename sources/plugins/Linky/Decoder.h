#pragma once

#include "IDecoder.h"
//#include "Keywords/RunningPeriod.h"
//#include "Keywords/ColorPrice.h"
#include "specificHistorizer/Color.h"
#include "specificHistorizer/Period.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

DECLARE_ENUM_HEADER(EContract,
((BASE))
((CREUSE))
((EJP))
((TEMPO))
);

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
   // [END] IDecoder implementation

private:
   //--------------------------------------------------------------
   /// \brief	                     Create the Device with the counter Id
   //--------------------------------------------------------------
   void createDeviceAndKeywords(bool isTriphases);


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
   boost::shared_ptr<yApi::historization::CEnergy> m_counter[10];
   boost::shared_ptr<yApi::historization::CEnergy> m_activeEnergyInjected;
   boost::shared_ptr<yApi::historization::CApparentPower> m_apparentPower[3];
   boost::shared_ptr<yApi::historization::CVoltage> m_meanVoltage[3];

   boost::shared_ptr<linky::specificHistorizers::CPeriod> m_TimePeriod;
   boost::shared_ptr<linky::specificHistorizers::CColor> m_forecastTomorrow;
   boost::shared_ptr<linky::specificHistorizers::CColor> m_forecastToday;

   boost::shared_ptr<yApi::IYPluginApi> m_api;
   std::string m_deviceName;

   bool m_isdeveloperMode;
   bool m_linkyEnableInCounter; // TODO : Check if this one always exist
   bool m_deviceCreated;
   unsigned char m_revision;
   
   enum
   {
      OP_NOT_DEFINED,
      OP_BASE,
      OP_CREUSE,
      OP_EJP,
      OP_TEMPO
   } m_optarif;

   static const std::string m_tag_ADSC;
   static const std::string m_tag_VTIC;
   static const std::string m_tag_NGTF;
   static const std::string m_tag_LTARF;
   static const std::string m_tag_EASF;
   static const std::string m_tag_EASD;
   static const std::string m_tag_STGE;
   static const std::string m_tag_EAIT;
   static const std::string m_tag_SINST1;
   static const std::string m_tag_SINST2;
   static const std::string m_tag_SINST3;
   static const std::string m_tag_SINSTS;
   static const std::string m_tag_UMOY1;
   static const std::string m_tag_UMOY2;
   static const std::string m_tag_UMOY3;

   static const int m_nb_period=11;
   static const int m_nb_contrat=4;

   static const std::string Enedis_Periode[m_nb_period];
   static const std::string Enedis_Contrat[m_nb_contrat];

   unsigned char m_indexFournisseurActif;
   bool m_producteur;
};
