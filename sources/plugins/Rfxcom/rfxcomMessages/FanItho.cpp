#include "stdafx.h"
#include "FanItho.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CFanItho::CFanItho()
      : m_cmndByte(0),
        m_speed1(boost::make_shared<yApi::historization::CEvent>("speed1")),
        m_speed2(boost::make_shared<yApi::historization::CEvent>("speed2")),
        m_speed3(boost::make_shared<yApi::historization::CEvent>("speed3")),
        m_timer(boost::make_shared<yApi::historization::CEvent>("timer")),
        m_notAtHome(boost::make_shared<yApi::historization::CEvent>("notAtHome")),
        m_keywords({m_speed1, m_speed2, m_speed3, m_timer, m_notAtHome})
   {
   }


   CFanItho::~CFanItho()
   {
   }


   std::string CFanItho::getModel() const
   {
      return "Itho CVE RFT";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CFanItho::keywords() const
   {
      return m_keywords;
   }

   void CFanItho::set(const std::string& keyword, const std::string& yadomsCommand)
   {
      if (boost::iequals(keyword, m_speed1->getKeyword()))
      {
         m_cmndByte = fan_Itho1;
      }
      else if (boost::iequals(keyword, m_speed2->getKeyword()))
      {
         m_cmndByte = fan_Itho2;
      }
      else if (boost::iequals(keyword, m_speed3->getKeyword()))
      {
         m_cmndByte = fan_Itho3;
      }
      else if (boost::iequals(keyword, m_timer->getKeyword()))
      {
         m_cmndByte = fan_IthoTimer;
      }
      else if (boost::iequals(keyword, m_notAtHome->getKeyword()))
      {
         m_cmndByte = fan_IthoNotAtHome;
      }
   }

   void CFanItho::reset()
   {
      m_cmndByte = 0;
   }

   unsigned char CFanItho::toProtocolCmnd() const
   {
      return m_cmndByte;
   }

   unsigned long CFanItho::idFromConfiguration(const boost::shared_ptr<shared::CDataContainer>& configuration) const
   {
      return configuration->get<unsigned int>("id");
   }
} // namespace rfxcomMessages
