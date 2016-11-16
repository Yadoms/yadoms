#include "stdafx.h"
#include "Profile_D2_01_12.h"
#include "../bitsetHelpers.hpp"

CProfile_D2_01_12::CProfile_D2_01_12()
   : m_switch1(boost::make_shared<yApi::historization::CSwitch>("Switch 1")),
     m_switch2(boost::make_shared<yApi::historization::CSwitch>("Switch 2")),
     m_historizers({m_switch1 , m_switch2})
{
}

CProfile_D2_01_12::~CProfile_D2_01_12()
{
}

unsigned int CProfile_D2_01_12::id() const
{
   return 0x12;
}

const std::string& CProfile_D2_01_12::title() const
{
   static const std::string title("Slot-in module with 2 channels");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_12::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_12::states(const boost::dynamic_bitset<>& data) const
{
   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   auto ioChannel = bitset_extract(data, 11, 5);
   auto state = bitset_extract(data, 17, 1) ? true : false;
   switch (ioChannel)
   {
   case 0:
      m_switch1->set(state);
      historizers.push_back(m_switch1);
      break;
   case 1:
      m_switch2->set(state);
      historizers.push_back(m_switch2);
      break;
   default:
      std::cout << "Profile D2_01_12 : receive unsupported ioChannel value " << ioChannel << std::endl;
      break;
   }
   return historizers;
}

void CProfile_D2_01_12::sendConfiguration(const shared::CDataContainer& deviceConfiguration) const
{
   //TODO
}
