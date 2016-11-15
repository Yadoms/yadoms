#include "stdafx.h"
#include "Profile_D2_01_0F.h"

CProfile_D2_01_0F::CProfile_D2_01_0F()
   : m_historizers({})
{
}

CProfile_D2_01_0F::~CProfile_D2_01_0F()
{
}

unsigned int CProfile_D2_01_0F::id() const
{
   return 0x0F;
}

const std::string& CProfile_D2_01_0F::title() const
{
   static const std::string title("Slot-in module with 1 channel");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0F::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0F::states(const boost::dynamic_bitset<>& data) const
{
   //TODO
   return m_historizers;
}

