#include "stdafx.h"
#include "Profile_D2_01_12.h"

CProfile_D2_01_12::CProfile_D2_01_12()
   : m_historizers({})
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

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CProfile_D2_01_12::historizers() const
{
   return m_historizers;
}

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CProfile_D2_01_12::states(const boost::dynamic_bitset<>& data) const
{
   return m_historizers;
}

