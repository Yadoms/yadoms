#pragma once

#include "IDecoder.h"
#include "specificHistorizer/Color.h"

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
                           const boost::shared_ptr<std::map<std::string, std::vector<std::string>>>& messages) override;
   // [END] IDecoder implementation

private:
   //--------------------------------------------------------------
   /// \brief	                     Create the Device with the counter Id
   //--------------------------------------------------------------
   void createDeviceAndKeywords();
   void createFirstKeywordList(bool isTriphases);
   void createRunningKeywordList(bool isTriphases);
   void processMessage(const std::string& key,
                       const std::vector<std::string>& value);

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
   boost::shared_ptr<yApi::historization::CText> m_runningPeriod;
   boost::shared_ptr<yApi::historization::CCounter> m_runningIndex;

   boost::shared_ptr<linky::specificHistorizers::CColor> m_tomorrowColor;
   boost::shared_ptr<linky::specificHistorizers::CColor> m_todayColor;

   boost::shared_ptr<yApi::IYPluginApi> m_api;
   std::string m_deviceName;
   std::string m_newPeriod;

   bool m_deviceCreated;
   int m_revision;

   static const std::string m_tag_ADSC;
   static const std::string m_tag_VTIC;
   static const std::string m_tag_LTARF;
   static const std::string m_tag_EASF;
   static const std::string m_tag_STGE;
   static const std::string m_tag_EAIT;
   static const std::string m_tag_SINST1;
   static const std::string m_tag_SINST2;
   static const std::string m_tag_SINST3;
   static const std::string m_tag_SINSTS;
   static const std::string m_tag_UMOY1;
   static const std::string m_tag_UMOY2;
   static const std::string m_tag_UMOY3;

   // index 0 : new index
   // index 1 : old index
   unsigned char m_activeIndex[2];
   bool m_production;
};