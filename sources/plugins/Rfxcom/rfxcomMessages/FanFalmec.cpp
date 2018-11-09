#include "stdafx.h"
#include "FanFalmec.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CFanFalmec::CFanFalmec()
      : m_cmndByte(0),
        m_light(boost::make_shared<yApi::historization::CSwitch>("light")),
        m_powerOff(boost::make_shared<yApi::historization::CEvent>("powerOff")),
        m_speed1(boost::make_shared<yApi::historization::CEvent>("speed1")),
        m_speed2(boost::make_shared<yApi::historization::CEvent>("speed2")),
        m_speed3(boost::make_shared<yApi::historization::CEvent>("speed3")),
        m_speed4(boost::make_shared<yApi::historization::CEvent>("speed4")),
        m_timer1(boost::make_shared<yApi::historization::CEvent>("timer1")),
        m_timer2(boost::make_shared<yApi::historization::CEvent>("timer2")),
        m_timer3(boost::make_shared<yApi::historization::CEvent>("timer3")),
        m_timer4(boost::make_shared<yApi::historization::CEvent>("timer4")),
        m_keywords({m_light, m_powerOff, m_speed1, m_speed2, m_speed3, m_speed4, m_timer1, m_timer2, m_timer3, m_timer4})
   {
   }


   CFanFalmec::~CFanFalmec()
   {
   }


   std::string CFanFalmec::getModel() const
   {
      return "Falmec";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CFanFalmec::keywords() const
   {
      return m_keywords;
   }

   void CFanFalmec::set(const std::string& keyword, const std::string& yadomsCommand)
   {
      if (boost::iequals(keyword, m_light->getKeyword()))
      {
         m_light->setCommand(yadomsCommand);
         m_cmndByte = m_light.get() ? fan_FalmecLightOn : fan_FalmecLightOff;
      }
      if (boost::iequals(keyword, m_powerOff->getKeyword()))
      {
         m_cmndByte = fan_FalmecPower;
      }
      else if (boost::iequals(keyword, m_speed1->getKeyword()))
      {
         m_cmndByte = fan_FalmecSpeed1;
      }
      else if (boost::iequals(keyword, m_speed2->getKeyword()))
      {
         m_cmndByte = fan_FalmecSpeed2;
      }
      else if (boost::iequals(keyword, m_speed3->getKeyword()))
      {
         m_cmndByte = fan_FalmecSpeed3;
      }
      else if (boost::iequals(keyword, m_speed4->getKeyword()))
      {
         m_cmndByte = fan_FalmecSpeed4;
      }
      else if (boost::iequals(keyword, m_timer1->getKeyword()))
      {
         m_cmndByte = fan_FalmecTimer1;
      }
      else if (boost::iequals(keyword, m_timer2->getKeyword()))
      {
         m_cmndByte = fan_FalmecTimer2;
      }
      else if (boost::iequals(keyword, m_timer3->getKeyword()))
      {
         m_cmndByte = fan_FalmecTimer3;
      }
      else if (boost::iequals(keyword, m_timer4->getKeyword()))
      {
         m_cmndByte = fan_FalmecTimer4;
      }
   }

   void CFanFalmec::reset()
   {
      m_cmndByte = 0;
      m_light->set(false);
   }

   unsigned char CFanFalmec::toProtocolCmnd() const
   {
      return m_cmndByte;
   }

   unsigned long CFanFalmec::idFromConfiguration(const shared::CDataContainer& configuration) const
   {
      return configuration.get<unsigned int>("id");
   }
} // namespace rfxcomMessages
