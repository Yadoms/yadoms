#include "stdafx.h"
#include "FanLucciAirDc2.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CFanLucciAirDc2::CFanLucciAirDc2()
      : m_cmndByte(0),
        m_light(boost::make_shared<yApi::historization::CSwitch>("light")),
        m_off(boost::make_shared<yApi::historization::CEvent>("off")),
        m_speed1(boost::make_shared<yApi::historization::CEvent>("speed1")),
        m_speed2(boost::make_shared<yApi::historization::CEvent>("speed2")),
        m_speed3(boost::make_shared<yApi::historization::CEvent>("speed3")),
        m_speed4(boost::make_shared<yApi::historization::CEvent>("speed4")),
        m_speed5(boost::make_shared<yApi::historization::CEvent>("speed5")),
        m_speed6(boost::make_shared<yApi::historization::CEvent>("speed6")),
        m_reverse(boost::make_shared<yApi::historization::CEvent>("reverse")),
        m_keywords({m_light, m_off, m_speed1, m_speed2, m_speed3, m_speed4, m_speed5, m_speed6, m_reverse})
   {
   }


   CFanLucciAirDc2::~CFanLucciAirDc2()
   {
   }


   std::string CFanLucciAirDc2::getModel() const
   {
      return "Lucci Air DCII";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CFanLucciAirDc2::keywords() const
   {
      return m_keywords;
   }

   void CFanLucciAirDc2::set(const std::string& keyword, const std::string& yadomsCommand)
   {
      if (boost::iequals(keyword, m_light->getKeyword()))
      {
         m_light->setCommand(yadomsCommand);
         m_cmndByte = m_light.get() ? fan_LucciDCIILight : 0x00;
      }
      if (boost::iequals(keyword, m_off->getKeyword()))
      {
         m_cmndByte = fan_LucciDCIIOff;
      }
      else if (boost::iequals(keyword, m_speed1->getKeyword()))
      {
         m_cmndByte = fan_LucciDCII1;
      }
      else if (boost::iequals(keyword, m_speed2->getKeyword()))
      {
         m_cmndByte = fan_LucciDCII2;
      }
      else if (boost::iequals(keyword, m_speed3->getKeyword()))
      {
         m_cmndByte = fan_LucciDCII3;
      }
      else if (boost::iequals(keyword, m_speed4->getKeyword()))
      {
         m_cmndByte = fan_LucciDCII4;
      }
      else if (boost::iequals(keyword, m_speed5->getKeyword()))
      {
         m_cmndByte = fan_LucciDCII5;
      }
      else if (boost::iequals(keyword, m_speed6->getKeyword()))
      {
         m_cmndByte = fan_LucciDCII6;
      }
      else if (boost::iequals(keyword, m_reverse->getKeyword()))
      {
         m_cmndByte = fan_LucciDCIIReverse;
      }
   }

   void CFanLucciAirDc2::reset()
   {
      m_cmndByte = 0;
      m_light->set(false);
   }

   unsigned char CFanLucciAirDc2::toProtocolCmnd() const
   {
      return m_cmndByte;
   }

   unsigned long CFanLucciAirDc2::idFromConfiguration(const shared::CDataContainerSharedPtr& configuration) const
   {
      return configuration->get<unsigned int>("id");
   }
} // namespace rfxcomMessages
