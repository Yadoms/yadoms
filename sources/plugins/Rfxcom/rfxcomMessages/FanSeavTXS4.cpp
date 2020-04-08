#include "stdafx.h"
#include "FanSeavTXS4.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CFanSeavTxs4::CFanSeavTxs4()
      : m_cmndByte(0),
        m_speed1(boost::make_shared<yApi::historization::CEvent>("speed1")),
        m_speed2(boost::make_shared<yApi::historization::CEvent>("speed2")),
        m_speed3(boost::make_shared<yApi::historization::CEvent>("speed3")),
        m_speed4(boost::make_shared<yApi::historization::CEvent>("speed4")),
        m_keywords({m_speed1, m_speed2, m_speed3, m_speed4})
   {
   }


   CFanSeavTxs4::~CFanSeavTxs4()
   {
   }


   std::string CFanSeavTxs4::getModel() const
   {
      return "SEAV TXS4";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CFanSeavTxs4::keywords() const
   {
      return m_keywords;
   }

   void CFanSeavTxs4::set(const std::string& keyword, const std::string& yadomsCommand)
   {
      if (boost::iequals(keyword, m_speed1->getKeyword()))
      {
         m_cmndByte = fan_T1;
      }
      else if (boost::iequals(keyword, m_speed2->getKeyword()))
      {
         m_cmndByte = fan_T2;
      }
      else if (boost::iequals(keyword, m_speed3->getKeyword()))
      {
         m_cmndByte = fan_T3;
      }
      else if (boost::iequals(keyword, m_speed4->getKeyword()))
      {
         m_cmndByte = fan_T4;
      }
   }

   void CFanSeavTxs4::reset()
   {
      m_cmndByte = 0;
   }

   unsigned char CFanSeavTxs4::toProtocolCmnd() const
   {
      return m_cmndByte;
   }

   unsigned long CFanSeavTxs4::idFromConfiguration(const shared::CDataContainerSharedPtr& configuration) const
   {
      return configuration->get<unsigned int>("id") |
         ((configuration->get<bool>("sw2-1") ? 0x80 : 0x00) |
            (configuration->get<bool>("sw2-2") ? 0x40 : 0x00) << 16);
   }
} // namespace rfxcomMessages
