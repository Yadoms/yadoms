#include "stdafx.h"
#include "FanSiemensSF01.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CFanSiemensSf01::CFanSiemensSf01()
      : m_cmndByte(0),
        m_light(boost::make_shared<yApi::historization::CSwitch>("light")),
        m_timer(boost::make_shared<yApi::historization::CEvent>("timer")),
        m_minus(boost::make_shared<yApi::historization::CEvent>("minus")),
        m_plus(boost::make_shared<yApi::historization::CEvent>("plus")),
        m_keywords({m_light, m_timer, m_minus, m_plus})
   {
   }


   CFanSiemensSf01::~CFanSiemensSf01()
   {
   }


   std::string CFanSiemensSf01::getModel() const
   {
      return "Siemens SF01";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CFanSiemensSf01::keywords() const
   {
      return m_keywords;
   }

   void CFanSiemensSf01::set(const std::string& keyword, const std::string& yadomsCommand)
   {
      if (boost::iequals(keyword, m_light->getKeyword()))
      {
         m_light->setCommand(yadomsCommand);
         m_cmndByte = m_light.get() ? fan_sLight : 0x00;
      }
      else if (boost::iequals(keyword, m_timer->getKeyword()))
      {
         m_cmndByte = fan_sTimer;
      }
      else if (boost::iequals(keyword, m_minus->getKeyword()))
      {
         m_cmndByte = fan_sMin;
      }
      else if (boost::iequals(keyword, m_plus->getKeyword()))
      {
         m_cmndByte = fan_sPlus;
      }
   }

   void CFanSiemensSf01::reset()
   {
      m_cmndByte = 0;
      m_light->set(false);
   }

   unsigned char CFanSiemensSf01::toProtocolCmnd() const
   {
      return m_cmndByte;
   }

   unsigned long CFanSiemensSf01::idFromConfiguration(const boost::shared_ptr<shared::CDataContainer>& configuration) const
   {
      return configuration->get<unsigned int>("id");
   }
} // namespace rfxcomMessages
