#include "stdafx.h"
#include "FanFt1211R.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CFanFt1211R::CFanFt1211R()
      : m_cmndByte(0),
        m_light(boost::make_shared<yApi::historization::CSwitch>("light")),
        m_power(boost::make_shared<yApi::historization::CSwitch>("power")),
        m_speed1(boost::make_shared<yApi::historization::CEvent>("speed1")),
        m_speed2(boost::make_shared<yApi::historization::CEvent>("speed2")),
        m_speed3(boost::make_shared<yApi::historization::CEvent>("speed3")),
        m_speed4(boost::make_shared<yApi::historization::CEvent>("speed4")),
        m_speed5(boost::make_shared<yApi::historization::CEvent>("speed5")),
        m_reverse(boost::make_shared<yApi::historization::CEvent>("reverse")),
        m_stopIn1Hour(boost::make_shared<yApi::historization::CEvent>("stopIn1Hour")),
        m_stopIn4Hour(boost::make_shared<yApi::historization::CEvent>("stopIn4Hour")),
        m_stopIn8Hour(boost::make_shared<yApi::historization::CEvent>("stopIn8Hour")),
        m_keywords({m_light, m_power, m_speed1, m_speed2, m_speed3, m_speed4, m_speed5, m_reverse, m_stopIn1Hour, m_stopIn4Hour, m_stopIn8Hour})
   {
   }


   CFanFt1211R::~CFanFt1211R()
   {
   }


   std::string CFanFt1211R::getModel() const
   {
      return "FT1211R fan controller";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CFanFt1211R::keywords() const
   {
      return m_keywords;
   }

   void CFanFt1211R::set(const std::string& keyword, const std::string& yadomsCommand)
   {
      if (boost::iequals(keyword, m_light->getKeyword()))
      {
         m_light->setCommand(yadomsCommand);
         m_cmndByte = m_light.get() ? fan_FT1211Rlight : 0x00;
      }
      if (boost::iequals(keyword, m_power->getKeyword()))
      {
         m_power->setCommand(yadomsCommand);
         m_cmndByte = m_power.get() ? fan_FT1211Rpower : 0x00;
      }
      else if (boost::iequals(keyword, m_speed1->getKeyword()))
      {
         m_cmndByte = fan_FT1211R1;
      }
      else if (boost::iequals(keyword, m_speed2->getKeyword()))
      {
         m_cmndByte = fan_FT1211R2;
      }
      else if (boost::iequals(keyword, m_speed3->getKeyword()))
      {
         m_cmndByte = fan_FT1211R3;
      }
      else if (boost::iequals(keyword, m_speed4->getKeyword()))
      {
         m_cmndByte = fan_FT1211R4;
      }
      else if (boost::iequals(keyword, m_speed5->getKeyword()))
      {
         m_cmndByte = fan_FT1211R5;
      }
      else if (boost::iequals(keyword, m_reverse->getKeyword()))
      {
         m_cmndByte = fan_FT1211Rfr;
      }
      else if (boost::iequals(keyword, m_stopIn1Hour->getKeyword()))
      {
         m_cmndByte = fan_FT1211R1H;
      }
      else if (boost::iequals(keyword, m_stopIn4Hour->getKeyword()))
      {
         m_cmndByte = fan_FT1211R4H;
      }
      else if (boost::iequals(keyword, m_stopIn8Hour->getKeyword()))
      {
         m_cmndByte = fan_FT1211R8H;
      }
   }

   void CFanFt1211R::reset()
   {
      m_cmndByte = 0;
      m_light->set(false);
   }

   unsigned char CFanFt1211R::toProtocolCmnd() const
   {
      return m_cmndByte;
   }

   unsigned long CFanFt1211R::idFromConfiguration(const shared::CDataContainerSharedPtr& configuration) const
   {
      return configuration->get<unsigned int>("id");
   }
} // namespace rfxcomMessages
