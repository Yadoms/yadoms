#include "stdafx.h"
#include "FanLucciAirDc.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CFanLucciAirDc::CFanLucciAirDc()
      : m_cmndByte(0),
        m_light(boost::make_shared<yApi::historization::CSwitch>("light")),
        m_power(boost::make_shared<yApi::historization::CSwitch>("power")),
        m_minus(boost::make_shared<yApi::historization::CEvent>("minus")),
        m_plus(boost::make_shared<yApi::historization::CEvent>("plus")),
        m_reverse(boost::make_shared<yApi::historization::CEvent>("reverse")),
        m_naturalFlow(boost::make_shared<yApi::historization::CEvent>("naturalFlow")),
        m_keywords({m_light, m_power, m_minus, m_plus, m_reverse, m_naturalFlow})
   {
   }


   CFanLucciAirDc::~CFanLucciAirDc()
   {
   }


   std::string CFanLucciAirDc::getModel() const
   {
      return "Lucci Air DC";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CFanLucciAirDc::keywords() const
   {
      return m_keywords;
   }

   void CFanLucciAirDc::set(const std::string& keyword, const std::string& yadomsCommand)
   {
      if (boost::iequals(keyword, m_light->getKeyword()))
      {
         m_light->setCommand(yadomsCommand);
         m_cmndByte = m_light.get() ? fan_LucciDCLight : 0x00;
      }
      else if (boost::iequals(keyword, m_power->getKeyword()))
      {
         m_power->setCommand(yadomsCommand);
         m_cmndByte = m_power.get() ? fan_LucciDCPower : 0x00;
      }
      else if (boost::iequals(keyword, m_minus->getKeyword()))
      {
         m_cmndByte = fan_LucciDCMin;
      }
      else if (boost::iequals(keyword, m_plus->getKeyword()))
      {
         m_cmndByte = fan_LucciDCPlus;
      }
      else if (boost::iequals(keyword, m_reverse->getKeyword()))
      {
         m_cmndByte = fan_LucciDCReverse;
      }
      else if (boost::iequals(keyword, m_naturalFlow->getKeyword()))
      {
         m_cmndByte = fan_LucciDCNaturalflow;
      }
   }

   void CFanLucciAirDc::reset()
   {
      m_cmndByte = 0;
      m_light->set(false);
      m_power->set(false);
   }

   unsigned char CFanLucciAirDc::toProtocolCmnd() const
   {
      return m_cmndByte;
   }

   unsigned long CFanLucciAirDc::idFromConfiguration(const boost::shared_ptr<shared::CDataContainer>& configuration) const
   {
      return configuration->get<unsigned int>("id");
   }
} // namespace rfxcomMessages
