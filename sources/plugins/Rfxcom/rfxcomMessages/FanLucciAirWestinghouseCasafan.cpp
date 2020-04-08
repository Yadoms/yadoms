#include "stdafx.h"
#include "FanLucciAirWestinghouseCasafan.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CFanLucciAirWestinghouseCasafan::CFanLucciAirWestinghouseCasafan(unsigned char subType)
      : m_subType(subType),
        m_cmndByte(0),
        m_light(boost::make_shared<yApi::historization::CSwitch>("light")),
        m_fanSpeedOff(boost::make_shared<yApi::historization::CEvent>("fanSpeedOff")),
        m_fanSpeedLow(boost::make_shared<yApi::historization::CEvent>("fanSpeedLow")),
        m_fanSpeedMed(boost::make_shared<yApi::historization::CEvent>("fanSpeedMed")),
        m_fanSpeedHigh(boost::make_shared<yApi::historization::CEvent>("fanSpeedHigh")),
        m_keywords({m_light, m_fanSpeedOff, m_fanSpeedLow, m_fanSpeedMed, m_fanSpeedHigh})
   {
   }


   CFanLucciAirWestinghouseCasafan::~CFanLucciAirWestinghouseCasafan()
   {
   }


   std::string CFanLucciAirWestinghouseCasafan::getModel() const
   {
      switch (m_subType)
      {
      case sTypeLucciAir: return "Lucci Air fan";
      case kRfxValueWestinghouse: return "Lucci Air fan";
      case kRfxValueCasafan: return "Lucci Air fan";
      default: return "Unknown";
      }
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CFanLucciAirWestinghouseCasafan::keywords() const
   {
      return m_keywords;
   }

   void CFanLucciAirWestinghouseCasafan::set(const std::string& keyword, const std::string& yadomsCommand)
   {
      if (boost::iequals(keyword, m_light->getKeyword()))
      {
         m_light->setCommand(yadomsCommand);
         m_cmndByte = m_light.get() ? fan_LucciLight : 0x00;
      }
      else if (boost::iequals(keyword, m_fanSpeedOff->getKeyword()))
      {
         m_cmndByte = fan_LucciOff;
      }
      else if (boost::iequals(keyword, m_fanSpeedLow->getKeyword()))
      {
         m_cmndByte = fan_LucciLow;
      }
      else if (boost::iequals(keyword, m_fanSpeedMed->getKeyword()))
      {
         m_cmndByte = fan_LucciMed;
      }
      else if (boost::iequals(keyword, m_fanSpeedHigh->getKeyword()))
      {
         m_cmndByte = fan_LucciHi;
      }
   }

   void CFanLucciAirWestinghouseCasafan::reset()
   {
      m_cmndByte = 0;
      m_light->set(false);
   }

   unsigned char CFanLucciAirWestinghouseCasafan::toProtocolCmnd() const
   {
      return m_cmndByte;
   }

   unsigned long CFanLucciAirWestinghouseCasafan::idFromConfiguration(const shared::CDataContainerSharedPtr& configuration) const
   {
      return configuration->get<unsigned int>("id");
   }
} // namespace rfxcomMessages
